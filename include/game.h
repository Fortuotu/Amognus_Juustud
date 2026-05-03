#pragma once

#include "parse.h"

#include <stdint.h>

void game_init();

void game_hook_player_update(void *control);
void game_hook_end_frame();

player_t *game_get_players(int *len);
