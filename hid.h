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
 * USB magic numbers used in hid requests
 */
#ifndef _HID_H
#define _HID_H

/* HID Class specific bRequest */
#define USBHID_REQ_GET_REPORT    0x01
#define USBHID_REQ_GET_IDLE      0x02
#define USBHID_REQ_GET_PROTOCOL  0x03
#define USBHID_REQ_SET_REPORT    0x09
#define USBHID_REQ_SET_IDLE      0x0a
#define USBHID_REQ_SET_PROTOCOL  0x0b

#endif /* _HID_H */
