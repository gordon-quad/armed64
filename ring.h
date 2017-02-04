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

#ifndef _RING_H
#define _RING_H

#include <stdint.h>

typedef int32_t ring_size_t;

typedef struct ring {
	uint8_t *data;
	ring_size_t size;
	uint32_t begin;
	uint32_t end;
} ring_t;

#define RING_SIZE(RING)  ((RING)->size - 1)
#define RING_DATA(RING)  (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

void ring_init(ring_t *ring, uint8_t *buf, ring_size_t size);
int32_t ring_write_ch(ring_t *ring, uint8_t ch);
int32_t ring_write(ring_t *ring, uint8_t *data, ring_size_t size);
int32_t ring_read_ch(ring_t *ring, uint8_t *ch);
int32_t ring_read(ring_t *ring, uint8_t *data, ring_size_t size);
int32_t ring_read_contineous(ring_t *ring, uint8_t **data);
uint32_t ring_mark(ring_t *ring);
uint32_t ring_marklen(ring_t *ring, uint32_t mark);
#endif
