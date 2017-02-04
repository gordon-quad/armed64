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
 * Keymap
 *
 * Holds the keymap and code to take on press and release events from the
 * matrix. These can then be passed on to the keyboard, mouse or extrakey
 * modules.
 */
#include <libopencm3/cm3/scb.h>

#include <stdint.h>
#include <string.h>
#include "keymap.h"
#include "usb_keycode.h"
#include "config.h"
#include "i2c.h"
#include "led.h"
#include "command.h"
#include "elog.h"

#include "keyboard.h"
#include "mouse.h"

#include "extrakey.h"

// Layer 0 - Base layer
// Layer 1 - Punctuation layer
// Layer 2 - Numeric layer
// Layer 3 - Mouse layer
// Layer 4 - Cyrillic layer
// Layer 5 - Command layer
// Layer 6 - Numeric fn layer
// Layer 7 - Cyrillic punctuation layer
// Layer 8 - Base layer with AltGr
// Layer 9 - Cyrillic layer with AltGr

static event_t *current[MAX(ROWS_NUM, S_ROWS_NUM)][COLS_NUM + S_COLS_NUM];

static uint8_t active_mods = 0;
static bool num_active = false;
static bool ignore_group_change = false;

event_t keymap[][MAX(ROWS_NUM, S_ROWS_NUM)][COLS_NUM + S_COLS_NUM] =
{
    // Base Layer
    {
        { _K(ESC),       _K(1),           _K(2),         _K(3),        _K(4),     _K(5),          _K(CAPS_LOCK),   _K(NUM_LOCK), _K(6),         _K(7),      _K(8),         _K(9),          _K(0),            _L(5) },
        { _S(LALT),      _K(Q),           _K(W),         _K(E),        _K(R),     _K(T),          _K(TAB),         _K(ENTER),    _K(Y),         _K(U),      _K(I),         _K(O),          _K(P),            _S(LALT) },
        { _S(LCTRL),     _K(A),           _K(S),         _K(D),        _K(F),     _K(G),          _S(LGUI),        _L(1),        _K(H),         _K(J),      _K(K),         _K(L),          _K(SEMICOLON),    _S(RCTRL) },
        { _S(LSHIFT),    _K(Z),           _K(X),         _K(C),        _K(V),     _K(B),          _K(BACKSPACE),   _K(SPACE),    _K(N),         _K(M),      _K(COMMA),     _K(PERIOD),     _K(SLASH),        _S(RSHIFT) },
        { _N,            _K(HOME),        _K(PAGE_DOWN), _K(PAGE_UP),  _K(END),   _N,             _N,              _N,           _N,            _K(LEFT),   _K(DOWN),      _K(UP),         _K(RIGHT),        _N },
    },
    // Punctuation Layer
    {
        { _K(ESC),       _K(F1),          _K(F2),        _K(F3),       _K(F4),    _K(F5),         _K(SCROLL_LOCK), _LL(3),       _K(F6),        _K(F7),     _K(F8),        _K(F9),         _K(F10),          _L(5) },
        { _S(LALT),      _SK(BACKTICK),   _K(BACKTICK),  _SK(4),       _SK(2),    _K(BACKSLASH),  _K(APP),         _K(APP),      _SK(6),        _SK(5),     _SK(8),        _K(F11),        _K(F12),          _S(LALT) },
        { _S(LCTRL),     _SK(LEFTBRACE),  _K(LEFTBRACE), _SK(9),       _K(QUOTE), _K(MINUS),      _L(8),           _N,           _SK(MINUS),    _SK(QUOTE), _SK(0),        _K(RIGHTBRACE), _SK(RIGHTBRACE),  _S(RCTRL) },
        { _S(LSHIFT),    _SK(1),          _SK(3),        _SK(COMMA),   _K(EQUAL), _SK(BACKSLASH), _K(DELETE),      _K(SPACE),    _SK(7),        _SK(EQUAL), _SK(PERIOD),   _C(PLAYPAUSE),  _C(MUTE),         _S(RSHIFT) },
        { _N,            _K(PRINTSCREEN), _K(PAUSE),     _K(INSERT),   _K(EXEC),  _N,             _N,              _N,           _N,            _C(REWIND), _C(VOLUMEDEC), _C(VOLUMEINC),  _C(FASTFORWARD),  _N },
    },
    // Numeric Layer
    {
        { _K(ESC),       _N,              _N,            _N,           _N,        _N,             _K(CAPS_LOCK),   _K(NUM_LOCK), _K(CLEAR),     _K(PAD_7),  _K(PAD_8),     _K(PAD_9),      _K(INSERT),       _L(5) },
        { _S(LALT),      _N,              _N,            _N,           _N,        _N,             _K(TAB),         _K(SPACE),    _K(PAD_SLASH), _K(PAD_4),  _K(PAD_5),     _K(PAD_6),      _K(PAD_ASTERISK), _K(TAB) },
        { _S(LCTRL),     _N,              _N,            _N,           _N,        _N,             _BL,             _L(6),        _K(PAD_MINUS), _K(PAD_1),  _K(PAD_2),     _K(PAD_3),      _K(PAD_PLUS),     _K(BACKSPACE) },
        { _S(LSHIFT),    _N,              _N,            _N,           _N,        _N,             _K(BACKSPACE),   _K(ENTER),    _K(PAD_EQUAL), _K(PAD_0),  _K(PAD_00),    _K(PAD_PERIOD), _K(PAD_COMMA),    _K(DELETE) },
        { _N,            _K(HOME),        _K(PAGE_DOWN), _K(PAGE_UP),  _K(END),   _N,             _N,              _N,           _N,            _K(LEFT),   _K(DOWN),      _K(UP),         _K(RIGHT),        _N },
    },
    // Mouse Layer
    {
        { _N,            _N,              _N,            _N,           _N,        _K(CAPS_LOCK),  _PL,             _PL,          _K(NUM_LOCK),  _N,         _N,            _N,             _N,               _L(5) },
        { _S(LALT),      _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _WL,        _WD,           _WU,            _WR,              _S(LALT) },
        { _S(LCTRL),     _MA(1),          _MA(2),        _MA(3),       _MA(4),    _B(BTN4),       _B(BTN2),        _BL,          _B(BTN5),      _ML,        _MD,           _MU,            _MR,              _S(RCTRL) },
        { _S(LSHIFT),    _N,              _N,            _N,           _N,        _N,             _B(BTN3),        _B(BTN1),     _N,            _N,         _N,            _N,             _N,               _S(RSHIFT) },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
    },
    // Cyrillic Layer
    {
        { _K(ESC),       _SK(1),          _SK(2),        _K(BACKTICK), _SK(4),    _SK(5),         _K(CAPS_LOCK),   _K(NUM_LOCK), _SK(6),        _SK(7),     _K(LEFTBRACE), _K(RIGHTBRACE), _K(QUOTE),        _L(5) },
        { _S(LALT),      _K(Q),           _K(W),         _K(E),        _K(R),     _K(T),          _K(TAB),         _K(ENTER),    _K(Y),         _K(U),      _K(I),         _K(O),          _K(P),            _S(LALT) },
        { _S(LCTRL),     _K(A),           _K(S),         _K(D),        _K(F),     _K(G),          _S(LGUI),        _L(7),        _K(H),         _K(J),      _K(K),         _K(L),          _K(SEMICOLON),    _S(RCTRL) },
        { _S(LSHIFT),    _K(Z),           _K(X),         _K(C),        _K(V),     _K(B),          _K(BACKSPACE),   _K(SPACE),    _K(N),         _K(M),      _K(COMMA),     _K(PERIOD),     _K(SLASH),        _S(RSHIFT) },
        { _N,            _K(HOME),        _K(PAGE_DOWN), _K(PAGE_UP),  _K(END),   _N,             _N,              _N,           _N,            _K(LEFT),   _K(DOWN),      _K(UP),         _K(RIGHT),        _N },
    },
    // Command layer
    {
        { _CMD(RESET),   _N,              _N,            _N,           _N,        _N,             _CMD(SERIAL),    _CMD(NKRO),   _N,            _N,         _N,            _N,             _N,               _L(5) },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
    },
    // Numeric Fn Layer
    {
        { _K(ESC),       _K(F1),          _K(F2),        _K(F3),       _K(F4),    _K(F5),         _K(SCROLL_LOCK), _N,           _K(F6),        _K(F7),     _K(F8),        _K(F9),         _K(F10),          _L(5) },
        { _S(LALT),      _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _K(F11),        _K(F12),          _K(LALT) },
        { _S(LCTRL),     _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _K(RCTRL) },
        { _S(LSHIFT),    _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _K(RSHIFT) },
        { _N,            _N,              _N,            _N,           _N,        _N,             _N,              _N,           _N,            _N,         _N,            _N,             _N,               _N },
    },
    // Cyrillic punctuation Layer
    {
        { _K(ESC),       _K(1),           _K(2),         _K(3),        _K(4),     _K(5),          _K(SCROLL_LOCK), _LL(3),       _K(6),         _K(7),      _K(8),         _K(9),          _K(0),            _L(5) },
        { _S(LALT),      _SK(BACKTICK),   _K(BACKTICK),  _SK(4),       _SK(2),    _K(BACKSLASH),  _K(APP),         _K(APP),      _SK(6),        _SK(5),     _SK(8),        _N,             _N,               _S(LALT) },
        { _S(LCTRL),     _SK(LEFTBRACE),  _K(LEFTBRACE), _SK(9),       _K(QUOTE), _K(MINUS),      _L(9),           _N,           _SK(MINUS),    _SK(QUOTE), _SK(0),        _K(RIGHTBRACE), _SK(RIGHTBRACE),  _S(RCTRL) },
        { _S(LSHIFT),    _SK(1),          _SK(3),        _SK(COMMA),   _K(EQUAL), _SK(BACKSLASH), _K(DELETE),      _K(SPACE),    _SK(7),        _SK(EQUAL), _SK(PERIOD),   _C(PLAYPAUSE),  _C(MUTE),         _S(RSHIFT) },
        { _N,            _K(PRINTSCREEN), _K(PAUSE),     _K(INSERT),   _K(EXEC),  _N,             _N,              _N,           _N,            _C(REWIND), _C(VOLUMEDEC), _C(VOLUMEINC),  _C(FASTFORWARD),  _N },
    },
    // Base Layer with AltGr
    {
        { _RK(BACKTICK), _RK(1),          _RK(2),        _RK(3),       _RK(4),    _RK(5),         _N,              _N,           _RK(6),        _RK(7),     _RK(8),        _RK(9),         _RK(0),            _RK(QUOTE) },
        { _S(LALT),      _RK(Q),          _RK(W),        _RK(E),       _RK(R),    _RK(T),         _K(TAB),         _K(ENTER),    _RK(Y),        _RK(U),     _RK(I),        _RK(O),         _RK(P),            _S(LALT) },
        { _S(LCTRL),     _RK(A),          _RK(S),        _RK(D),       _RK(F),    _RK(G),         _S(LGUI),        _L(1),        _RK(H),        _RK(J),     _RK(K),        _RK(L),         _RK(SEMICOLON),    _S(RCTRL) },
        { _S(LSHIFT),    _RK(Z),          _RK(X),        _RK(C),       _RK(V),    _RK(B),         _K(BACKSPACE),   _K(SPACE),    _RK(N),        _RK(M),     _RK(COMMA),    _RK(PERIOD),    _RK(SLASH),        _S(RSHIFT) },
        { _N,            _K(HOME),        _K(PAGE_DOWN), _K(PAGE_UP),  _K(END),   _N,             _N,              _N,           _N,            _K(LEFT),   _K(DOWN),      _K(UP),         _K(RIGHT),         _N },
    },
    // Cyrillic Layer with AltGr
    {
        { _RK(BACKTICK), _RK(1),          _RK(2),        _RK(3),       _RK(4),    _RK(5),         _N,              _N,           _RK(6),        _RK(7),     _RK(8),        _RK(9),         _RK(0),            _RK(QUOTE) },
        { _S(LALT),      _RK(Q),          _RK(W),        _RK(E),       _RK(R),    _RK(T),         _K(TAB),         _K(ENTER),    _RK(Y),        _RK(U),     _RK(I),        _RK(O),         _RK(P),            _S(LALT) },
        { _S(LCTRL),     _RK(A),          _RK(S),        _RK(D),       _RK(F),    _RK(G),         _S(LGUI),        _L(1),        _RK(H),        _RK(J),     _RK(K),        _RK(L),         _RK(SEMICOLON),    _S(RCTRL) },
        { _S(LSHIFT),    _RK(Z),          _RK(X),        _RK(C),       _RK(V),    _RK(B),         _K(BACKSPACE),   _K(SPACE),    _RK(N),        _RK(M),     _RK(COMMA),    _RK(PERIOD),    _RK(SLASH),        _S(RSHIFT) },
        { _N,            _K(HOME),        _K(PAGE_DOWN), _K(PAGE_UP),  _K(END),   _N,             _N,              _N,           _N,            _K(LEFT),   _K(DOWN),      _K(UP),         _K(RIGHT),         _N },
    },
};

static uint8_t base_layer = 0;
static uint8_t layer_stack[LAYER_STACK_MAX] = {0};
static uint8_t layer_idx = 0;

event_t layer_key[] = 
{
    _N,
    _N,
    _N,
    _N,
    _N,
    _N,
    _N,
    _KI(RGUI),
    _N,
    _KI(RGUI),
};

static void keymap_exec(event_t event, bool pressed);

static uint8_t
layer_peek(void)
{
    return layer_stack[layer_idx];
}

static uint8_t
layer_pop(void)
{
    if (layer_idx == 0)
    {
        keymap_exec(layer_key[layer_stack[layer_idx]], false);
        layer_stack[layer_idx] = base_layer;
        keymap_exec(layer_key[layer_stack[layer_idx]], true);
        elog("layer = %d\r\n", layer_stack[layer_idx]);
        return layer_stack[layer_idx];
    }
    elog("layer = %d\r\n", layer_stack[layer_idx-1]);
    keymap_exec(layer_key[layer_stack[layer_idx]], false);
    keymap_exec(layer_key[layer_stack[layer_idx-1]], true);
    return layer_stack[layer_idx--];
}

static bool
layer_push(uint8_t layer)
{
    if (layer_idx == (LAYER_STACK_MAX-1))
        return false;
    keymap_exec(layer_key[layer_stack[layer_idx]], false);
    layer_stack[++layer_idx] = layer;
    keymap_exec(layer_key[layer_stack[layer_idx]], true);
    elog("layer = %d\r\n", layer_stack[layer_idx]);
    return true;
}

static void
layer_base_change(uint8_t new_base)
{
    int i;
    if (layer_stack[layer_idx] == base_layer)
    {
        keymap_exec(layer_key[base_layer], false);
        keymap_exec(layer_key[new_base], true);
    }
    for (i = 0; i <= layer_idx; i++)
        if (layer_stack[i] == base_layer)
            layer_stack[i] = new_base;
    base_layer = new_base;
    elog("layer = %d\r\n", layer_stack[layer_idx]);
}

static void
keymap_exec(event_t event, bool pressed)
{
    switch (event.type) {
    case KMT_KEY:
        if (!pressed)
            event.key.mod &= ~active_mods;
        if (event.key.ignore_group)
            ignore_group_change = pressed;
        keyboard_event(&event, pressed);
        break;

    case KMT_MOD:
        if (pressed)
            active_mods |= event.key.mod;
        else
            active_mods &= ~event.key.mod;
        if (event.key.ignore_group)
            ignore_group_change = pressed;
        keyboard_event(&event, pressed);
        break;

    case KMT_MOUSE:
        mouse_event(&event, pressed);
        break;

    case KMT_CONSUMER:
        extrakey_consumer_event(&event, pressed);
        break;

    case KMT_SYSTEM:
        extrakey_system_event(&event, pressed);
        break;

    case KMT_LAYER:
        if (pressed)
            layer_push(event.layer.number % (sizeof(keymap)));
        else
            layer_pop();
        break;

    case KMT_LAYER_LOCK:
        if (pressed)
        {
            slave_leds[0] = 1;
            layer_stack[0] = event.layer.number % (sizeof(keymap));
        }
        break;

    case KMT_LAYER_UNLOCK:
        if (pressed)
        {
            slave_leds[0] = 0;
            layer_stack[0] = base_layer;
        }
        break;

    case KMT_LAYER_BASE:
        if (pressed)
            layer_push(base_layer);
        else
            layer_pop();
        break;

    case KMT_LAYER_BASE_CHANGE:
        if (pressed)
            layer_base_change(event.layer.number % (sizeof(keymap)));
        break;

    case KMT_CMD:
        if (pressed)
            command_exec(&event);
        break;

    case KMT_NONE:
        break;
    }
}

void
keymap_event(uint16_t row, uint16_t col, bool pressed)
{
    event_t event;
    if (pressed)
        current[row][col] = &keymap[layer_peek()][row][col];
    memcpy(&event, current[row][col], sizeof(event_t));

    keymap_exec(event, pressed);
}

void keymap_leds(uint8_t leds)
{
    // Num Lock
    if (leds & 0b001)
    {
        if (!num_active)
        {
            num_active = true;
            layer_push(2);
        }
    }
    else 
        if (num_active)
        {
            num_active = false;
            layer_pop();
        }
    // Caps Lock
    if (!ignore_group_change)
    {
        if (leds & 0b010)
            layer_base_change(4);
        else
            layer_base_change(0);
    }

    if (ignore_group_change)
        led_set((leds & 0b101) | 0b010);
    else
        led_set(leds & 0b111);
}
