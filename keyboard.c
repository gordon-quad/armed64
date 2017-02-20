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
 * Keyboard
 *
 * Receive keyboard events and process them into usb communication with the
 * host.
 */

#include "usb.h"
#include "keyboard.h"
#include "usb_keycode.h"
#include "led.h"
#include "elog.h"

static report_keyboard_t keyboard_state;
static bool keyboard_dirty = false;
bool keyboard_active = false;
uint8_t keyboard_idle = 0;

static report_nkro_t nkro_state;
bool nkro_active = true;
static bool nkro_dirty = false;
uint8_t nkro_idle = 0;

static uint32_t timer_out = 0;

void
keyboard_clear(void)
{
    int i;
    keyboard_state.mods = 0;
    nkro_state.mods = 0;
    for (i = 0; i < EP_SIZE_KEYBOARD - 2; i++)
        keyboard_state.keys[i] = 0;

    for (i = 0; i < EP_SIZE_NKRO - 1; i++)
        nkro_state.bits[i] = 0;

    if (nkro_active) {
        nkro_dirty = true;
    } else {
        keyboard_dirty = true;
    }
}

void
keyboard_set_protocol(uint8_t protocol)
{
    nkro_active = (protocol == 1);
}

uint8_t *
keyboard_get_protocol()
{
    return (uint8_t *) &nkro_active;
}
report_keyboard_t *
keyboard_report()
{
    return &keyboard_state;
}

report_nkro_t *
nkro_report()
{
    return &nkro_state;
}

void keyboard_process(void)
{
    uint32_t now = usb_now();
    if (timer_out >= now)
        return;

    if (keyboard_dirty) {
        usb_update_keyboard(&keyboard_state);
        keyboard_dirty = false;
    }

    if (nkro_dirty) {
        usb_update_nkro(&nkro_state);
        nkro_dirty = false;
    }

    timer_out = now+2;
}

void
keyboard_event(event_t *event, bool pressed)
{
    uint8_t key = event->key.code;
    uint8_t mod = event->key.mod;

    elog("key %02x %d\r\n", key, pressed);

    switch (key) {
    case KEY_LCTRL ... KEY_RGUI:
        if (pressed) {
            keyboard_add_modifier(key);
        } else {
            keyboard_del_modifier(key);
        }
        break;

    default:
        if (pressed) {
            if (mod != 0)
            {
                keyboard_state.mods |= mod;
                nkro_state.mods |= mod;
            }
            keyboard_add_key(key);
        } else {
            if (mod != 0)
            {
                keyboard_state.mods &= ~mod;
                nkro_state.mods &= ~mod;
            }
            keyboard_del_key(key);
        }
        break;
    }
}

void
keyboard_add_key(uint8_t key)
{
    uint8_t i;
    uint8_t k;

    if (nkro_active) {
        /*
         * NKRO is coded as n bits where each bit corresponds with an pressed
         * key. The first bit corresponds with the first real key that can be
         * pressed, i.e. KEY_A
         */
        k = key - KEY_A;
        if ((k >> 3) < sizeof(nkro_state.bits)) {
            nkro_state.bits[k >> 3] |= (1 << (k & 0x07));
            nkro_dirty = true;
            return;
        }
    }

    /* nkro inactive or keycode too large for nkro, on to boot keyboard */
    for (i = 0; i < sizeof(keyboard_state.keys); i++) {
        if ((keyboard_state.keys[i] == 0) ||
            (keyboard_state.keys[i] == key)) {
            keyboard_state.keys[i] = key;
            keyboard_dirty = true;
            return;
        }
    }
}

void
keyboard_del_key(uint8_t key)
{
    uint8_t i;
    uint8_t k;

    if (nkro_active) {
        k = key - KEY_A;
        if ((k >> 3) < sizeof(nkro_state.bits)) {
            nkro_state.bits[k >> 3] &= ~(1 << (k & 0x07));
            nkro_dirty = true;
            return;
        }
    }

    /* nkro inactive or keycode too large for nkro, on to boot keyboard */

    for (i = 0; i < sizeof(keyboard_state.keys); i++) {
        if (keyboard_state.keys[i] == key) {
            keyboard_state.keys[i] = 0;
            keyboard_dirty = true;
            return;
        }
    }
}

void
keyboard_add_modifier(uint8_t modifier)
{
    keyboard_state.mods |= MODIFIER_BIT(modifier);
    nkro_state.mods |= MODIFIER_BIT(modifier);

    if (nkro_active) {
        nkro_dirty = true;
    } else {
        keyboard_dirty = true;
    }
}

void
keyboard_del_modifier(uint8_t modifier)
{
    keyboard_state.mods &= ~MODIFIER_BIT(modifier);
    nkro_state.mods &= ~MODIFIER_BIT(modifier);

    if (nkro_active) {
        nkro_dirty = true;
    } else {
        keyboard_dirty = true;
    }
}
