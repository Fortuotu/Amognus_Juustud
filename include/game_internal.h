#pragma once

#include <stdint.h>

#include "types.h"
#include "utils.h"
#include "offsets.h"

typedef enum player_role_e {
    CREWMATE = 0,
    IMPOSTOR = 1,
    SCIENTIST = 2,
    ENGINEER = 3,
    GUARDIAN_ANGEL = 4,
    SHAPESHIFTER = 5,
    CREWMATE_GHOST = 6,
    IMPOSTOR_GHOST = 7,
    NOISEMAKER = 8,
    PHANTOM = 9,
    TRACKER = 10,
    DETECTIVE = 12,
    VIPER = 18
} player_role_t;

typedef struct player_control_s {
    uint8_t id;
    void *info;
    void *rb;
} player_control_t;

typedef struct player_info_s {
    player_role_t role;
} player_info_t;

typedef struct game_rb_s {
    int dummy;
} game_rb_t;

typedef struct game_camera_s {
    int dummy;
} game_camera_t;

void internal_extract_control(void *control, player_control_t *out);

void internal_extract_info(void *info, player_info_t *out);
void internal_call_info_get_player_name(void *info, void **out_sys_str);

void internal_extract_rb(void *rb, game_rb_t *out);
void internal_call_rb_get_position_injected(void *rb, vec2f_t *out_pos);

void internal_call_camera_get_projection_matrix_injected(void *camera, mat4x4f_t *out_mat);
void internal_call_camera_get_view_matrix_injected(void *camera, mat4x4f_t *out_mat);

void internal_static_call_get_main_camera(void **camera_out);

void internal_extract_sys_str(void *sys_str, static_string_t *out);

char *internal_string_for_role(player_role_t);
