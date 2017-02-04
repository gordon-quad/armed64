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
 * A keymap consists of <layers> * <rows> * <colums> of scanmap_t.
 *
 *
 * |87654321|87654321|87654321|87654321|
 * |--------+--------+--------+--------|
 * |type    |data1   |data2   |data3   |
 * |    0000|        |keymod  |scancode|
 * |    0001|        |extra key        |
 * |    0010|buttons |x       |y       |
 * |    0011|buttons |h       |v       |
 * |    0100|layer select              |
 * |--------+--------+--------+--------|
 */

#ifndef _KEYMAP_H
#define _KEYMAP_H
#include "config.h"
#include "usb.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct {
    uint8_t type;
    union {
        uint8_t data[3];
        struct {
            uint8_t ignore_group;
            uint8_t mod;
            uint8_t code;
        } __attribute__ ((packed)) key;
        struct {
            uint8_t empty2;
            uint16_t code;
        } __attribute__ ((packed)) extra;
        struct {
            uint8_t button;
            int8_t x;
            int8_t y;
        } __attribute__ ((packed)) mouse;
        struct {
            uint8_t button;
            int8_t h;
            int8_t v;
        } __attribute__ ((packed)) wheel;
        struct {
            uint8_t empty3;
            uint8_t empty4;
            uint8_t number;
        } __attribute__ ((packed)) layer;
        struct {
            uint8_t empty5;
            uint8_t empty6;
            uint8_t type;
        } __attribute__ ((packed)) cmd;
    };
} __attribute__ ((packed)) event_t;

extern event_t keymap[][MAX(ROWS_NUM, S_ROWS_NUM)][COLS_NUM + S_COLS_NUM];

enum {
    KMT_KEY,
    KMT_MOD,
    KMT_LAYER,
    KMT_LAYER_LOCK,
    KMT_LAYER_BASE,
    KMT_LAYER_PREV,
    KMT_LAYER_BASE_CHANGE,
    KMT_CONSUMER,
    KMT_SYSTEM,
    KMT_MOUSE,
    KMT_WHEEL,
    KMT_CMD,
    KMT_NONE
};

enum {
    CMD_RESET,
    CMD_NKRO,
    CMD_SERIAL
};

#define _K(Key)        {.type = KMT_KEY, .key = { .ignore_group = false, .mod = 0, .code = KEY_##Key }}
#define _KI(Key)       {.type = KMT_KEY, .key = { .ignore_group = true, .mod = 0, .code = KEY_##Key }}
#define _SK(Key)       {.type = KMT_KEY, .key = { .ignore_group = false, .mod = MODIFIER_BIT(KEY_RSHIFT), .code = KEY_##Key }}
#define _RK(Key)       {.type = KMT_KEY, .key = { .ignore_group = false, .mod = MODIFIER_BIT(KEY_RALT), .code = KEY_##Key }}
#define _KC(Code)      {.type = KMT_KEY, .key = { .ignore_group = false, .mod = 0, .code = 0x##Code }}
#define _S(Key)        {.type = KMT_MOD, .key = { .ignore_group = false, .code = KEY_##Key, .mod = MODIFIER_BIT(KEY_##Key) }}
#define _M(X,Y)        {.type = KMT_MOUSE, .mouse = {.button = 0, .x = X, .y = Y }}
#define _B(Button)     {.type = KMT_MOUSE, .mouse = {.button = Button, .x = 0, .y = 0}}
#define _W(H,V)        {.type = KMT_WHEEL, .wheel = {.button = 0, .h = H, .v = V }}
#define _C(Key)        {.type = KMT_CONSUMER, .extra = { .empty2 = 0, .code = CONSUMER_##Key }}
#define _Y(Key)        {.type = KMT_SYSTEM, .extra = { .empty2 = 0, .code = SYSTEM_##Key }}
#define _L(Layer)      {.type = KMT_LAYER, .layer = { .number = Layer }}
#define _LL(Layer)     {.type = KMT_LAYER_LOCK, .layer = { .number = Layer }}
#define _BL            {.type = KMT_LAYER_BASE}
#define _PL            {.type = KMT_LAYER_PREV}
#define _CBL(Layer)    {.type = KMT_LAYER_BASE_CHANGE, .layer = { .number = Layer }}
#define _CMD(Cmd)      {.type = KMT_CMD, .cmd = { .type = CMD_##Cmd }}
#define _N             {.type = KMT_NONE}

void keymap_leds(uint8_t leds);
void keymap_event(uint16_t row, uint16_t col, bool pressed);

#endif
