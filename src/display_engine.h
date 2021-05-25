#ifndef _PLAGUE_DISPLAY_ENGINE_H
#define _PLAGUE_DISPLAY_ENGINE_H

// display_background : display the background
void display_background(const int background);

// display_foreground : display the foreground, planes, statistics
void display_foreground(const int background, const struct game *current_game);

// display_mutation : display the mutation selection screen
void display_mutation(const struct game *current_game, const int mutation_menu, const int cursor_x, const int cursor_y);

// init_mat : copy src into dest
void init_mat(int x, int y, int dest[][x], int src[][x]);

#endif /* _PLAGUE_DISPLAY_ENGINE_H */