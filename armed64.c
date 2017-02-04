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
 * Entrypoint and mainloop for the master keyboard
 * - Setup usb for enumeration after reset
 * - Once enumeration is complete, contineously call:
 *   - usb_poll to recieve serial input and leds states
 *   - matrix_process to detect and process matrix events
 *   - serial_out to write output
 */
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "clock.h"
#include "usb.h"
#include "serial.h"
#include "matrix.h"
#include "keyboard.h"
#include "serial.h"
#include "led.h"
#include "elog.h"
#include "usb_keycode.h"
#include "keymap.h"
#include "i2c.h"

static void
mcu_init(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

int
main(void)
{
    SCB_VTOR = (uint32_t) 0x08005000;
    mcu_init();
    clock_init();
    serial_init();
    led_init();
    matrix_init();
    i2c_init();
    usb_init();

    elog("initialized\r\n");

    while (1) {
        usb_poll();

        if (serial_active) {
            serial_out();
        }
        if (keyboard_active) {
            matrix_process();
            keyboard_process();
        }
        i2c_process();
    }
}

