#include <gint/display.h>

#include "core.h"
#include "display_engine.h"


void display_background(const int background)
{
    extern const bopti_image_t img_bground;
    dsubimage(0, 0, &img_bground, 0, 65 * (background - 1), 128, 64, DIMAGE_NONE);
}


void display_foreground(const int background, const int mutation_menu, const struct game *current_game)
{
    extern const bopti_image_t img_mutations;
    extern const bopti_image_t img_planes;
    int length;

    switch (background)
    {
        case 1:
            for (int i = 0; current_game->planes[i]; i++)
            {
                dsubimage(current_game->planes[i]->x - 4, current_game->planes[i]->y - 4, &img_planes, 0, 8 * (current_game->planes[i]->direction - 1), 8, 8, DIMAGE_NONE);
            }
            break;
        case 2:
            for (int i = 0; current_game->planes[i]; i++)
            {
                if (current_game->planes[i]->y + 8 < 53) dsubimage(current_game->planes[i]->x - 4, current_game->planes[i]->y - 4, &img_planes, 0, 8 * (current_game->planes[i]->direction - 1), 8, 8, DIMAGE_NONE);
            }

            int length = 74 * current_game->research / current_game->limit;
            dprint(9, 58, C_BLACK, "%d", current_game->dna);

            dline(51, 60, 51 + length, 60, C_BLACK);
            dline(51, 59, 51 + length, 59, C_BLACK);
            break;

        case 3:
            dprint(102, 37, C_BLACK, "%d", current_game->dna);
            
            length = 67 * current_game->contagion / 26;
            dline(57, 48, 57 + length, 48, C_BLACK);
            dline(57, 49, 57 + length, 49, C_BLACK);
            
            length = 67 * current_game->severity / 20;
            dline(57, 54, 57 + length, 54, C_BLACK);
            dline(57, 55, 57 + length, 55, C_BLACK);
            
            length = 67 * current_game->lethality / 33;
            dline(57, 60, 57 + length, 60, C_BLACK);
            dline(57, 61, 57 + length, 61, C_BLACK);

            if (current_game->symptoms_sel) dsubimage(5, 15, &img_mutations, 0, 16 * (current_game->symptoms_sel - 1), 15, 15, 0);
            if (current_game->symptoms_sel) dsubimage(35, 15, &img_mutations, 16, 16 * (current_game->abilities_sel - 1), 15, 15, 0);
            if (current_game->symptoms_sel) dsubimage(65, 15, &img_mutations, 32, 16 * (current_game->transmission_sel - 1), 15, 15, 0);
            break;

        case 6:
            length = 63 *  current_game->healthy/ TOTAL_POP;
            dline(61, 31, 61 + length, 31, C_BLACK);
            dline(61, 32, 61 + length, 32, C_BLACK);
            
            length = 63 * current_game->infected / TOTAL_POP;
            dline(61, 39, 61 + length, 39, C_BLACK);
            dline(61, 40, 61 + length, 40, C_BLACK);
            
            length = 63 * current_game->dead / TOTAL_POP;
            dline(61, 47, 61 + length, 47, C_BLACK);
            dline(61, 48, 61 + length, 48, C_BLACK);
            
            length = 63 * current_game->cured / TOTAL_POP;
            dline(61, 55, 61 + length, 55, C_BLACK);
            dline(61, 56, 61 + length, 56, C_BLACK);
            break;
    }
}