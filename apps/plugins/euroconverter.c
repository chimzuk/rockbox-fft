/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2003 Pierre Delore
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "plugin.h"

#ifdef HAVE_LCD_CHARCELLS

/* Euro converter for the player */
/*
Use:
+ : Digit +1 
- : Digit -1
PLAY : Next digit
STOP : Prev digit
ON : RESET
ON+PLAY : Swap Euro<>Home
MENU : Display the Menu
        Currency -> Allows to choose the currency
        Exit-> Exit the plugin

Notes:
I don't use float.
I use signed long long (64 bits).
A value have 5 digits after the . (123.45 = 12345000)

To do:
- The Irish currency needs 6 digits after the . to have sufficient precision on big number
- Improve the config save format to be like rockbox setting 
  (use of settings_parseline(...) of setting.c ? must be extern and export in the config.h)  
*/

/* Name and path of the config file*/
#define CFGFILE "/.rockbox/euroconverter.cfg"

/*Pattern for the converter*/
static unsigned char pattern_euro[]={0x07, 0x08, 0x1E, 0x10, 0x1E, 0x08, 0x07};    /* � */
static unsigned char pattern_home[]={0x04, 0x0A, 0x11, 0x1F, 0x11, 0x11, 0x1F};    /* Home icon*/

/* 1 euro = ... (remenber 5 digits after the .)*/
static int currency[12]={
                            655957,     /*FRF France*/
                            195583,     /*DEM Germany*/
                            1376030,    /*ATS Austria*/
                            4033990,    /*BEF Belgium*/
                            16638600,   /*ESP Spain*/
                            594573,     /*FIM Finland*/
                            78756,      /*IEP Irland*/
                            193627000,  /*ITL Italy*/
                            4033990,    /*LUF Luxemburg*/
                            220371,     /*NLG Netherlands*/
                            20048200,   /*PTE Portugal*/
                            34075100,   /*GRD Greece*/
                        };

/* Number of digit of the currency (for the display) */
static int nb_digit[12]={
                            2,   /*FRF France*/
                            2,   /*DEM Germany*/
                            2,   /*ATS Austria*/
                            2,   /*BEF Belgium*/
                            0,   /*ESP Spain*/
                            2,   /*FIM Finland*/
                            2,   /*IEP Irland*/
                            0,   /*ITL Italy*/
                            2,   /*LUF Luxemburg*/
                            2,   /*NLG Netherlands*/
                            0,   /*PTE Portugal*/
                            0    /*GRD Greece*/
                        };

/* max euro to have home currency */
static long long max_euro[12]={
                                  99999999000LL,    /*FRF France      999 999.99 */
                                  99999999000LL,    /*DEM Germany     999 999.99 */
                                  99999999000LL,    /*ATS Austria     999 999.99 */
                                  99999999000LL,    /*BEF Belgium     999 999.99 */
                                  99999999000LL,    /*ESP Spain       99 999 999 */
                                  99999999000LL,    /*FIM Finland     999 999.99 */
                                  99999999000LL,    /*IEP Irland      999 999.99 */
                                  51645690000LL,    /*ITL Italy       999 999 999 */
                                  99999999000LL,    /*LUF Luxemburg   999 999.99 */
                                  99999999000LL,   /*NLG Netherlands 999 999.99 */
                                  99999999000LL,   /*PTE Portugal    99 999 999 */
                                  29347028000LL    /*GRD Greece      99 999 999 */
                              };

/* max home to have euro currency */
/* 92233720300000 Limitation due to the max capacity of long long (2^63)*/
static long long max_curr[12]={
                                  99999999000LL,   /*FRF France      152449.02 */
                                  99999999000LL,   /*DEM Germany     511291.88 */
                                  99999999000LL,   /*ATS Austria     72672.83 */
                                  99999999000LL,   /*BEF Belgium     24789.35 */
                                  92233720300000LL, /*ESP Spain      5543358.23 */
                                  99999999000LL,   /*FIM Finland     168187.92 */
                                  9999999900LL,    /*IEP Irland      1269744.51 exact value=1269738.07 */
                                  92233720300000LL,/*ITL Italy       476347.41 */
                                  99999999000LL,   /*LUF Luxemburg   24789.35 */
                                  99999999000LL,   /*NLG Netherlands 453780.21 */
                                  92233720300000LL, /*PTE Portugal    4600598.57 */
                                  92233720300000LL  /*GRD Greece      2706777.69 */
                              };

static unsigned char *abbrev_str[12] = {
                                          "...FRF...",  /*France*/
                                          "...DEM...",  /*Germany*/
                                          "...ATS...",  /*Austria*/
                                          "...BEF...",  /*Belgium*/
                                          "...ESP...",  /*Spain*/
                                          "...FIM...",  /*Finland*/
                                          "...IEP...",  /*Irland*/
                                          "...ITL...",  /*Italy*/
                                          "...LUF...",  /*Luxemburg*/
                                          "...NLG...",  /*Netherlands*/
                                          "...PTE...",  /*Portugal*/
                                          "...GRD..."   /*Greece*/
                                      };


static unsigned char heuro,hhome; /*Handles for the new patterns*/

static struct plugin_api* rb;

static int country;     /*Country selected*/
static int cur_pos;     /*Cursor position*/
static long long inc;   


/* 64bits*64 bits with 5 digits after the . */
static long long mul(long long a, long long b)
{
    return((a*b)/100000);
}


/* 64bits/64 bits with 5 digits after the . */
static long long mydiv(long long a, long long b)
{
    return((a*100000)/b);
}


/* 123.45=12345000  split => i=123 f=45000*/
static void split(long long v, long long* i, long long* f)
{
    long long t;

    t=v/100000LL;
    (*i)=t;
    (*f)=(v-(t*100000LL));
}


/* result=10^n */
static long long pow10(int n)
{
    int i;
    long long r;

    r=1;
    for (i=0;i<n;i++)
        r=r*10LL;
    return(r);
}


/* round the i.f at n digit after the . */
static void round(long long* i, long long* f, int n)
{

    long long m;
    int add=0;

    m=(int)pow10(5-n-1);
    if (((*f)/m)%10>=5)
        add=1;
    if (n>0)
    {
        (*f)=((*f)/(int)pow10(5-n))+add;
        if ((*f)==100LL)
        {
            (*i)+=1;
            (*f)=0;
        }
    }
    else
    {
        (*i)+=add;
        (*f)=0;
    }
}


/* Display the imput and the result
    pos: false : first line 
       : true  : second line
*/
static void display(long long euro, long long home, bool pos)
{
    char c1,c2;
    long long i,f;
    unsigned char str[20];
    unsigned char s1[20];
    unsigned char s2[20];

    if (pos)
    {   /*Edit the second line*/
        c1=0x20;
        rb->strcpy(s1,"%c%c%6d.%02d");
        c2=0x81;
        if (nb_digit[country]==2)
            rb->strcpy(s2,"%c%c%06d.%02d");
        else
            rb->strcpy(s2,"%c%c%09d");
    }
    else
    {
        c1=0x81;
        rb->strcpy(s1,"%c%c%06d.%02d");
        c2=0x20;
        if (nb_digit[country]==2)
            rb->strcpy(s2,"%c%c%6d.%02d");
        else
            rb->strcpy(s2,"%c%c%9d");
    }

    rb->lcd_remove_cursor();
    /*First line*/
    split(euro,&i,&f);
    if (pos)
        round(&i,&f,2);
    rb->snprintf(str,sizeof(str),s1,heuro,c1,(int)i,(int)f);

    if (!pos)
    {
        rb->lcd_puts(0,0,str);
        rb->lcd_put_cursor(10-cur_pos,0,0x5F);
    }
    else
        rb->lcd_puts_scroll(0,0,str);

    /*Second line*/
    split(home,&i,&f);
    if (!pos)
        round(&i,&f,nb_digit[country]);
    rb->snprintf(str,sizeof(str),s2,hhome,c2,(int)i,(int)f);
    if (pos)
    {
        rb->lcd_puts(0,1,str);
        rb->lcd_put_cursor(10-cur_pos,1,0x5F);
    }
    else
        rb->lcd_puts_scroll(0,1,str);
}


/* Show country Abbreviation */
static void show_abbrev(void)
{
//    rb->lcd_remove_cursor();
    rb->lcd_puts(2,1,abbrev_str[country]);
    rb->sleep(HZ*3/4);
}


/* Save the config to disk */
static void save_config(void)
{
    int fd;

    fd = rb->creat(CFGFILE,0);
    if (fd < 0)
    {
        rb->lcd_clear_display();
        rb->lcd_puts(0,0,"Impossible");
        rb->lcd_puts(0,1,"to save cfg");
        rb->sleep(HZ);
    }
    else
    {
        rb->fprintf(fd, "%c", country+0x30);
        rb->close(fd);
    }
    return;
}


/* Load the config from disk */
static void load_config(void)
{
    int fd;
    char line[128];

    fd = rb->open(CFGFILE, O_RDONLY);
    if (fd < 0)
        return;

    rb->read(fd, line,1);
    country=line[0]-0x30;

    if ((country>11)|| (country<0))
        country=0;
    
    rb->close(fd);
    return;
}


/*Currency choice*/
static void currency_menu(void)
{
    unsigned char *currency_str[12] = {
                                          "France",
                                          "Germany",
                                          "Austria",
                                          "Belgium",
                                          "Spain",
                                          "Finland",
                                          "Irland",
                                          "Italy",
                                          "Luxemburg",
                                          "Netherlands",
                                          "Portugal",
                                          "Greece"
                                      };
    int c=country;

    rb->lcd_clear_display();
    while (true)
    {
        rb->lcd_puts(0,0,"Currency:");
        rb->lcd_puts(0,1,currency_str[c]);
        switch (rb->button_get(true))
        {
            case BUTTON_RIGHT|BUTTON_REL:
                c++;
                if (c>11)
                    c=0;
                break;
            case BUTTON_LEFT|BUTTON_REL:
                c--;
                if (c<0)
                    c=11;
                break;
            case BUTTON_PLAY|BUTTON_REL:
                country=c;
                save_config();
                return;
                break;
            case BUTTON_STOP|BUTTON_REL:
                return;
        }
    }
}


/* Display the choice menu. */
static int euro_menu(void)
{
    int c=0;


    while (true)
    {
        rb->lcd_clear_display();
        rb->lcd_puts(0,0," Currency");
        rb->lcd_puts(0,1," Exit");
        rb->lcd_putc(0,c,0x81);

        switch (rb->button_get(true))
        {
            case BUTTON_RIGHT|BUTTON_REL:
                c=1;
                break;
            case BUTTON_LEFT|BUTTON_REL:
                c=0;
                break;
            case BUTTON_PLAY|BUTTON_REL:
                if (c==0)
                    currency_menu();
                else
                    return 1;
                break;
            case BUTTON_STOP|BUTTON_REL:
                return 0;
        }
    }
}


/* Call when the program end */
static void euro_exit(void)
{
    //Restore the old pattern (i don't find another way to do this. An idea?)
    rb->lcd_unlock_pattern(heuro);
    rb->lcd_unlock_pattern(hhome);

    //Clear the screen
    rb->lcd_clear_display();
}


/* this is the plugin entry point */
enum plugin_status plugin_start(struct plugin_api* api, void* parameter)
{
    bool end, pos;
    long long e,h,old_e,old_h;

    /* this macro should be called as the first thing you do in the plugin.
       it test that the api version and model the plugin was compiled for
       matches the machine it is running on */
    TEST_PLUGIN_API(api);

    /* if you don't use the parameter, you can do like
       this to avoid the compiler warning about it */
    (void)parameter;

    /* if you are using a global api pointer, don't forget to copy it!
       otherwise you will get lovely "I04: IllInstr" errors... :-) */
    rb = api;

    /*Get the pattern handle*/
    heuro=rb->lcd_get_locked_pattern();
    hhome=rb->lcd_get_locked_pattern();
    rb->lcd_define_pattern(heuro, pattern_euro);
    rb->lcd_define_pattern(hhome, pattern_home);

    h=0;
    e=0;
    end=false;
    pos=false;
    country=0;
    cur_pos=3;
    inc=100000;

    load_config();

    /*Empty the event queue*/
    while (rb->button_get(false)!=BUTTON_NONE) ;

    display(e,h,false);
    show_abbrev();
    display(e,h,false);

    /*Main loop*/
    while(end!=true)
    {
        switch (rb->button_get(true))
        {
            case BUTTON_MENU|BUTTON_REL:
                switch (euro_menu())
                {
                    case 1:
                        end=true;
                        break;
                }
                if (!pos)
                {
                    if (e>max_euro[country])
                        e=0;
                    cur_pos=3;
                }
                else
                {
                    if (h>max_curr[country])
                        h=0;
                    if (nb_digit[country]==2)
                        cur_pos=3;
                    else
                        cur_pos=0;
                }

                display(e,h,pos);
                break;

            case BUTTON_ON | BUTTON_PLAY:
                pos=!pos;

            case BUTTON_ON | BUTTON_REL:
                e=0;
                h=0;
                if (!pos)
                {
                    cur_pos=3;
                    inc=100000;
                }
                else
                {
                    inc=100000;
                    if (nb_digit[country]==2)
                        cur_pos=3;
                    else
                        cur_pos=0;
                }
                show_abbrev();
                break;

            case BUTTON_STOP|BUTTON_REL:
                cur_pos--;
                if (!pos)
                {
                    if (cur_pos<0)
                        cur_pos=0;
                    if (cur_pos==2)
                        cur_pos=1;
                    if (cur_pos>2)
                        inc=pow10(3+cur_pos-1);
                    else
                        inc=pow10(3+cur_pos);
                }
                else
                {
                    if (cur_pos<0)
                        cur_pos=0;
                    if (nb_digit[country]==2)
                    {
                        if (cur_pos==2)
                            cur_pos=1;
                        if (cur_pos>2)
                            inc=pow10(3+cur_pos-1);
                        else
                            inc=pow10(3+cur_pos);
                    }
                    else
                        inc=pow10(5+cur_pos);

                }
                break;

            case BUTTON_PLAY|BUTTON_REL:
                cur_pos++;
                if (!pos)
                {
                    if (cur_pos>8)
                        cur_pos=8;
                    if (cur_pos==2)
                        cur_pos=3;
                    if (cur_pos>2)
                        inc=pow10(3+cur_pos-1);
                    else
                        inc=pow10(3+cur_pos);
                }
                else
                {
                    if (cur_pos>8)
                        cur_pos=8;
                    if (nb_digit[country]==2)
                    {
                        if (cur_pos==2)
                            cur_pos=3;
                        if (cur_pos>2)
                            inc=pow10(3+cur_pos-1);
                        else
                            inc=pow10(3+cur_pos);
                    }
                    else
                        inc=pow10(5+cur_pos);
                }
                break;

            case BUTTON_LEFT|BUTTON_REL:
            case BUTTON_LEFT|BUTTON_REPEAT:
                if (!pos)
                {
                    e-=inc;
                    if (e<0)
                        e=0;
                }
                else
                {
                    h-=inc;
                    if (h<0)
                        h=0;
                }
                break;

            case BUTTON_RIGHT|BUTTON_REL:
            case BUTTON_RIGHT|BUTTON_REPEAT:
                old_e=e;
                old_h=h;
                if (!pos)
                {
                    e+=inc;
                    if (e>max_euro[country])
                        e=old_e;
                }
                else
                {
                    h+=inc;
                    if (h>max_curr[country])
                        h=old_h;
                }
                break;

            case SYS_USB_CONNECTED:
                rb->usb_screen();
                euro_exit();
                return PLUGIN_USB_CONNECTED;
        }
        /*Display*/
        if (!pos)   /*Euro>home*/
            h=mul(e,currency[country]);
        else    /*Home>euro*/
            e=mydiv(h,currency[country]);
        display(e,h,pos);
    }
    euro_exit();
    return PLUGIN_OK;
}

#endif
