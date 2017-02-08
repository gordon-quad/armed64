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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>

#include "i2c.h"
#include "elog.h"
#include "clock.h"
#include "config.h"

#include "matrix.h"
#include "led.h"

static int idx;

static uint8_t snd_buf[S_CONFIG_LEN];
static uint8_t rcv_buf[S_ROWS_NUM];

static bool pec_err = true;
static uint32_t i2c_timer;
static bool transfer = false;
static bool active = false;

void i2c2_er_isr(void)
{
    uint32_t foo = I2C2_SR1;
    I2C2_SR1 &= 0x00ff;
    pec_err = (foo & I2C_SR1_PECERR) != 0;
}

void i2c2_ev_isr(void)
{
    uint32_t foo = I2C2_SR1;
    foo |= I2C2_SR2 << 16;
    foo &= 0xffffffff;
    // address start
    if ((foo & I2C_SR1_SB) &&
        (foo & (I2C_SR2_BUSY << 16)) &&
        (foo & (I2C_SR2_MSL) << 16))
    {
        idx = 0;
        if (transfer)
        {
            i2c_send_7bit_address(I2C2, 0x50, I2C_WRITE);
            i2c_enable_ack(I2C2);
        }
        else
        {
            pec_err = false;
            i2c_send_7bit_address(I2C2, 0x50, I2C_READ);
            i2c_enable_ack(I2C2);
        }
    }
    // transfer start
    else if ((foo & I2C_SR1_ADDR) &&
             (foo & I2C_SR1_TxE) &&
             (foo & (I2C_SR2_BUSY << 16)) &&
             (foo & (I2C_SR2_MSL << 16)) &&
             (foo & (I2C_SR2_TRA << 16)))
        i2c_send_data(I2C2, snd_buf[idx++]);
    // transfering
    else if ((foo & (I2C_SR1_TxE)) &&
             (foo & (I2C_SR2_BUSY << 16)) &&
             (foo & (I2C_SR2_MSL << 16)) &&
             (foo & (I2C_SR2_TRA << 16)))
    {
        if (idx < S_CONFIG_LEN)
            i2c_send_data(I2C2, snd_buf[idx++]);
        else if (idx == S_CONFIG_LEN)
        {
            I2C2_CR1 |= I2C_CR1_PEC;
            idx++;
        }
        else
        {
            active = false;
            i2c_send_stop(I2C2);
            i2c_disable_ack(I2C2);
        }
    }
    // reciving
    else if ((foo & I2C_SR1_RxNE) && 
             (foo & (I2C_SR2_BUSY << 16)) &&
             (foo & (I2C_SR2_MSL << 16)))
    {
        if (idx < S_ROWS_NUM)
            rcv_buf[idx++] = i2c_get_data(I2C2);
        if (idx == S_ROWS_NUM-1)
        {
            I2C2_CR1 |= I2C_CR1_PEC;
            i2c_nack_next(I2C2);
            i2c_disable_ack(I2C2);
        }
        if (idx == S_ROWS_NUM)
            idx++;
        if (idx > S_ROWS_NUM)
        {
            active = false;
            i2c_get_data(I2C2);
            i2c_send_stop(I2C2);
        }
    }
}

void i2c_init(void)
{
    i2c_timer = timer_set(I2C_POLL+2);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C2);
    rcc_periph_clock_enable(RCC_AFIO);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                  GPIO_I2C2_SCL | GPIO_I2C2_SDA);

    i2c_peripheral_disable(I2C2);

    i2c_set_clock_frequency(I2C2, I2C_CR2_FREQ_36MHZ);

    i2c_set_fast_mode(I2C2);

    i2c_set_ccr(I2C2, 0x1e);
    i2c_set_trise(I2C2, 0x0c);
    //i2c_set_dutycycle(I2C2, I2C_CCR_DUTY_16_DIV_9);

    i2c_set_own_7bit_slave_address(I2C2, 0x50);

    i2c_enable_interrupt(I2C2, I2C_CR2_ITEVTEN);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITERREN);
    i2c_enable_interrupt(I2C2, I2C_CR2_ITBUFEN);

    nvic_enable_irq(NVIC_I2C2_EV_IRQ);
    nvic_enable_irq(NVIC_I2C2_ER_IRQ);
    nvic_set_priority(NVIC_I2C2_EV_IRQ, 1);
    nvic_set_priority(NVIC_I2C2_ER_IRQ, 2);

    I2C2_CR1 |= I2C_CR1_ENPEC;

    i2c_peripheral_enable(I2C2);
}

void i2c_process(void)
{
    int i;
    if (timer_passed(i2c_timer))
    {
        if ((!transfer) && (!pec_err))
            for (i = 0; i < S_ROWS_NUM; i++)
                slave_matrix.row[i] = rcv_buf[i];
        transfer = !transfer;
        if (transfer)
        {
            snd_buf[0] = slave_leds[0];
            snd_buf[1] = slave_leds[1];
            snd_buf[2] = slave_leds[2];
            snd_buf[3] = DEBOUNCE_MS;
        }
        if (active)
        {
            i2c_reset(I2C2);
            i2c_init();
        }
        active = true;
        i2c_timer = timer_set(I2C_POLL);
        i2c_send_start(I2C2);
    }
}
