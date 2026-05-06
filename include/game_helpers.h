#pragma once

#include <stdint.h>

#include "types.h"
#include "game_internal.h"

typedef struct player_s {
    uint8_t id;
    static_string_t name;
    player_role_t role;
} player_t;

typedef struct camera_s {
    void *raw;

    mat4x4f_t proj_mat;
    mat4x4f_t view_mat;
} camera_t;

void game_helpers_init();

void game_hook_start_frame();
void game_hook_end_frame();

void game_hook_player_update(void *control);

player_t *game_get_players(int *len);
camera_t *game_get_main_camera();

vec2f_t game_world_to_screen(vec2f_t point);
