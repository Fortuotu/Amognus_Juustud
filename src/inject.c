#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "io.h"
#include "override.h"
#include "parse.h"
#include "hooks.h"

void *find_gameassembly() {
    FILE *maps_file = fopen("/proc/self/maps", "r");
    char *line = NULL;
    size_t size = 0;

    void *start = NULL;
    void *end = NULL;
    char filename[1028] = { 0 };

    void *smallest = NULL;

    while (getline(&line, &size, maps_file) != -1) {
        sscanf(line, "%p-%p %*s %*s %*s %*s %1027[^\n]", &start, &end, filename);
        
        if (strstr(filename, "GameAssembly.dll") != NULL) {            
            if (start < smallest || smallest == NULL) {
                smallest = start;
            }
        }
    }

    free(line);
    fclose(maps_file);

    return smallest;
}

static void *gameassembly = NULL;

static int pcfu_idx = 0;
static void (*pcfu)(void *self) = NULL;

static void *latest_player = NULL;
static int pcfu_call_counter = 0;

#define MAX_PLAYERS 128

typedef struct player_list_s {
    int len;
    uint8_t id[MAX_PLAYERS];
    player_t data[MAX_PLAYERS];
} player_list_t;

player_list_t list = { 0 };
static pthread_mutex_t players_mutex = { 0 };

__attribute__((ms_abi))
void pcfu_override(void *self) {
    player_t player = { 0 };
    uint8_t player_id = -1;
    int player_idx = -1;

    override_disable(pcfu_idx);
    pcfu(self);
    override_enable(pcfu_idx);

    pthread_mutex_lock(&players_mutex);

    parse_player_id(gameassembly, self, &player_id);
    for (int i = 0; i < list.len; i++) {
        if (list.id[i] == player_id) {
            player_idx = i;
            break;
        }
    }

    if (player_idx == -1) {
        player_idx = list.len++;
        list.id[player_idx] = player_id;
    }

    parse_player(gameassembly, self, &player);
    list.data[player_idx] = player;

    pthread_mutex_unlock(&players_mutex);

    pcfu_call_counter++;
}

void *main_thread(void *arg) {
    (void)arg;

    int result = 0;

    static char buf[512] = { 0 };

    char filename[256];
    pid_t pid = getpid();
    sprintf(filename, "/tmp/cheat_io_%d.sock", pid);

    result = io_init(filename, sizeof(buf));
    if (result == -1) {
        exit(1);
    }

    result = io_wait_connection();
    if (result == -1) {
        exit(1);
    }

    pthread_mutex_init(&players_mutex, NULL);

    while (1) {
        io_recvstr(buf);

        if (strcmp(buf, "pid") == 0) {
            int pid = getpid();
            
            sprintf(buf, "%d\n", pid);
            io_sendstr(buf);
        }
        else if (strcmp(buf, "install") == 0) {
            gameassembly = find_gameassembly();
            pcfu = gameassembly + 0x5C2520;

            pcfu_idx = override_install(pcfu, pcfu_override);

            hooks_init();

            sprintf(buf, "Override succesfully installed, gameassembly base: %p\n", gameassembly);
            io_sendstr(buf);
        }
        else if (strcmp(buf, "enable") == 0) {
            override_enable(pcfu_idx);

            sprintf(buf, "Override enabled\n");
            io_sendstr(buf);
        }
        else if (strcmp(buf, "toggle") == 0) {
            hooks_toggle();

            sprintf(buf, "Toggled hooks.\n");
            io_sendstr(buf);
        }
        else if (strcmp(buf, "disable") == 0) {
            override_disable(pcfu_idx);

            sprintf(buf, "Override disabled\n");
            io_sendstr(buf);
        }
        else if (strcmp(buf, "clear") == 0) {
            memset(&list, 0, sizeof(list));

            sprintf(buf, "Cleared list\n");
            io_sendstr(buf);
        }
        else if (strcmp(buf, "state") == 0) {
            sprintf(buf, "%d ---\n", list.len);
            io_sendstr(buf);

            pthread_mutex_lock(&players_mutex);
            for (int i = 0; i < list.len; i++) {
                sprintf(buf, "id: %d, name: %s, x: %f, y: %f, role: %s\n", list.id[i], list.data[i].name, list.data[i].pos.x, list.data[i].pos.y, list.data[i].role_str);
                io_sendstr(buf);
            }
            pthread_mutex_unlock(&players_mutex);
        }
        else if (strcmp(buf, "count") == 0) {
            sprintf(buf, "Calls made: %d\n", pcfu_call_counter);
            io_sendstr(buf);
        }
        else if (strcmp(buf, "latest") == 0) {
            sprintf(buf, "latest: %p\n", latest_player);
            io_sendstr(buf);
        } 
        else if (strcmp(buf, "test") == 0) {
            sprintf(buf, "Hello from server\n");
            io_sendstr(buf);
        }
        else {
            sprintf(buf, "No such command\n");
            io_sendstr(buf);
        }
    }
}

__attribute__((constructor))
void init() {
    pthread_t t = { 0 };
    pthread_create(&t, NULL, main_thread, NULL);
}
