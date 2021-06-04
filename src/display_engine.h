#ifndef _PLAGUE_DISPLAY_ENGINE_H
#define _PLAGUE_DISPLAY_ENGINE_H

#include "core.h"

// display_background : display the background
void display_background(const int background);

// display_foreground : display the foreground, planes, statistics
void display_foreground(const int background, const struct game *current_game, const int mutation_menu);

// display_mutation : display the mutation selection screen
void display_mutation(const int table[4][8], const struct cursor c, const int mutation_menu);

// mutation_selected : display the mutation's informations screen
void display_mutation_buy(const struct cursor c, const int mutation_menu, const int table[4][8], const int button_selected, const struct game *current_game);

// output_error : display text and message background
void display_message(const char *msg[5]);

// display_dna_animation : display the DNA according to the frame number
void display_dna_animation(const int frame);

#endif /* _PLAGUE_DISPLAY_ENGINE_H */