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

#ifndef _MATRIX_H
#define _MATRIX_H

#include "config.h"

typedef uint32_t matrix_row_t;

typedef struct {
    matrix_row_t row[ROWS_NUM];
} matrix_t;

extern matrix_t matrix;

#ifdef MASTER
typedef struct {
    matrix_row_t row[S_ROWS_NUM];
} slave_matrix_t;

extern slave_matrix_t slave_matrix;
#endif

extern uint32_t debounce_config;

void matrix_init(void);
void matrix_scan(void);
void matrix_process(void);
void matrix_event(uint16_t row, uint16_t col, bool pressed);

#ifdef MASTER
void matrix_debug(void);
void slave_matrix_debug(void);
#endif

#endif
