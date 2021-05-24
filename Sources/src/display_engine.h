#ifndef _PLAGUE_DISPLAY_ENGINE_H
#define _PLAGUE_DISPLAY_ENGINE_H

// Display the background
void display_background(const int background);

// Display the foreground
void display_foreground(const int background, const int mutation_menu, const struct game *current_game);

#endif /* _PLAGUE_DISPLAY_ENGINE_H */