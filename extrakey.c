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
 * Extrakey
 *
 * Receive extrakey events and process them into usb communication with the
 * host.
 */

#include "elog.h"
#include "extrakey.h"

uint8_t extrakey_idle = 0;

static report_extrakey_t state;

void
extrakey_clear(void)
{
    state.id = REPORTID_CONSUMER;
    state.codel = state.codeh = 0;
    usb_update_extrakey(&state);
    state.id = REPORTID_SYSTEM;
    state.codel = state.codeh = 0;
    usb_update_extrakey(&state);
}

report_extrakey_t *
extrakey_report()
{
    return &state;
}

void
extrakey_consumer_event(event_t *event, bool pressed)
{
    state.id = REPORTID_CONSUMER;

    elog("extrakey consumer %04x %d\r\n", event->extra.code, pressed);

    if (pressed) {
        state.codel = event->extra.code & 0xff;
        state.codeh = event->extra.code >> 8;
    } else {
        state.codel = state.codeh = 0;
    }

    usb_update_extrakey(&state);
}

void
extrakey_system_event(event_t *event, bool pressed)
{
    state.id = REPORTID_SYSTEM;

    elog("extrakey consumer %04x %d\r\n", event->extra.code, pressed);

    if (pressed) {
        state.codel = event->extra.code & 0xff;
        state.codeh = event->extra.code >> 8;
    } else {
        state.codel = state.codeh = 0;
    }

    usb_update_extrakey(&state);
}
