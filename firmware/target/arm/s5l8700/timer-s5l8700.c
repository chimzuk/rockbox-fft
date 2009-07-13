/***************************************************************************
*             __________               __   ___.
*   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
*   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
*   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
*   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
*                     \/            \/     \/    \/            \/
* $Id$
*
* Copyright (C) 2009 Bertrik Sikken
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
* KIND, either express or implied.
*
****************************************************************************/

#include "config.h"

#include "inttypes.h"
#include "s5l8700.h"
#include "system.h"
#include "timer.h"

/*  Timer driver for the S5L8700

    The S5L8700 timer resolution is only 16-bit. Larger counts are done by using
    both the clock-select and the clock prescaler to bring the count down into
    the range of the 16-bit counter.
    
    TODO: investigate why the timer seems to count twice as fast as expected
*/

void INT_TIMERD(void)
{
    /* clear interrupt */
    TDCON = TDCON;
    
    if (pfn_timer != NULL) {
        pfn_timer();
    }
}

bool timer_set(long cycles, bool start)
{
    static const int cs_table[] = {1, 2, 4, 6};
    int prescale, cs;
    long count;

    /* stop and clear timer */
    TDCMD = (1 << 1);   /* TD_CLR */

    /* optionally unregister any previously registered timer user */
    if (start) {
        if (pfn_unregister != NULL) {
            pfn_unregister();
            pfn_unregister = NULL;
        }
    }

    /* scale the count down with the clock select */
    for (cs = 0; cs < 4; cs++) {
        count = cycles >> cs_table[cs];
        if ((count < 65536) || (cs == 3)) {
            break;
        }
    }
    
    /* scale the count down with the prescaler */
    prescale = 1;
    while (count >= 65536) {
        count >>= 1;
        prescale <<= 1;
    }

    /* configure timer */
    TDCON = (1 << 12) |     /* TD_INT0_EN */
            (cs << 8) |     /* TS_CS */
            (0 << 4);       /* TD_MODE_SEL, 0 = interval mode */
    TDPRE = prescale - 1;
    TDDATA0 = count;
    TDCMD = (1 << 0);       /* TD_ENABLE */

    /* enable interrupt */
    INTMSK |= (1 << 9);
    
    return true;
}

bool timer_start(void)
{
    TDCMD = (1 << 0);       /* TD_ENABLE */
    return true;
}

void timer_stop(void)
{
    TDCMD = (0 << 0);       /* TD_ENABLE */
}
