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
    lock_players();

    for (uint32_t i = 0; i < get_player_count(); i++) {
        player_t *player = get_player(i);
        
    }

    unlock_players();
    renderer_finish(&renderer);
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

        if (strcmp(buf, "install") == 0) {
            hooks_init(render_overlay, player_update);

            sprintf(buf, "Installed hooks, gameassembly base: %p\n", find_gameassembly());
            io_sendstr(buf);
        }
        else if (strcmp(buf, "toggle") == 0) {
            hooks_toggle();

            sprintf(buf, "Toggled hooks\n");
            io_sendstr(buf);
        }
    }
}

__attribute__((constructor))
void init() {
    pthread_t t = { 0 };
    pthread_create(&t, NULL, main_thread, NULL);
}
