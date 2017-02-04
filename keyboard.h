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

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdint.h>
#include "keymap.h"
#include "usb.h"

extern bool keyboard_active;
extern uint8_t keyboard_idle;

void keyboard_clear(void);
void keyboard_set_protocol(uint8_t protocol);
uint8_t *keyboard_get_protocol(void);
report_keyboard_t *keyboard_report(void);
void keyboard_event(event_t *event, bool pressed);
void keyboard_add_key(uint8_t key);
void keyboard_del_key(uint8_t key);
void keyboard_add_modifier(uint8_t modifier);
void keyboard_del_modifier(uint8_t modifier);
void keyboard_process(void);

extern uint8_t nkro_idle;
report_nkro_t *nkro_report(void);

#endif
