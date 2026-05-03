#include "game_hooks.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

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

static void parse_player(void *control, player_t *player) {
    player_control_t parsed_control = { 0 };
    player_info_t parsed_info = { 0 };

    void *player_name_sys_str = NULL;

    internal_extract_control(control, &parsed_control);
    internal_extract_info(parsed_control.info, &parsed_info);

    internal_call_info_get_player_name(parsed_control.info, &player_name_sys_str);
    internal_extract_sys_str(player_name_sys_str, &player->name);

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

void game_init() {
    memset(&list, 0, sizeof(list));
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

void game_hook_end_frame() {
    for (int i = list.len - 1; i >= 0; i--) {
        if (list.frame_count - list.timestamp[i] >= MAX_TIMESTAMP_DIFF) {
            remove_player(i);
        }
    }

    list.frame_count++;
}

player_t *game_get_players(int *len) {
    *len = list.len;

    return list.player;
}
