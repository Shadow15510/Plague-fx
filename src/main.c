/*
  Name : Plague
  Version : - dev -
  Last modification : 24 May 2021
*/

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/timer.h>
#include <gint/clock.h>

#include "core.h"
#include "display_engine.h"


// title_screen : display the title screen
static void title_screen(void);

// main_loop : display background, foreground and manage inputs
void main_loop(struct game *current_game);

// callback_timer : basic timer
int callback_tick(volatile int *tick);


int main(void)
{
    extern font_t font_plague;
    dfont(&font_plague);

    title_screen();

    struct plane plane_1 = {22, 20, 2, 84, 20, 22, 20};
    struct plane plane_2 = {34, 20, 3, 34, 44, 34, 20};
    struct plane plane_3 = {68, 44, 1, 68, 20, 68, 44};
    struct plane plane_4 = {104, 20, 3, 104, 50, 104, 20};
    struct plane plane_5 = {68, 44, 4, 34, 44, 68, 44};

    GUNUSED struct game current_game =
    {
        .contagion = 0,
        .severity = 0,
        .lethality = 0,

        .dna = 0,

        .abilities = 1, .abilities_sel = 0,
        .symptoms = 1, .symptoms_sel = 0,
        .transmissions = 1, .transmissions_sel = 0,

        .research = 0,
        .limit = 100,

        .healthy = TOTAL_POP - 1,
        .infected = 1,
        .dead = 0,

        .time = 0,

        .planes = {&plane_1, &plane_2, &plane_3, &plane_4, &plane_5, NULL}
    };

    main_loop(&current_game);

    return 1;
}


static void title_screen(void)
{
    extern bopti_image_t img_title;

    dclear(C_WHITE);
    dimage(0, 0, &img_title);
    
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
        display_foreground(background, current_game);
        dupdate();

        // Compute the motion of planes
        next_frame(current_game);
        
        // Get inputs from the keyboard and manage it
        background = get_inputs(background, &mutation_menu);

        // Special actions : quit and manage mutations
        if (background == -1) end = 1;
        if (background == 5)
        {
            manage_mutation(current_game, mutation_menu);
            background = 3;
        }
    }

    if (t >= 0) timer_stop(t);
}


int callback_tick(volatile int *tick)
{
    *tick = 1;
    return TIMER_CONTINUE;
}
