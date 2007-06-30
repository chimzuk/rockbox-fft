/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2004 by Jens Arnold
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#ifndef __ATA_MMC_H__
#define __ATA_MMC_H__
#include "hotswap.h"

void mmc_enable_int_flash_clock(bool on);
bool mmc_detect(void);
tCardInfo *mmc_card_info(int card_no);
bool mmc_touched(void);
bool mmc_usb_active(int delayticks);

#ifdef HAVE_HOTSWAP
void mmc_enable_monitoring(bool on);
#endif

#endif
