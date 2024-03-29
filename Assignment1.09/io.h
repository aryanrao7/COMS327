#ifndef IO_H
# define IO_H

class Character;
typedef int16_t pair_t[2];

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(void);
void io_handle_input(pair_t dest);
void io_queue_message(const char *format, ...);
void io_battle(Character *enemy);
void io_encounter_pokemon(void);
int io_enter_bag(bool in_wild_battle);

#endif