/*
 * This file is part of the armed64 distribution (https://github.com/gordon-quad/armed64)
 * Copyright (c) 2015-2016 by Willem Dijkstra <wpd@xs4all.nl>.
 * Copyright (c) 2017 by Gordon Quad <gordon@niflheim.info>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include "config.h"
#include "clock.h"
#include "led.h"
#include "matrix.h"
#include "i2c.h"

static uint8_t recv_buf[CONFIG_LEN] = {0x00, 0x00, 0x00, 0x00};
static uint8_t idx = 0;
static bool pec_err = false;
static bool active = false;

static uint32_t i2c_timer;

void i2c2_er_isr(void)
{
    uint32_t foo = I2C2_SR1;
    I2C2_SR1 &= 0xff;
    pec_err = (foo & I2C_SR1_PECERR) != 0;
    if (foo & I2C_SR1_AF)
        active = false;
}

void i2c2_ev_isr(void)
{
    uint32_t foo = (I2C2_SR1 | (I2C2_SR2 << 16)) & 0xffffffff;
    // transfering start
    if ((foo & I2C_SR1_ADDR) && (foo & (I2C_SR2_TRA << 16)))
    {
        idx = 0;
        i2c_send_data(I2C2, (uint8_t)(matrix.row[idx++]));
        i2c_timer = timer_set(I2C_TIMEOUT);
        active = true;
    }
    // receiving start
    else if ((foo & I2C_SR1_ADDR) && !(foo & (I2C_SR2_TRA << 16)))
    {
        pec_err = false;
        idx = 0;
    }
    // transfering
    else if ((foo & I2C_SR1_TxE) && (foo & I2C_SR1_BTF))
    {
        if (idx >= ROWS_NUM)
            I2C2_CR1 |= I2C_CR1_PEC;
        else
            i2c_send_data(I2C2, (uint8_t)(matrix.row[idx++]));
    }
    // receiving
    else if ((foo & I2C_SR1_RxNE))
    {
        if (idx < CONFIG_LEN)
            recv_buf[idx++] = i2c_get_data(I2C2);
        if (idx >= CONFIG_LEN)
        {
            I2C2_CR1 |= I2C_CR1_PEC;
            i2c_get_data(I2C2);
        }
    }
    else if (foo & I2C_SR1_STOPF)
    {
        active = false;
        i2c_peripheral_enable(I2C2);
        if (!pec_err)
        {
            uint8_t i;
            uint8_t leds = 0;
            for (i = 0; i < LEDS_NUM; i++)
                leds |= (recv_buf[i] << i) & (1 << i);
            debounce_config = recv_buf[LEDS_NUM];
            led_set(leds);
        }
    }
}

void i2c_init(void)
{
      rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                  GPIO_I2C2_SCL | GPIO_I2C2_SDA);

    i2c_peripheral_disable(I2C2);
    i2c_set_clock_frequency(I2C2, I2C_CR2_FREQ_36MHZ);
    i2c_set_fast_mode(I2C2);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITEVTEN);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITERREN);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITBUFEN);
    nvic_enable_irq(NVIC_I2C2_EV_IRQ);
    nvic_enable_irq(NVIC_I2C2_ER_IRQ);
    I2C2_CR1 |= I2C_CR1_ENPEC;
    i2c_peripheral_enable(I2C2);
    i2c_set_own_7bit_slave_address(I2C2, I2C_SLAVE);
    i2c_disable_dual_addressing_mode(I2C2);
    i2c_enable_ack(I2C2);
}

void i2c_process(void)
{
    if (active)
        if (timer_passed(i2c_timer))
        {
            i2c_reset(I2C2);
            i2c_init();
        }
}
