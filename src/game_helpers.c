#include "game_helpers.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "glad/glad.h"
#include "io.h"

#define MAX_PLAYERS 32
#define MAX_TIMESTAMP_DIFF 5

typedef struct player_list_s {
    int len;
    int frame_count;
    player_t player[MAX_PLAYERS];
    uint32_t timestamp[MAX_PLAYERS];
} player_list_t;

static player_list_t list = { 0 };
static camera_t main_camera = { 0 };

static void parse_player(void *control, player_t *player) {
    player_control_t parsed_control = { 0 };
    player_info_t parsed_info = { 0 };

    void *player_name_sys_str = NULL;

    internal_extract_control(control, &parsed_control);
    internal_extract_info(parsed_control.info, &parsed_info);

    internal_call_info_get_player_name(parsed_control.info, &player_name_sys_str);
    internal_extract_sys_str(player_name_sys_str, &player->name);
    
    internal_call_rb_get_position_injected(parsed_control.rb, &player->pos);

    player->id = parsed_control.id;
    player->role = parsed_info.role;
}

static int find_player_idx(player_t *player, int *success) {
    for (int i = 0; i < list.len; i++) {
        
        if (list.player[i].id == player->id) {
            *success = true;
            return i;
        }
    }

    *success = false;
    return 0;
}

static void remove_player(int idx) {
    if (idx == list.len - 1) {
        list.len--;

        return;
    }

    memmove(&list.player[idx], &list.player[idx + 1], (list.len - 1 - idx) * sizeof(list.player[0]));
    memmove(&list.timestamp[idx], &list.timestamp[idx + 1], (list.len - 1 - idx) * sizeof(list.timestamp[0]));

    list.len--;
}

void game_helpers_init() {
    memset(&list, 0, sizeof(list));
}

void game_hook_start_frame() {
    void *camera_raw = NULL;

    internal_static_call_get_main_camera(&camera_raw);

    internal_call_camera_get_projection_matrix_injected(camera_raw, &main_camera.proj_mat);
    internal_call_camera_get_view_matrix_injected(camera_raw, &main_camera.view_mat);

    main_camera.raw = camera_raw;
}

void game_hook_end_frame() {
    for (int i = list.len - 1; i >= 0; i--) {
        if (list.frame_count - list.timestamp[i] >= MAX_TIMESTAMP_DIFF) {
            remove_player(i);
        }
    }

    list.frame_count++;
}

void game_hook_player_update(void *control) {
    int success;

    player_t player;
    int player_idx;

    parse_player(control, &player);

    player_idx = find_player_idx(&player, &success);
    if (!success) {
        player_idx = list.len++;
    }

    list.player[player_idx] = player;
    list.timestamp[player_idx] = list.frame_count;
}

player_t *game_get_players(int *len) {
    *len = list.len;

    return list.player;
}

// TODO: Replace this with a real solution that doesn't require a static name.
player_t *game_get_local_player() {
    for (int i = 0; i < list.len; i++) {
        if (strcmp("Sighteach", list.player[i].name.data) == 0) {
            return &list.player[i];
        }
    }

    return &list.player[0];
}

camera_t *game_get_main_camera() {
    return &main_camera;
}

vec2f_t game_world_to_screen(vec2f_t world_point) {
    vec4f_t point_homo = (vec4f_t){ world_point.x, world_point.y, 0.0f, 1.0f };
    vec4f_t view_homo = vec4_mul_mat4x4(&point_homo, &main_camera.view_mat);
    vec4f_t clip_homo = vec4_mul_mat4x4(&view_homo, &main_camera.proj_mat);
    GLint vp[4] = { 0 };
    float x_nd = 0.0f;
    float y_nd = 0.0f;

    if (clip_homo.w < 0.0001f && clip_homo.w > -0.0001f) {
        return (vec2f_t){ 0 };
    }

    x_nd = clip_homo.x / clip_homo.w;
    y_nd = clip_homo.y / clip_homo.w;

    glGetIntegerv(GL_VIEWPORT, vp);

    vec2f_t out = { 0 };
    vp_transform(
        (float)vp[0],
        (float)vp[1],
        (float)vp[2],
        (float)vp[3],
        x_nd,
        y_nd,
        &out.x,
        &out.y
    );

    out.y = (float)vp[1] + (float)vp[3] - (out.y - (float)vp[1]);

    return out;
}
