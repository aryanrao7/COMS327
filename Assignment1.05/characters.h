#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>

#include "pokemon.h"

typedef enum __attribute__ ((__packed__)) movement_type {
  move_hiker,
  move_rival,
  move_pace,
  move_wander,
  move_sentry,
  move_explore,
  move_swim,
  move_pc,
  num_movement_types
} movement_type_t;

typedef enum __attribute__ ((__packed__)) character_type {
  char_pc,
  char_hiker,
  char_rival,
  char_swimmer,
  char_other,
  num_character_types
} character_type_t;

extern const char *char_type_name[num_character_types];

extern int32_t move_cost[num_character_types][num_terrain_types];

typedef struct npc {
  character_type_t ctype;
  movement_type_t mtype;
  int defeated;
  pair_t dir;
} npc_t;

typedef struct pc {
} pc_t;

int32_t cmp_char_turns(const void *key, const void *with);
void delete_character(void *v);
void pathfind(map_t *m);

extern void (*move_func[num_movement_types])(character_t *, pair_t);

int pc_move(char);

#endif
