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
 * mouse
 *
 * Receive mouse events and process them into usb communication with the
 * host.
 */

#include "mouse.h"
#include "clock.h"

static report_mouse_t state;
uint8_t mouse_idle = 0;

static int8_t mouse_x = 0;
static int8_t mouse_y = 0;
static int8_t wheel_x = 0;
static int8_t wheel_y = 0;
static uint32_t mouse_timer = 0;
static uint8_t buttons = 0;
static uint8_t accel = 1;

report_mouse_t *
mouse_report()
{
    return &state;
}

void
mouse_event(event_t *event, bool pressed)
{
    if (pressed)
    {
        buttons |= event->mouse.button;
        switch (event->mouse.dir)
        {
        case MOUSE_DIR_LEFT:
            mouse_x += -1;
            break;
        case MOUSE_DIR_RIGHT:
            mouse_x += +1;
            break;
        case MOUSE_DIR_UP:
            mouse_y += -1;
            break;
        case MOUSE_DIR_DOWN:
            mouse_y += +1;
            break;
        case WHEEL_DIR_LEFT:
            wheel_x += -1;
            break;
        case WHEEL_DIR_RIGHT:
            wheel_x += +1;
            break;
        case WHEEL_DIR_UP:
            wheel_y += -1;
            break;
        case WHEEL_DIR_DOWN:
            wheel_y += +1;
            break;
        }
        if (event->mouse.accel != 0)
            accel = event->mouse.accel;
    }
    else
    {
        buttons &= ~event->mouse.button;
        switch (event->mouse.dir)
        {
        case MOUSE_DIR_LEFT:
            mouse_x -= -1;
            break;
        case MOUSE_DIR_RIGHT:
            mouse_x -= +1;
            break;
        case MOUSE_DIR_UP:
            mouse_y -= -1;
            break;
        case MOUSE_DIR_DOWN:
            mouse_y -= +1;
            break;
        case WHEEL_DIR_LEFT:
            wheel_x -= -1;
            break;
        case WHEEL_DIR_RIGHT:
            wheel_x -= +1;
            break;
        case WHEEL_DIR_UP:
            wheel_y -= -1;
            break;
        case WHEEL_DIR_DOWN:
            wheel_y -= +1;
            break;
        }
    }
}

static int8_t mouse_move(void)
{
    return (1 << accel);
}

static int8_t wheel_move(void)
{
    return accel;
}

void
mouse_process(void)
{
    if (timer_passed(mouse_timer))
    {
        mouse_timer = timer_set(MOUSE_INTERVAL);
        state.buttons = buttons;
        state.x = mouse_x * mouse_move();
        state.y = mouse_y * mouse_move();
        state.h = wheel_x * wheel_move();
        state.v = wheel_y * wheel_move();
        usb_update_mouse(&state);
    }
}
