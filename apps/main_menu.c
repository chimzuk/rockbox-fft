/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Bj�rn Stenberg
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "menu.h"
#include "tree.h"
#include "credits.h"
#include "lcd.h"
#include "button.h"
#include "kernel.h"
#include "main_menu.h"
#include "version.h"
#include "debug_menu.h"
#include "sprintf.h"
#include <string.h>
#include "playlist.h"
#include "settings.h"
#include "settings_menu.h"
#include "power.h"
#include "sound_menu.h"

#ifdef HAVE_LCD_BITMAP
#include "games_menu.h"
#include "screensavers_menu.h"
#include "bmp.h"
#include "icons.h"
#endif

int show_logo( void )
{
#ifdef HAVE_LCD_BITMAP
    char version[32];
    unsigned char *ptr=rockbox112x37;
    int height, i, font_h, font_w;

    lcd_clear_display();

    for(i=0; i < 37; i+=8) {
        /* the bitmap function doesn't work with full-height bitmaps
           so we "stripe" the logo output */
        lcd_bitmap(ptr, 0, 10+i, 112, (37-i)>8?8:37-i, false);
        ptr += 112;
    }
    height = 37;

#if 0
    /*
     * This code is not used anymore, but I kept it here since it shows
     * one way of using the BMP reader function to display an externally
     * providing logo.
     */
    unsigned char buffer[112 * 8];
    int width;

    int i;
    int eline;

    int failure;
    failure = read_bmp_file("/rockbox112.bmp", &width, &height, buffer);

    debugf("read_bmp_file() returned %d, width %d height %d\n",
           failure, width, height);

        for(i=0, eline=0; i < height; i+=8, eline++) {
            /* the bitmap function doesn't work with full-height bitmaps
               so we "stripe" the logo output */
            lcd_bitmap(&buffer[eline*width], 0, 10+i, width,
                       (height-i)>8?8:height-i, false);
        }
    }
#endif

    snprintf(version, sizeof(version), "Ver. %s", appsversion);
    lcd_getfontsize(0, &font_w, &font_h);
    lcd_putsxy((LCD_WIDTH/2) - ((strlen(version)*font_w)/2),
               height+10+font_h, version, 0);
    lcd_update();

#else
    char *rockbox = "ROCKbox!";
    lcd_clear_display();
#ifdef HAVE_NEW_CHARCELL_LCD
    lcd_double_height(true);
#endif
    lcd_puts(0, 0, rockbox);
    lcd_puts(0, 1, appsversion);
#endif

    return 0;
}

void show_credits(void)
{
    int j = 0;
    int btn;

    show_logo();
#ifdef HAVE_NEW_CHARCELL_LCD
    lcd_double_height(false);
#endif
    
    for (j = 0; j < 10; j++) {
        sleep((HZ*2)/10);

        btn = button_get(false);
        if (btn !=  BUTTON_NONE && !(btn & BUTTON_REL))
            return;
    }
    roll_credits();
}

#ifdef SIMULATOR
#define mp3buf 0
#define mp3end 0
#else
/* defined in linker script */
extern unsigned char mp3buf[];
extern unsigned char mp3end[];
#endif
void show_info(void)
{
    char s[32];
    int buflen = ((mp3end - mp3buf) * 100) / 0x100000;
    int integer, decimal;
    
    lcd_clear_display();
    lcd_puts(0, 0, "Rockbox info:");
    /* TODO: add disk size/usage info, battery charge etc here? */

#ifdef HAVE_RTC
    snprintf(s, sizeof(s), "Booted: %d times", global_settings.total_boots);
    lcd_puts(0, 2, s);
#endif

    integer = buflen / 100;
    decimal = buflen % 100;
#ifdef HAVE_LCD_CHARCELLS
    snprintf(s, sizeof(s), "Buf: %d.%02dM", integer, decimal);
#else
    snprintf(s, sizeof(s), "Buffer: %d.%02d Mb", integer, decimal);
#endif
    lcd_puts(0, 3, s);
    
#ifdef HAVE_LCD_CHARCELLS
    snprintf(s, sizeof(s), "Batt: %d%%", battery_level());
#else
    snprintf(s, sizeof(s), "Battery: %d%%", battery_level());
#endif
    lcd_puts(0, 4, s);
    
    lcd_update();

    /* Wait for a key to be pushed */
    while(button_get(true) & BUTTON_REL);
}

void main_menu(void)
{
    int m;

    /* main menu */
    struct menu_items items[] = {
        { "Sound Settings",     sound_menu        },
        { "General Settings",   settings_menu     },
#ifdef HAVE_LCD_BITMAP
        { "Games",              games_menu        },
        { "Screensavers",       screensavers_menu },
#endif
        { "Info",               show_info         },
        { "Version",            show_credits      },
#ifndef SIMULATOR
        { "Debug (keep out!)",  debug_menu        },
#endif
    };

    m=menu_init( items, sizeof items / sizeof(struct menu_items) );
    menu_run(m);
    menu_exit(m);
}
