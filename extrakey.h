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

#ifndef _EXTRAKEY_H
#define _EXTRAKEY_H

#include <stdint.h>
#include "keymap.h"
#include "usb.h"

extern uint8_t extrakey_idle;

void extrakey_clear(void);
report_extrakey_t *extrakey_report(void);
void extrakey_consumer_event(event_t *event, bool pressed);
void extrakey_system_event(event_t *event, bool pressed);

#endif
