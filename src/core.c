#include <gint/keyboard.h>

#include "core.h"
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
}


int get_inputs(const int background, int *mutation_menu)
{
    int opt = GETKEY_DEFAULT & ~GETKEY_REP_ARROWS;
    int timeout = 1;

    key_event_t ev = getkey_opt(opt, &timeout);
    if(ev.type == KEYEV_NONE) return background;

    int key = ev.key;

    if (key == KEY_OPTN && (background == 1 || background == 2)) return (background % 2) + 1;
    if (key == KEY_VARS) return 3;
    if (key == KEY_SQUARE) return 6;

    if (key == KEY_EXIT)
    {
        if (background == 5) return 3;
        if (background != 1 && background != 2) return 1;
        if (background == 1) return -1;
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


void manage_mutation(struct game *current_game, const int mutation_menu)
{
    int key = 0, end = 0;
    int cursor_x = 1, cursor_y = 1;
    while (!end)
    {
        display_mutation(current_game, mutation_menu, cursor_x, cursor_y);
        key = getkey().key;

        if (key == KEY_EXIT) end = 1;
    }
}
