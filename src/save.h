#ifndef _SAVE_H
#define _SAVE_H

#include "core.h"

// read_save : read or create a new save
void read_save(struct game *current_game);

// write_save : write the savefile
void write_save(const struct game *current_game);

#endif /* _SAVE_H */