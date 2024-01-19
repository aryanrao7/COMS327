#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>

# include "poke327.h"

class Map;

typedef int16_t pair_t[2];

class Character;

int32_t cmp_char_turns(const void *key, const void *with);
void delete_character(void *v);
void pathfind(Map *m);

int pc_move(char);

#endif
