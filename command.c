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

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>

#include "config.h"
#include "command.h"
#include "keyboard.h"
#include "led.h"
#include "matrix.h"

extern bool serial_active;

void command_exec(event_t *event)
{
    uint8_t *nkro;
    switch(event->cmd.type)
    {
    case CMD_NKRO:
        nkro = keyboard_get_protocol();
        if (*nkro)
        {
            slave_leds[1] = 0x0;
            keyboard_set_protocol(0);
        }
        else
        {
            slave_leds[1] = 0x1;
            keyboard_set_protocol(1);
        }
        break;
    case CMD_SERIAL:
        serial_active = !serial_active;
        break;
    case CMD_RESET:
        __asm volatile ("dsb");
        scb_reset_core();
        break;
    default:
        break;
    }
}

