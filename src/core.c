#include <gint/timer.h>
#include <gint/clock.h>
#include <gint/keyboard.h>

#include "core.h"
#include "mutation_engine.h"
#include "epidemic_engine.h"
#include "display_engine.h"


void next_frame(struct game *current_game)
{
    for (int i = 0; current_game->planes[i]; i++)
    {
        switch(current_game->planes[i]->direction)
        {
            case 1:
                current_game->planes[i]->y -= 1;
                break;
            case 2:
                current_game->planes[i]->x += 1;
                break;
            case 3:
                current_game->planes[i]->y += 1;
                break;
            case 4:
                current_game->planes[i]->x -= 1;
                break;
        }
        if (current_game->planes[i]->x == current_game->planes[i]->dest_x && current_game->planes[i]->y == current_game->planes[i]->dest_y)
        {
            // Set the new destination
            current_game->planes[i]->dest_x = current_game->planes[i]->depa_x;
            current_game->planes[i]->dest_y = current_game->planes[i]->depa_y;

            // Set the new departure
            current_game->planes[i]->depa_x = current_game->planes[i]->x;
            current_game->planes[i]->depa_y = current_game->planes[i]->y;

            // Set the new direction
            int new_dir = (current_game->planes[i]->direction + 2) % 4;
            if (!new_dir) new_dir = 4;
            current_game->planes[i]->direction = new_dir;
        }
    }

    current_game->time += ENGINE_TICK;
    current_game->total_time += ENGINE_TICK;

    if (current_game->time > LIMIT_TICK)
    {
        // Reset internal clock
        current_game->time = 0;

        // Update the game
        if (current_game->dna <= 100) current_game->dna += 1;
        current_game->research += current_game->priority;
        epidemic_simulation(current_game);

        // Check the end of the game
        if (current_game->research > current_game->limit)
        {
            const char *msg[5] = {"Vous avez", "perdu.", "", "", ""};
            message(msg);
            current_game->research = 0;
        }
        if (!current_game->humans[1])
        {
            if (current_game->humans[0] != 0)
            {
                const char *msg[5] = {"Vous avez", "perdu.", "", "", ""};
                message(msg);
            }
            else
            {
                const char *msg[5] = {"Vous avez", "gagne.", "", "", ""};
                message(msg);
            }
        }
    }
}


int get_inputs(const int background, int *mutation_menu)
{
    int key = rtc_key();

    if (key == KEY_OPTN && (background == 1 || background == 2)) return (background % 2) + 1;
    if (key == KEY_VARS) return 3;
    if (key == KEY_SQUARE) return 6;

    if (key == KEY_EXIT)
    {
        if (background == 5) return 3;
        if (background != 1 && background != 2) return 1;
        if (background == 1 || background == 2) return -1;
    }

    if (background == 3)
    {
        switch (key)
        {
            // Symptoms
            case KEY_F1:
                *mutation_menu = 1;
                return 5;
                break;

            // Abilities
            case KEY_F3:
                *mutation_menu = 2;
                return 5;
                break;

            // Transmission
            case KEY_F5:
                *mutation_menu = 3;
                return 5;
                break;

            // Return to the main menu
            case KEY_F6:
                *mutation_menu = 0;
                return 1;
                break;
        }
    }

    return background;
}


int rtc_key(void)
{
    int opt = GETKEY_DEFAULT & ~GETKEY_REP_ARROWS;
    int timeout = 1;
        
    key_event_t ev = getkey_opt(opt, &timeout);
    if(ev.type == KEYEV_NONE) return 0;
        
    return ev.key;
}


int callback_tick(volatile int *tick)
{
    *tick = 1;
    return TIMER_CONTINUE;
}


void message(const char *msg[5])
{
    display_message(msg);

    int key = 0, frame = 1;

    static volatile int tick = 1;
    int t = timer_configure(TIMER_ANY, DNA_ANIMATION_TICK*1000, GINT_CALL(callback_tick, &tick));
    if (t >= 0) timer_start(t);

    while (!key)
    {
        while(!tick) sleep();
        tick = 0;

        key = rtc_key();
        display_dna_animation(frame);

        frame = (frame + 1) % 24;
        if (!frame) frame = 24;
    }

    if (t >= 0) timer_stop(t);
}
