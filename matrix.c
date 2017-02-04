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
 * Matrix
 *
 * Tasks:
 * - scan the hardware matrix
 * - determine if there is a keydown or keyup event
 * - if so, trigger keyboard, mouse, extrakey events
 */

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "clock.h"
#include "config.h"
#include "matrix.h"

#include "serial.h"
#include "keymap.h"
#include "i2c.h"


static uint32_t debounce;
static bool matrix_update;
matrix_t matrix;
static matrix_t matrix_debounce;
static matrix_t matrix_previous;
#ifdef MASTER
slave_matrix_t slave_matrix;
static slave_matrix_t slave_matrix_previous;
uint8_t show_matrix = 0;
#endif
uint32_t debounce_config = DEBOUNCE_MS;

/*
 * row_select
 *
 * Pull a row high
 */
static void
row_select(uint8_t r)
{
    GPIO_BSRR(ROWS_GPIO) = (1 << (r+3));
}

/*
 * row_clear
 *
 * Pull a row low
 */
static void
row_clear(void)
{
    GPIO_BSRR(ROWS_GPIO) = (ROWS_BV) << 16;
}

/*
 * col_read
 *
 * Read a column bits and return them in ascending order. Note that this
 * function hides the physical column wiring.
 */
static uint16_t
col_read(void)
{
    uint16_t c = (uint16_t)(GPIO_IDR(COLS_GPIO) & COLS_BV);

    return COLS_DECODE(c);
}

/*
 * matrix_init
 *
 * Initialize the matrix module. Must be called before any other function in
 * this module is called. Initializes the hardware and module local variables
 * we depend on.
 */
void
matrix_init(void)
{
    uint8_t i;

    rcc_periph_clock_enable(ROWS_RCC);
    rcc_periph_clock_enable(COLS_RCC);
    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR & ~AFIO_MAPR_SWJ_MASK);

    gpio_set_mode(ROWS_GPIO, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, ROWS_BV);
    gpio_set_mode(COLS_GPIO, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, COLS_BV);
    GPIO_ODR(COLS_GPIO) &= COLS_BV;

    row_clear();

    for (i = 0; i < ROWS_NUM; i++) {
        matrix.row[i] = 0;
        matrix_debounce.row[i] = 0;
        matrix_previous.row[i] = 0;
    }

#ifdef MASTER
    for (i = 0; i < S_ROWS_NUM; i++)
        slave_matrix_previous.row[i] = 0;
#endif
}

/*
 * matrix_scan
 *
 * Read the matrix; select each row, and read column value. Debounce by quickly
 * noting the current value, and only taking that value after it has been
 * stable for DEBOUNCE ms.
 */
void
matrix_scan()
{
    uint8_t r;
    uint16_t col;

    for (r = 0; r < ROWS_NUM; r++) {
        row_select(r);
        col = col_read();
        if (matrix_debounce.row[r] != col) {
            matrix_debounce.row[r] = col;
            matrix_update = true;
            debounce = timer_set(debounce_config);
        }
        row_clear();
    }

    if (matrix_update && timer_passed(debounce)) {
        matrix_update = false;
        for (r = 0; r < ROWS_NUM; r++) {
            matrix.row[r] = matrix_debounce.row[r];
        }
    }
}

/*
 * matrix_process
 *
 * Generate key up/down events depending on the current and previous scan
 * state.
 */
void
matrix_process()
{
    uint8_t r, c;
    uint16_t col, colbit;

    /* Make sure that we pick up new scan events */
    matrix_scan();

    /*
     * Check for scan events, even if the previous matrix scan returned
     * nothing. We might still have some unprocessed events in our previous
     * matrix.
     */

    for (r = 0; r < ROWS_NUM; r++) {
        col = matrix.row[r] ^ matrix_previous.row[r];
        if (col) {
            for (c = 0; c < COLS_NUM; c++) {
                colbit = (1 << c);
                if (col & colbit) {
#ifdef MASTER
                    if (show_matrix) matrix_debug();
                    keymap_event(r, c, matrix.row[r] & colbit);
#endif
                    matrix_previous.row[r] ^= colbit;
                }
            }
        }
    }

#ifdef MASTER
    for (r = 0; r < S_ROWS_NUM; r++) {
        col = slave_matrix.row[r] ^ slave_matrix_previous.row[r];
        if (col) {
            for (c = 0; c < COLS_NUM; c++) {
                colbit = (1 << c);
                if (col & colbit) {
                    if (show_matrix) slave_matrix_debug();
                    keymap_event(r, COLS_NUM + c, slave_matrix.row[r] & colbit);
                    slave_matrix_previous.row[r] ^= colbit;
                }
            }
        }
    }
#endif
}

/*
 * matrix_debug
 *
 * Debugging routine that will emit the current module local state
 */
#ifdef MASTER
void
matrix_debug(void)
{
    uint8_t i;

    printf("\r\n\r");
    for (i = 0; i < ROWS_NUM; i++) {
        printf("%02x :%02x\n", i, (unsigned int) matrix.row[i]);
    }
}

void
slave_matrix_debug(void)
{
    uint8_t i;

    printf("\r\n\r");
    for (i = 0; i < ROWS_NUM; i++) {
        printf("%02x :%02x\n", i, (unsigned int) slave_matrix.row[i]);
    }
}
#endif
