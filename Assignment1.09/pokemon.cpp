#include <cstdlib>
#include <algorithm>

#include "pokemon.h"
#include "db_parse.h"

static int compare_move(const void *v1, const void *v2)
{
  return ((levelup_move *) v1)->level - ((levelup_move *) v2)->level;
}

Pokemon::Pokemon(int level) : level(level)
{
  pokemon_species_db *s;
  unsigned i, j;
  bool found;
  pokemon_species_index = rand() % ((sizeof (species) /
                                     sizeof (species[0])) - 1);
  s = species + pokemon_species_index;
  
  if (!s->levelup_moves) {
    for (s->num_levelup_moves = 0, i = 1;
         i < (sizeof (pokemon_moves) / sizeof (pokemon_moves[0]));
         i++) {
      if (s->id == pokemon_moves[i].pokemon_id &&
          pokemon_moves[i].pokemon_move_method_id == 1) {
        for (found = false, j = 0; !found && j < s->num_levelup_moves; j++) {
          if (s->levelup_moves[j].move == pokemon_moves[i].move_id) {
            found = true;
          }
        }
        if (!found) {
          s->num_levelup_moves++;
          s->levelup_moves = ((levelup_move *)
                              realloc(s->levelup_moves,
                                      (s->num_levelup_moves *
                                       sizeof (*s->levelup_moves))));
          s->levelup_moves[s->num_levelup_moves - 1].level =
            pokemon_moves[i].level;
          s->levelup_moves[s->num_levelup_moves - 1].move =
            pokemon_moves[i].move_id;
        }
      }
    }
    qsort(s->levelup_moves, s->num_levelup_moves,
          sizeof (*s->levelup_moves), compare_move);
    s->base_stat[0] = pokemon_stats[pokemon_species_index * 6 - 5].base_stat;
    s->base_stat[1] = pokemon_stats[pokemon_species_index * 6 - 4].base_stat;
    s->base_stat[2] = pokemon_stats[pokemon_species_index * 6 - 3].base_stat;
    s->base_stat[3] = pokemon_stats[pokemon_species_index * 6 - 2].base_stat;
    s->base_stat[4] = pokemon_stats[pokemon_species_index * 6 - 1].base_stat;
    s->base_stat[5] = pokemon_stats[pokemon_species_index * 6 - 0].base_stat;
  }
  for (i = 0;
       i < s->num_levelup_moves && s->levelup_moves[i].level <= level;
       i++)
    ;
  move_index[0] = move_index[1] = move_index[2] = move_index[3] = 0;
  if (i) {
    move_index[0] = s->levelup_moves[rand() % i].move;
    if (i != 1) {
      do {
        j = rand() % i;
      } while (s->levelup_moves[j].move == move_index[0]);
      move_index[1] = s->levelup_moves[j].move;
    }
  }
  for (i = 0; i < 6; i++) {
    IV[i] = rand() & 0xf;
    effective_stat[i] = 5 + ((s->base_stat[i] + IV[i]) * 2 * level) / 100;
    if (i == 0) { 
      effective_stat[i] += 5 + level;
    }
  }

  shiny = ((rand() & 0x1fff) ? false : true);
  gender = ((rand() & 0x1fff) ? gender_female : gender_male);
  cur_hp = effective_stat[stat_hp];
}

const char *Pokemon::get_species() const
{
  return species[pokemon_species_index].identifier;
}

int Pokemon::get_hp() const
{
  return effective_stat[stat_hp];
}

int Pokemon::get_atk() const
{
  return effective_stat[stat_atk];
}

int Pokemon::get_def() const
{
  return effective_stat[stat_def];
}

int Pokemon::get_spatk() const
{
  return effective_stat[stat_spatk];
}

int Pokemon::get_spdef() const
{
  return effective_stat[stat_spdef];
}

int Pokemon::get_speed() const
{
  return effective_stat[stat_speed];
}

int Pokemon::get_move_id(int i)
{
  if(i < 4){
    return move_index[i];
  }
  return -1;
}

int Pokemon::get_level()
{
  return level;
}

int Pokemon::get_species_id()
{
  return pokemon_species_index;
}

const char *Pokemon::get_gender_string() const
{
  return gender == gender_female ? "female" : "male";
}

bool Pokemon::is_shiny() const
{
  return shiny;
}

const char *Pokemon::get_move(int i) const
{
  if (i < 4 && move_index[i]) {
    return moves[move_index[i]].identifier;
  } else {
    return "";
  }
}

std::ostream &Pokemon::print(std::ostream &o) const
{
  pokemon_species_db *s = species + pokemon_species_index;
  unsigned i;

  o << get_species() << " level:" << level << " "
    << get_gender_string() << " " << (shiny ? "shiny" : "not shiny")
    << std::endl;
  o << "         HP:" << effective_stat[stat_hp] << std::endl
    << "        ATK:" << effective_stat[stat_atk] << std::endl
    << "        DEF:" << effective_stat[stat_def] << std::endl
    << "      SPATK:" << effective_stat[stat_spatk] << std::endl
    << "      SPDEF:" << effective_stat[stat_spdef] << std::endl
    << "      SPEED:" << effective_stat[stat_speed] << std::endl;
  o << "       HPIV:" << IV[stat_hp] << std::endl
    << "      ATKIV:" << IV[stat_atk] << std::endl
    << "      DEFIV:" << IV[stat_def] << std::endl
    << "    SPATKIV:" << IV[stat_spatk] << std::endl
    << "    SPDEFIV:" << IV[stat_spdef] << std::endl
    << "    SPEEDIV:" << IV[stat_speed] << std::endl;
  o << "     HPBASE:" << s->base_stat[stat_hp] << std::endl
    << "    ATKBASE:" << s->base_stat[stat_atk] << std::endl
    << "    DEFBASE:" << s->base_stat[stat_def] << std::endl
    << "  SPATKBASE:" << s->base_stat[stat_spatk] << std::endl
    << "  SPDEFBASE:" << s->base_stat[stat_spdef] << std::endl
    << "  SPEEDBASE:" << s->base_stat[stat_speed] << std::endl;

  o << "  Levelup moves: " << std::endl; 
  for (i = 0; i < s->num_levelup_moves; i++) {
    o << "    " << moves[s->levelup_moves[i].move].identifier
      << ":" << s->levelup_moves[i].level << std::endl;
  }
  o << "  Known moves: " << std::endl;
  if (move_index[0]) {
    o << "    " << moves[move_index[0]].identifier << std::endl;
  }
  if (move_index[1]) {
    o << "    " << moves[move_index[1]].identifier << std::endl;
  }
  if (move_index[2]) {
    o << "    " << moves[move_index[2]].identifier << std::endl;
  }
  if (move_index[3]) {
    o << "    " << moves[move_index[3]].identifier << std::endl;
  }

  return o;
}

std::ostream &operator<<(std::ostream &o, const Pokemon &p)
{
  return p.print(o);
}