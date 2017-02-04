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
 * clock
 *
 * Millisecond system clock that counts up incrementally.
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"

static volatile uint32_t system_ms = 0;

void
sys_tick_handler(void)
{
    system_ms++;
}

void
clock_init(void)
{
    systick_set_reload(rcc_ahb_frequency / 1000);
    STK_CVR = 0;
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    systick_interrupt_enable();
}

uint32_t
clock_now(void)
{
    return system_ms;
}

uint32_t
timer_set(uint32_t delay)
{
    return system_ms + delay;
}

bool
timer_passed(uint32_t timer)
{
    return (timer < system_ms);
}
