#include "game.h"
#include "parse.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#define MAX_PLAYERS 32
#define MAX_TIMESTAMP_DIFF 5

typedef struct player_list_s {
    uint32_t len;
    uint32_t frame_count;
    player_t player[MAX_PLAYERS];
    uint32_t timestamp[MAX_PLAYERS];
} player_list_t;

static uint8_t inited = false;

static pthread_mutex_t list_mutex = { 0 };
static player_list_t list = { 0 };

void lock_players() {
    pthread_mutex_lock(&list_mutex);
}

void unlock_players() {
    pthread_mutex_unlock(&list_mutex);
}

static uint32_t find_player_idx(player_t *player, uint8_t *success) {
    for (uint32_t i = 0; i < list.len; i++) {
        
        if (list.player[i].id == player->id) {
            *success = true;
            return i;
        }
    }

    *success = false;
    return 0;
}

static void remove_player(uint32_t idx) {
    if (idx == list.len - 1) {
        list.len--;

        return;
    }

    memmove(&list.player[idx], &list.player[idx + 1], (list.len - 1 - idx) * sizeof(list.player[0]));
    memmove(&list.timestamp[idx], &list.timestamp[idx + 1], (list.len - 1 - idx) * sizeof(list.timestamp[0]));

    list.len--;
}

void player_update(void *control) {
    uint8_t success;

    player_t player;
    uint32_t player_idx;

    lock_players();

    parse_player(control, &player);

    player_idx = find_player_idx(&player, &success);
    if (!success) {
        player_idx = list.len++;
    }

    if (list.frame_count++ - list.timestamp[player_idx] >= MAX_TIMESTAMP_DIFF) {
        remove_player(player_idx);
    }
    else {
        list.player[player_idx] = player;
        list.timestamp[player_idx] = list.frame_count; 
    }

    unlock_players();
}

uint32_t get_player_count() {
    return list.len;
}

player_t *get_player(uint32_t idx) {
    return &list.player[idx];
}
