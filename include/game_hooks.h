#pragma once

#include <stdint.h>

#include "types.h"
#include "game_internal.h"

typedef struct player_s {
    uint8_t id;
    static_string_t name;
    player_role_t role;
} player_t;

void game_init();

void game_hook_player_update(void *control);
void game_hook_end_frame();

player_t *game_get_players(int *len);
