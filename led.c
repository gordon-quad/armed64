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

/*
 * Led
 *
 * Drive keyboard leds
 */

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "led.h"
#include "i2c.h"

uint8_t slave_leds[S_LEDS_NUM] = {0x0, 0x1, 0x0};

void
led_init(void)
{
    rcc_periph_clock_enable(LEDS_RCC);
    gpio_set_mode(LEDS_GPIO, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, LEDS_BV);
    gpio_set(LEDS_GPIO, LEDS_BV);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO1); 
    gpio_set(GPIOB, GPIO1);
}

void
led_set(uint8_t leds)
{
    if (leds & 0b000001)
        gpio_clear(LEDS_GPIO, LED1IO);
    else
        gpio_set(LEDS_GPIO, LED1IO);

    if (leds & 0b000010)
        gpio_clear(LEDS_GPIO, LED2IO);
    else
        gpio_set(LEDS_GPIO, LED2IO);

    if (leds & 0b000100)
        gpio_clear(LEDS_GPIO, LED3IO);
    else
        gpio_set(LEDS_GPIO, LED3IO);

}

#ifdef MASTER
void
slave_led_set(uint8_t leds)
{
    if (leds & 0b001)
        slave_leds[0] = 0x01;
    else
        slave_leds[0] = 0x00;

    if (leds & 0b010)
        slave_leds[2] = 0x01;
    else
        slave_leds[2] = 0x00;

    if (leds & 0b100)
        slave_leds[1] = 0x01;
    else
        slave_leds[1] = 0x00;
}
#endif
