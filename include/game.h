#pragma once

#include "parse.h"

#include <stdint.h>

void player_update(void *control);

void lock_players();
void unlock_players();

uint32_t get_player_count();
player_t *get_player(uint32_t idx);
