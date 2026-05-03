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
#include "renderer.h"
#include "utils.h"
#include "offsets.h"
#include "game.h"

static renderer_t renderer = { 0 };

static void render_overlay() {
    renderer_start(&renderer);

    renderer_add_line(&renderer, 25.0f, 25.0f, 125.0f, 25.0f, 15.0f);

    renderer_finish(&renderer);
    game_hook_end_frame();
} 

static void player_update(void *control) {
    game_hook_player_update(control);
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

    while (1) {
        io_recvstr(buf);

        if (strcmp(buf, "init") == 0) {
            game_init();
            hooks_init(render_overlay, player_update);

            sprintf(buf, "Initialized hooks, gameassembly base: %p\n", find_gameassembly());
            io_sendstr(buf);
        }
        else if (strcmp(buf, "toggle") == 0) {
            uint8_t enabled = hooks_toggle();
            if (enabled) {
                sprintf(buf, "Enabled hooks\n");
                io_sendstr(buf);
            } else {
                sprintf(buf, "Disabled hooks\n");
                io_sendstr(buf);
            }
        }
        else if (strcmp(buf, "players") == 0) {
            int len = 0;
            player_t *players = game_get_players(&len);

            for (int i = 0; i < len; i++) {
                sprintf(buf, "Player: %s, Role: %s\n", players[i].name, players[i].role_str);
                io_sendstr(buf);
            }
        }
    }
}

__attribute__((constructor))
void init() {
    pthread_t t = { 0 };
    pthread_create(&t, NULL, main_thread, NULL);
}
