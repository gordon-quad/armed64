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

static report_mouse_t state;
uint8_t mouse_idle = 0;

report_mouse_t *
mouse_report()
{
    return &state;
}

void
mouse_event(event_t *event, bool pressed)
{
    if (pressed) {
        state.buttons = event->mouse.button;
        state.x = event->mouse.x;
        state.y = event->mouse.y;
        state.h = state.v = 0;
        usb_update_mouse(&state);
    }
}

void
wheel_event(event_t *event, bool pressed)
{
    if (pressed) {
        state.buttons = event->wheel.button;
        state.h = event->wheel.h;
        state.v = event->wheel.v;
        state.x = state.y = 0;
        usb_update_mouse(&state);
    }
}
