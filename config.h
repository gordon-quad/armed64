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

#ifndef _CONFIG_H
#define _CONFIG_H

#include "usb.h"

#define SERIAL_BUF_SIZEIN (EP_SIZE_SERIALDATAOUT*4)
#define SERIAL_BUF_SIZEOUT (EP_SIZE_SERIALDATAIN*4)

/*
 * Matrix pinout definition:
 *
 * PB3-B7 = row driver
 * PB0-B6 = column reader
 *
 * COLS_DECODE is used after column GPIO reading to get a consecutive bitfield
 */

#define ROWS_NUM        5
#define ROWS_GPIO       GPIOB
#define ROWS_RCC        RCC_GPIOB
#define ROWS_BV         0b11111000

#define COLS_NUM        7
#define COLS_GPIO       GPIOA
#define COLS_RCC        RCC_GPIOA
#define COLS_BV         0b1111111
#define COLS_DECODE(x)  (x & 0b1111111)

#define DEBOUNCE_MS     10

#define LEDS_GPIO       GPIOC
#define LEDS_RCC        RCC_GPIOC
#define LEDS_BV         (GPIO13 | GPIO14 | GPIO15)
#define LED1IO          GPIO13
#define LED2IO          GPIO14
#define LED3IO          GPIO15
#define LEDS_NUM        3

#define I2C_SLAVE       0x50

#define S_ROWS_NUM      5
#define S_COLS_NUM      7
#define S_LEDS_NUM      3

#define KEY_STATE_LEN   4
#define CONFIG_LEN      (LEDS_NUM + 1)

#define S_CONFIG_LEN    (S_LEDS_NUM + 1)

#define I2C_TIMEOUT     25
#define I2C_POLL        5

#define LAYER_STACK_MAX 5

#endif
