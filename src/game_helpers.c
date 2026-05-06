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
    int success = 0;

    player_t player = { 0 };
    int player_idx = 0;

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
    if (list.len == 0) { return NULL; }

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

vec2f_t game_world_to_screen(vec2f_t world_point, renderer_t *renderer) {
    vec4f_t point = (vec4f_t){ world_point.x, world_point.y, 0.0f, 1.0f };
    vec4f_t view = vec4_mul_mat4x4(&point, &main_camera.view_mat);
    vec4f_t clip = vec4_mul_mat4x4(&view, &main_camera.proj_mat);
    
    float x_nd = 0.0f;
    float y_nd = 0.0f;

    if (clip.w < 0.0001f && clip.w > -0.0001f) {
        return (vec2f_t){ 0 };
    }

    x_nd = clip.x / clip.w;
    y_nd = clip.y / clip.w;

    float vp_x;
    float vp_y;
    float vp_w;
    float vp_h;

    renderer_get_viewport(renderer, &vp_x, &vp_y, &vp_w, &vp_h);

    vec2f_t out = { 0 };
    vp_transform(
        vp_x,
        vp_y,
        vp_w,
        vp_h,
        x_nd,
        y_nd,
        &out.x,
        &out.y
    );

    out.y = vp_y + vp_h - (out.y - vp_y);

    return out;
}

uint8_t is_impostor(player_t *player) {
    return player->role == IMPOSTOR || player->role == VIPER || player->role == SHAPESHIFTER || player->role == PHANTOM || player->role == IMPOSTOR_GHOST;
}
