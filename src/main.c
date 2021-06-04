/*
  Project name ......: Plague
  Version ...........: 1.1
  Last modification .: 4 June 2021

  code and assets provided with licence :
  GNU General Public Licence v3.0
*/

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>
#include <gint/defs/types.h>
#include <gint/std/stdlib.h>

#include "core.h"
#include "display_engine.h"
#include "mutation_engine.h"
#include "save.h"

// title_screen : display the title screen
static void title_screen(void);


// main_loop : display background, foreground and manage inputs
void main_loop(struct game *current_game);


int main(void)
{
    extern font_t font_plague;
    dfont(&font_plague);

    title_screen();

    // Game statistics
    struct plane plane_1 = {22, 20, 2, 84, 20, 22, 20};
    struct plane plane_2 = {34, 20, 3, 34, 44, 34, 20};
    struct plane plane_3 = {68, 44, 1, 68, 20, 68, 44};
    struct plane plane_4 = {104, 20, 3, 104, 50, 104, 20};
    struct plane plane_5 = {68, 44, 4, 34, 44, 68, 44};


    struct game current_game =
    {
        .contagion = 0,
        .severity = 0,
        .lethality = 0,

        .dna = 0,

        .mutations_count = {0, 0, 0},
        .mutations_selected = {0, 0, 0},
        .mutations_bought = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        .research = 0,
        .limit = RESEARCH_LIMIT,
        .priority = 0,
        .humans = {0, 1, 0, 0},

        .time = 0, .total_time = 0,

        .planes = {&plane_1, &plane_2, &plane_3, &plane_4, &plane_5, NULL},

        .grid = {64, 128, NULL},
    };

    // Allocate memory
    current_game.grid.data = calloc(current_game.grid.width * current_game.grid.height, sizeof(uint8_t));

    current_game.grid.data[95 + 20 * current_game.grid.width] = 1;
    current_game.humans[0] = (current_game.grid.width * current_game.grid.height) - 1;

    read_save(&current_game);

    main_loop(&current_game);

    write_save(&current_game);

    // Free memory
    free(current_game.grid.data);

    return 1;
}


static void title_screen(void)
{
    extern bopti_image_t img_title;
    extern bopti_image_t img_explosion;

    dclear(C_BLACK);
    dupdate();
    sleep_ms(250);

    dsubimage(0, 0, &img_title, 0, 0, 128, 64, DIMAGE_NONE);    
    dupdate();
    sleep_ms(500);

    for (int frame = 0; frame < 5; frame ++)
    {
        dclear(C_BLACK);
        dsubimage(0, 0, &img_title, 0, 0, 128, 64, DIMAGE_NONE);
        dsubimage(76, 9, &img_explosion, 41 * frame, 0, 40, 40, DIMAGE_NONE);
        dupdate();
        sleep_ms(100);
    }

    dclear(C_BLACK);
    dsubimage(0, 0, &img_title, 0, 65, 128, 64, DIMAGE_NONE);
    dupdate();
    sleep_ms(500);

    for (int i = 0; i < 5; i ++)
    {
        dclear(C_BLACK);
        dsubimage(0, 0, &img_title, 0, ((i % 2) + 1) * 65, 128, 64, DIMAGE_NONE);
        dupdate();
        sleep_ms(250);
    }

    dclear(C_BLACK);
    dsubimage(0, 0, &img_title, 0, 130, 128, 64, DIMAGE_NONE);
    dupdate();

    getkey();
}


void main_loop(struct game *current_game)
{
    int background = 1, mutation_menu = 0;
    int end = 0;

    static volatile int tick = 1;
    int t = timer_configure(TIMER_ANY, ENGINE_TICK*1000, GINT_CALL(callback_tick, &tick));
    if (t >= 0) timer_start(t);

    while (!end)
    {
        // Real-time clock system
        while (!tick) sleep();
        tick = 0;

        // Update the screen
        dclear(C_WHITE);
        display_background(background);
        display_foreground(background, current_game, mutation_menu);
        dupdate();

        // Compute the motion of planes, DNA points and infectious model
        next_frame(current_game);
        
        // Get inputs from the keyboard and manage it
        background = get_inputs(background, &mutation_menu);

        // Special actions : quit and manage mutations
        if (background == -1) end = 1;
        if (background == 5)
        {
            mutation_select(current_game, mutation_menu);
            background = 3;
        }
    }

    if (t >= 0) timer_stop(t);
}
