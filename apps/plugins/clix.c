/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2008-2009 Rene Peinthor
 * Contribution from Johannes Schwarz (new menu system, use of highscore lib)
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "plugin.h"
#include "lib/highscore.h"
#include "lib/playback_control.h"
#include "lib/display_text.h"

PLUGIN_HEADER

#if (CONFIG_KEYPAD == SANSA_E200_PAD)
#define CLIX_BUTTON_QUIT        BUTTON_POWER
#define CLIX_BUTTON_UP          BUTTON_UP
#define CLIX_BUTTON_DOWN        BUTTON_DOWN
#define CLIX_BUTTON_SCROLL_FWD  BUTTON_SCROLL_FWD
#define CLIX_BUTTON_SCROLL_BACK BUTTON_SCROLL_BACK
#define CLIX_BUTTON_LEFT        BUTTON_LEFT
#define CLIX_BUTTON_RIGHT       BUTTON_RIGHT
#define CLIX_BUTTON_CLICK       BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_CLIP_PAD)
#define CLIX_BUTTON_QUIT        BUTTON_POWER
#define CLIX_BUTTON_UP          BUTTON_UP
#define CLIX_BUTTON_DOWN        BUTTON_DOWN
#define CLIX_BUTTON_LEFT        BUTTON_LEFT
#define CLIX_BUTTON_RIGHT       BUTTON_RIGHT
#define CLIX_BUTTON_CLICK       BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_FUZE_PAD)
#define CLIX_BUTTON_QUIT        BUTTON_HOME
#define CLIX_BUTTON_UP          BUTTON_UP
#define CLIX_BUTTON_DOWN        BUTTON_DOWN
#define CLIX_BUTTON_SCROLL_FWD  BUTTON_SCROLL_FWD
#define CLIX_BUTTON_SCROLL_BACK BUTTON_SCROLL_BACK
#define CLIX_BUTTON_LEFT        BUTTON_LEFT
#define CLIX_BUTTON_RIGHT       BUTTON_RIGHT
#define CLIX_BUTTON_CLICK       BUTTON_SELECT

#elif (CONFIG_KEYPAD == SANSA_C200_PAD)
#define CLIX_BUTTON_QUIT          BUTTON_POWER
#define CLIX_BUTTON_UP            BUTTON_UP
#define CLIX_BUTTON_DOWN          BUTTON_DOWN
#define CLIX_BUTTON_SCROLL_FWD    BUTTON_VOL_UP
#define CLIX_BUTTON_SCROLL_BACK   BUTTON_VOL_DOWN
#define CLIX_BUTTON_LEFT          BUTTON_LEFT
#define CLIX_BUTTON_RIGHT         BUTTON_RIGHT
#define CLIX_BUTTON_CLICK         BUTTON_SELECT

#elif (CONFIG_KEYPAD == IPOD_4G_PAD) || \
      (CONFIG_KEYPAD == IPOD_3G_PAD) || \
      (CONFIG_KEYPAD == IPOD_1G2G_PAD)
#define CLIX_BUTTON_QUIT       BUTTON_MENU
#define CLIX_BUTTON_UP         BUTTON_SCROLL_BACK
#define CLIX_BUTTON_DOWN       BUTTON_SCROLL_FWD
#define CLIX_BUTTON_CLICK      BUTTON_SELECT
#define CLIX_BUTTON_RIGHT      BUTTON_RIGHT
#define CLIX_BUTTON_LEFT       BUTTON_LEFT

#elif (CONFIG_KEYPAD == GIGABEAT_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_POWER
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif (CONFIG_KEYPAD == GIGABEAT_S_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_BACK
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif (CONFIG_KEYPAD == IRIVER_H10_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_POWER
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_PLAY
#define CLIX_BUTTON_UP      BUTTON_SCROLL_UP
#define CLIX_BUTTON_DOWN    BUTTON_SCROLL_DOWN

#elif CONFIG_KEYPAD == IAUDIO67_PAD
#define CLIX_BUTTON_QUIT    BUTTON_POWER
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_PLAY
#define CLIX_BUTTON_UP      BUTTON_STOP
#define CLIX_BUTTON_DOWN    BUTTON_PLAY

#elif CONFIG_KEYPAD == IAUDIO_X5M5_PAD
#define CLIX_BUTTON_QUIT    BUTTON_POWER
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif (CONFIG_KEYPAD == IRIVER_H300_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_OFF
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif CONFIG_KEYPAD == CREATIVEZVM_PAD
#define CLIX_BUTTON_QUIT    BUTTON_BACK
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif (CONFIG_KEYPAD == PHILIPS_HDD1630_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_POWER
#define CLIX_BUTTON_LEFT    BUTTON_LEFT
#define CLIX_BUTTON_RIGHT   BUTTON_RIGHT
#define CLIX_BUTTON_CLICK   BUTTON_SELECT
#define CLIX_BUTTON_UP      BUTTON_UP
#define CLIX_BUTTON_DOWN    BUTTON_DOWN

#elif CONFIG_KEYPAD == COWOND2_PAD
#define CLIX_BUTTON_QUIT    BUTTON_POWER

#elif (CONFIG_KEYPAD == ONDAVX747_PAD) || \
      (CONFIG_KEYPAD == MROBE500_PAD)
#define CLIX_BUTTON_QUIT    BUTTON_POWER

#else
#error "no keymap"
#endif

#ifdef HAVE_TOUCHSCREEN
#ifndef CLIX_BUTTON_LEFT
#define CLIX_BUTTON_LEFT    BUTTON_MIDLEFT
#endif
#ifndef CLIX_BUTTON_RIGHT
#define CLIX_BUTTON_RIGHT   BUTTON_MIDRIGHT
#endif
#ifndef CLIX_BUTTON_CLICK
#define CLIX_BUTTON_CLICK  BUTTON_CENTER
#endif
#ifndef CLIX_BUTTON_UP
#define CLIX_BUTTON_UP      BUTTON_TOPMIDDLE
#endif
#ifndef CLIX_BUTTON_DOWN
#define CLIX_BUTTON_DOWN    BUTTON_BOTTOMMIDDLE
#endif
#endif

#define HIGHSCORE_FILE       PLUGIN_GAMES_DIR "/clix.score"
#define NUM_SCORES 5
struct highscore highest[NUM_SCORES];

#define NUM_LEVELS 9
#define BLINK_TICKCOUNT 25
#define MARGIN 5

#if (LCD_WIDTH >= LCD_HEIGHT)
#define BOARD_WIDTH  18
#define BOARD_HEIGHT 12
#else
#define BOARD_WIDTH  12
#define BOARD_HEIGHT 18
#endif

#if (LCD_WIDTH >= 306 && LCD_HEIGHT>= 204)
#define CELL_SIZE 16

#elif (LCD_WIDTH >= 270 && LCD_HEIGHT>= 180)
#define CELL_SIZE 14

#elif (LCD_WIDTH >= 234 && LCD_HEIGHT>= 156)
#define CELL_SIZE 12

#elif (LCD_WIDTH >= 198 && LCD_HEIGHT>= 132)
#define CELL_SIZE 10

#elif (LCD_WIDTH >= 162 && LCD_HEIGHT>= 108)
#define CELL_SIZE 8

#elif (LCD_WIDTH >= 126 && LCD_HEIGHT>= 84)
#define CELL_SIZE 6

#elif (LCD_WIDTH >= 60)
#define CELL_SIZE 4
#endif

#define XYPOS(x,y) ((y) * BOARD_WIDTH + x)
#define XOFS LCD_WIDTH/2-(BOARD_WIDTH * (CELL_SIZE + 1)/2)
#define YOFS (LCD_HEIGHT+10)/2-(BOARD_HEIGHT * (CELL_SIZE + 1)/2)


struct clix_game_state_t {
    unsigned char level; /* current level */
    char x,y; /* current positions of the cursor */
    int board[BOARD_WIDTH * BOARD_HEIGHT]; /* play board*/
    /* state of selected fields,maybe we can store this in the play board too */
    bool board_selected[ BOARD_WIDTH * BOARD_HEIGHT];
    char selected_count;
    unsigned short score; /* current game score */
    char status;
    bool blink; /* true if selected CELLS are currently white */
};

/* game state enum */
enum {
    CLIX_GAMEOVER = -1,
    CLIX_CONTINUE,
    CLIX_CLEARED
};

/* cell color enum */
enum {
    CC_BLACK = -1,
    CC_BLUE,
    CC_GREEN,
    CC_RED,
    CC_YELLOW,
    CC_ORANGE,
    CC_CYAN,
    CC_BROWN,
    CC_PINK,
    CC_DARK_BLUE,
    CC_DARK_GREEN
};

/* display the highscore list and highlight the last one */
static void clix_show_highscores(int position)
{
    int i, w, h;
    char str[30];

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_set_foreground(LCD_WHITE);
#endif
    rb->button_clear_queue();
    rb->lcd_clear_display();

    rb->lcd_setfont(FONT_UI);
    rb->lcd_getstringsize("High Scores", &w, &h);
    /* check wether it fits on screen */
    if ((4*h + h*(NUM_SCORES-1) + MARGIN) > LCD_HEIGHT) {
        rb->lcd_setfont(FONT_SYSFIXED);
        rb->lcd_getstringsize("High Scores", &w, &h);
    }
    rb->lcd_putsxy(LCD_WIDTH/2-w/2, MARGIN, "High Scores");
    rb->lcd_putsxy(LCD_WIDTH/4-w/4,2*h, "Score");
    rb->lcd_putsxy(LCD_WIDTH*3/4-w/4,2*h, "Level");

    for (i = 0; i<NUM_SCORES; i++)
    {
#ifdef HAVE_LCD_COLOR
        if (i == position) {
            rb->lcd_set_foreground(LCD_RGBPACK(245,0,0));
        }
#endif
        rb->snprintf (str, sizeof (str), "%d)", i+1);
        rb->lcd_putsxy (MARGIN,3*h + h*i, str);
        rb->snprintf (str, sizeof (str), "%d", highest[i].score);
        rb->lcd_putsxy (LCD_WIDTH/4-w/4,3*h + h*i, str);
        rb->snprintf (str, sizeof (str), "%d", highest[i].level);
        rb->lcd_putsxy (LCD_WIDTH*3/4-w/4,3*h + h*i, str);
        if(i == position) {
#ifdef HAVE_LCD_COLOR
            rb->lcd_set_foreground(LCD_WHITE);
#else
            rb->lcd_hline(MARGIN, LCD_WIDTH-MARGIN, 3*h + h*(i+1));
#endif
        }
    }
    rb->lcd_update();
    rb->button_get(true);
    rb->lcd_setfont(FONT_SYSFIXED);
}

/*  recursiv function to check if a neighbour cell is of the same color
    if so call the function with the neighbours position
*/
static void clix_set_selected(struct clix_game_state_t* state,
                                const int x, const int y)
{
    state->selected_count++;
    state->board_selected[ XYPOS( x, y)] = true;
    int current_color = state->board[ XYPOS( x, y)];

    if( (x - 1) >= 0 &&
        state->board[ XYPOS( x - 1, y)] == current_color &&
        state->board_selected[ XYPOS(x - 1, y)] == false)
        clix_set_selected( state, x - 1, y);

    if( (y + 1) < BOARD_HEIGHT &&
        state->board[ XYPOS( x, y + 1)] == current_color &&
        state->board_selected[ XYPOS(x, y + 1)] == false)
        clix_set_selected( state, x, y + 1);

    if( (x + 1) < BOARD_WIDTH &&
        state->board[ XYPOS( x + 1, y)] == current_color &&
        state->board_selected[ XYPOS(x + 1, y)] == false)
        clix_set_selected( state, x + 1, y);

    if( (y - 1) >= 0 &&
        state->board[ XYPOS( x, y - 1)] == current_color &&
        state->board_selected[ XYPOS(x, y - 1)] == false)
        clix_set_selected( state, x, y - 1);
}

/* updates "blinking" cells by finding out which one is a valid neighbours */
static void clix_update_selected(struct clix_game_state_t* state)
{
    int i;

    for( i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; ++i)
    {
        state->board_selected[i] = false;
    }
    state->selected_count = 0;

    /* recursion starts here */
    clix_set_selected( state, state->x, state->y);
}

/* inits the board with new random colors according to the level */
static void clix_init_new_level(struct clix_game_state_t* state)
{
    int i;
    int r;

    state->y = BOARD_HEIGHT / 2;
    state->x = BOARD_WIDTH / 2;
    
    rb->srand( *rb->current_tick);
    /* create a random colored board, according to the current level */
    for(i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; ++i)
    {
        r = rb->rand() % (state->level + 1);
        state->board[i] = r;
    }
}

/* this inits the game state structure */
static void clix_init(struct clix_game_state_t* state)
{
    state->level = 1;
    state->score = 0;
    state->blink = false;
    state->status = CLIX_CONTINUE;

    clix_init_new_level(state);

    clix_update_selected(state);
}

/* Function for drawing a cell */
static void clix_draw_cell(struct clix_game_state_t* state, const int x, const int y)
{
    int realx = XOFS;
    int realy = YOFS;

    realx += x * (CELL_SIZE + 1);
    realy += y * (CELL_SIZE + 1);

    if (state->blink && state->board_selected[ XYPOS( x, y)]) {
        rb->lcd_set_foreground(LCD_WHITE);
    } else {
        switch (state->board[ XYPOS( x, y)])
        {
            case CC_BLUE:
                rb->lcd_set_foreground( LCD_RGBPACK( 25, 25, 255));
            break;
            case CC_GREEN:
                rb->lcd_set_foreground( LCD_RGBPACK( 25, 255, 25));
            break;
            case CC_RED:
                rb->lcd_set_foreground( LCD_RGBPACK( 255, 25, 25));
            break;
            case CC_YELLOW:
                rb->lcd_set_foreground( LCD_RGBPACK( 225, 225, 25));
            break;
            case CC_ORANGE:
                rb->lcd_set_foreground( LCD_RGBPACK( 230, 140, 15));
            break;
            case CC_CYAN:
                rb->lcd_set_foreground( LCD_RGBPACK( 25, 245, 230));
            break;
            case CC_BROWN:
                rb->lcd_set_foreground( LCD_RGBPACK(139, 69, 19));
            break;
            case CC_PINK:
                rb->lcd_set_foreground( LCD_RGBPACK(255, 105, 180));
            break;
            case CC_DARK_GREEN:
                rb->lcd_set_foreground( LCD_RGBPACK( 0, 100, 0));
            break;
            case CC_DARK_BLUE:
                rb->lcd_set_foreground( LCD_RGBPACK( 280, 32, 144));
            break;
            default:
                rb->lcd_set_foreground( LCD_BLACK);
            break;
        }
    }

    rb->lcd_fillrect( realx, realy, CELL_SIZE, CELL_SIZE);

    /* draw cursor */
    if ( x == state->x && y == state->y) {
        rb->lcd_set_foreground( LCD_WHITE);
        rb->lcd_drawrect( realx - 1, realy - 1, CELL_SIZE + 2, CELL_SIZE + 2);
    }
}

/* main function of drawing the whole board and score... */
static void clix_draw(struct clix_game_state_t* state)
{
    int i,j;
    char str[30];

    /* Clear screen */
    rb->lcd_clear_display();
    rb->lcd_set_foreground( LCD_WHITE);
    
    rb->lcd_putsxy( MARGIN, MARGIN, "Score:");
    rb->snprintf( str, sizeof(str), "%d", state->score);
    rb->lcd_putsxy( 43, MARGIN, str);
#if LCD_WIDTH <= 100
    rb->lcd_putsxy( 75, MARGIN, "L:");
    rb->snprintf( str, sizeof(str), "%d", state->level);
    rb->lcd_putsxy( 90, MARGIN, str);
#else
    rb->lcd_putsxy( 75, MARGIN, "Level:");
    rb->snprintf( str, sizeof(str), "%d", state->level);
    rb->lcd_putsxy( 113, MARGIN, str);
#endif
    for( i = 0; i < BOARD_WIDTH; ++i)
    {
        for( j = 0; j < BOARD_HEIGHT; ++j)
        {
            clix_draw_cell( state, i, j);
        }
    }

    rb->lcd_update();
}

static void clix_move_cursor(struct clix_game_state_t* state, const bool left)
{
    signed char x, y;

    x = state->x;
    do
    {
        y = state->y;
        while(state->board[ XYPOS( x, y)] == CC_BLACK && y < BOARD_HEIGHT) y++;
        if (y < BOARD_HEIGHT) {
            state->y = y;
            state->x = x;
        }
        else
        {
            if (left) {
                if( x >= 0)
                    x--;
                else
                    y = state->y;
            }
            else
            {
                if( x < BOARD_WIDTH - 1)
                    x++;
                else
                    x = 0;
            }
        }
    } while ( y != state->y);

}

/* returns the color of the given position, if out of bounds return CC_BLACK */
static int clix_get_color(struct clix_game_state_t* state, const int x, const int y)
{
    if( x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
        return state->board[XYPOS( x, y)];
    else
        return CC_BLACK;
}

static int clix_clear_selected(struct clix_game_state_t* state)
{
    int i, j, x, y;

    state->status = CLIX_CLEARED;

    /* clear the selected blocks */
    for( i = 0; i < BOARD_WIDTH; ++i)
    {
        for( j = 0; j < BOARD_HEIGHT; ++j)
        {
            if( state->board_selected[ XYPOS( i, j)] )
            {
                state->board_selected[ XYPOS( i, j)] = false;
                state->board[ XYPOS( i, j)] = CC_BLACK;
            }
        }
    }

    /* let blocks falling down */
    for( i = BOARD_WIDTH - 1; i >= 0; --i)
    {
        for( j = BOARD_HEIGHT - 1; j >= 0; --j)
        {
            y = j;
            while( state->board[ XYPOS( i, y + 1)] == CC_BLACK &&
                   y < BOARD_HEIGHT
                 )
                    y++;

            if (y != j) {
                state->board[ XYPOS(i, y)] = state->board[ XYPOS( i, j)];
                state->board[ XYPOS( i, j)] = CC_BLACK;
            }
        }
    }

    /* count score */
    state->score += state->selected_count * state->level;

    /* check every column (from right to left) if its empty,
       if so copy the contents from the right side */
    for( i = BOARD_WIDTH - 1; i >= 0; --i)
    {
        if (state->board[ XYPOS( i, BOARD_HEIGHT - 1)] == CC_BLACK) {
            if( (i + 1) < BOARD_WIDTH &&
                state->board[ XYPOS( i + 1, BOARD_HEIGHT - 1)] != CC_BLACK)
            {
                for( x = (i + 1); x < BOARD_WIDTH; ++x)
                {
                    for( j = 0; j < BOARD_HEIGHT; ++j)
                    {
                        state->board[ XYPOS( x - 1, j)] =
                            state->board[ XYPOS( x, j)];

                        state->board[ XYPOS( x, j)] = CC_BLACK;
                    }
                }
            }
        }
        else
            state->status = CLIX_CONTINUE;
    }

    if (state->status != CLIX_CLEARED) {
        /*  check if a move is still possible, otherwise the game is over.
            tart from the left bottom, because there are the last fields
            at the end of the game.
        */
        for( i = 0; i < BOARD_WIDTH; ++i)
        {
            for( j = BOARD_HEIGHT - 1; j >= 0; --j)
            {
                if (state->board[ XYPOS( i, j)] != CC_BLACK) {
                    if ( state->board[ XYPOS( i, j)] ==
                            clix_get_color( state, i - 1, j) ||
                        state->board[ XYPOS( i, j)] ==
                            clix_get_color( state, i + 1, j) ||
                        state->board[ XYPOS( i, j)] ==
                            clix_get_color( state, i, j - 1) ||
                        state->board[ XYPOS( i, j)] ==
                            clix_get_color( state, i, j + 1)
                      )
                    {
                        /* and the loop, but in a diffrent way than usually*/
                        i = BOARD_WIDTH + 1;
                        j = -2;
                    }
                }
            }
        }
        /* if the loops ended without a possible move, the game is over */
        if( i == BOARD_WIDTH && j == -1)
            state->status = CLIX_GAMEOVER;

        /* set cursor to the right position */
        if (state->status == CLIX_CONTINUE) {
            clix_move_cursor( state, true);
            clix_update_selected( state);
        }
    }

    return state->status;
}

static int clix_help(void)
{
    rb->lcd_setfont(FONT_UI);
    rb->lcd_set_foreground(LCD_WHITE);
#define WORDS (sizeof help_text / sizeof (char*))
	char *help_text[] = {
		"Clix", "", "Aim", "", "Remove", "all", "blocks", "from", "the",
        "board", "to", "achieve", "the", "next", "level.", "You", "can",
        "only", "remove", "blocks,", "if", "at", "least", "two", "blocks",
        "with", "the", "same", "color", "have", "a", "direct", "connection.",
        "The", "more", "blocks", "you", "remove", "per", "turn,", "the",
        "more", "points", "you", "get."
    };
    static struct style_text formation[]={
        { 0, TEXT_CENTER|TEXT_UNDERLINE },
        { 2, C_RED }
    };
    
    if (display_text(WORDS, help_text, formation, NULL)==PLUGIN_USB_CONNECTED)
        return PLUGIN_USB_CONNECTED;
    int button;
    do {
        button = rb->button_get(true);
        if (button == SYS_USB_CONNECTED) {
            return PLUGIN_USB_CONNECTED;
        }
    } while( ( button == BUTTON_NONE )
        || ( button & (BUTTON_REL|BUTTON_REPEAT) ) );
    rb->lcd_setfont(FONT_SYSFIXED);
    return 0;
}

static bool _ingame;
static int clix_menu_cb(int action, const struct menu_item_ex *this_item)
{
    if(action == ACTION_REQUEST_MENUITEM
       && !_ingame && ((intptr_t)this_item)==0)
        return ACTION_EXIT_MENUITEM;
    return action;
}

static int clix_menu(struct clix_game_state_t* state, bool ingame)
{
    rb->button_clear_queue();
    int choice = 0;

    _ingame = ingame;
    
    MENUITEM_STRINGLIST (main_menu, "Clix Menu", clix_menu_cb,
                             "Resume Game",
                             "Start New Game",
                             "Help",
                             "High Score",
                             "Playback Control",
                             "Quit");

    while (true) {
        choice = rb->do_menu(&main_menu, &choice, NULL, false);
        switch (choice) {
            case 0:
                return 0;
            case 1:
                clix_init(state);
                return 0;
            case 2:
                if (clix_help()==PLUGIN_USB_CONNECTED)
                    return 1;
                break;
            case 3:
                clix_show_highscores(NUM_SCORES);
                break;
            case 4:
                playback_control(NULL);
                break;
            case 5:
            case MENU_ATTACHED_USB:
                return 1;
            default:
                break;
        }
    }
}

static int clix_handle_game(struct clix_game_state_t* state)
{
    if (clix_menu(state, 0))
        return 1;

    int button;
    int blink_tick = *rb->current_tick + BLINK_TICKCOUNT;
    int position;

    int time;
    int start;
    int end;
    int oldx, oldy;

    while(true)
    {
        if (blink_tick < *rb->current_tick) {
            state->blink = state->blink ? false : true;
            blink_tick = *rb->current_tick + BLINK_TICKCOUNT;
        }

        time = 6; /* number of ticks this function will loop reading keys */
        start = *rb->current_tick;
        end = start + time;
        while(end > *rb->current_tick)
        {
            oldx = state->x;
            oldy = state->y;

            rb->button_get_w_tmo(end - *rb->current_tick);
            button = rb->button_status();
            rb->button_clear_queue();
            switch( button)
            {
#ifdef CLIX_BUTTON_SCROLL_BACK
                case CLIX_BUTTON_SCROLL_BACK:
#endif
                case CLIX_BUTTON_UP:
                    if( state->y == 0 ||
                        state->board[ XYPOS( state->x, state->y - 1)] ==
                            CC_BLACK
                      )
                        state->y = BOARD_HEIGHT - 1;
                    else
                        state->y--;

                    clix_move_cursor(state, true);
                break;
                case CLIX_BUTTON_RIGHT:
                    if( state->x == (BOARD_WIDTH - 1))
                        state->x = 0;
                    else
                        state->x++;

                    clix_move_cursor(state, false);
                break;
#ifdef CLIX_BUTTON_SCROLL_FWD
                case CLIX_BUTTON_SCROLL_FWD:
#endif
                case CLIX_BUTTON_DOWN:
                    if( state->y == (BOARD_HEIGHT - 1))
                        state->y = 0;
                    else
                        state->y++;

                    clix_move_cursor( state, true);
                break;
                case CLIX_BUTTON_LEFT:
                    if( state->x == 0)
                        state->x = BOARD_WIDTH - 1;
                    else
                        state->x--;

                    clix_move_cursor(state, true);

                break;
                case CLIX_BUTTON_CLICK:
                {
                    if (state->selected_count > 1) {
                        switch( clix_clear_selected( state))
                        {
                            case CLIX_CLEARED:
                                clix_draw( state);
                                if (state->level < NUM_LEVELS) {
                                    rb->splash(HZ*2, "Great! Next Level!");
                                    state->score += state->level * 100;
                                    state->level++;
                                    clix_init_new_level( state);
                                    clix_update_selected( state);
                                }
                                else {
                                    rb->splash(HZ*2, "Congratulation!!!");
                                    rb->lcd_clear_display();
                                    rb->splash(HZ*2, "You have finished the game.");
                                    if (clix_menu(state, 0))
                                        return 1;
                                }
                            break;
                            case CLIX_GAMEOVER:
                                clix_draw( state);
                                rb->splash(HZ*2, "Game Over!");
                                rb->lcd_clear_display();
                                if (highscore_would_update(state->score,
                                    highest, NUM_SCORES)) {
                                    position=highscore_update(state->score,
                                                            state->level, "",
                                                            highest,NUM_SCORES);            
                                    if (position == 0) {
                                        rb->splash(HZ*2, "New High Score");
                                    }
                                    clix_show_highscores(position);
                                }
                                if (clix_menu(state, 0))
                                    return 1;
                            break;
                            default:
                                rb->sleep(10);  /* prevent repeating clicks */
                            break;
                        }
                    }
                }
                break;
                case CLIX_BUTTON_QUIT:
                    if (clix_menu(state, 1) != 0) {
                        rb->button_clear_queue();
                        return 1;
                    }
                break;
                default:

                break;
            }

            if( (oldx != state->x || oldy != state->y) &&
                state->board_selected[ XYPOS( oldx, oldy)] !=
                state->board_selected[ XYPOS( state->x, state->y)]
              )
            {
                clix_update_selected(state);
            }
            clix_draw(state);
            rb->sleep(time);
        }
    }
}

/* this is the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    (void)parameter;

#ifdef HAVE_LCD_COLOR
    rb->lcd_set_backdrop(NULL);
    rb->lcd_set_foreground(LCD_WHITE);
    rb->lcd_set_background(LCD_BLACK);
    rb->lcd_setfont(FONT_SYSFIXED);

    highscore_load(HIGHSCORE_FILE, highest, NUM_SCORES);

    struct clix_game_state_t state;
    clix_handle_game( &state);

    highscore_save(HIGHSCORE_FILE, highest, NUM_SCORES);
    
    rb->lcd_set_foreground(LCD_WHITE);
    rb->lcd_setfont(FONT_UI);
#endif

    return PLUGIN_OK;
}