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
#include "hooks.h"
#include "renderer.h"
#include "utils.h"
#include "offsets.h"
#include "game_internal.h"
#include "game_helpers.h"

typedef enum user_request_e {
    NO_REQUEST,
    SHOW_PLAYERS,
    SHOW_IMPOSTORS,
    SHOW_CAMERA
} user_request_t;

static user_request_t user_request = NO_REQUEST;

static renderer_t renderer = { 0 };

static void show_players_request() {
    char buf[512] = { 0 };

    int len = 0;
    player_t *players = game_get_players(&len);

    if (len == 0) {
        io_sendstr("Not in game\n");

        return;
    }

    for (int i = 0; i < len; i++) {
        sprintf(buf, "Player: %s, Role: %s\n", players[i].name.data, internal_string_for_role(players[i].role));
        io_sendstr(buf);
    }
}

static void show_impostors_request() {
    char buf[512] = { 0 };

    int len = 0;
    player_t *players = game_get_players(&len);

    if (len == 0) {
        io_sendstr("Not in game\n");

        return;
    }

    for (int i = 0; i < len; i++) {
        if (players[i].role != IMPOSTOR &&
            players[i].role != VIPER &&
            players[i].role != SHAPESHIFTER &&
            players[i].role != PHANTOM &&
            players[i].role != IMPOSTOR_GHOST)
        {
            continue;
        }
        
        sprintf(buf, "Player: %s, Role: %s\n", players[i].name.data, internal_string_for_role(players[i].role));
        io_sendstr(buf);
    }
}

static void show_camera_request() {
    char buf[512] = { 0 };

    camera_t *camera = game_get_main_camera();

    sprintf(buf, "Main Camera -> %p\n", camera->raw);
    io_sendstr(buf);

    io_sendstr("\n");

    io_sendstr("Projection Matrix:\n");

    sprintf_mat4x4(buf, &camera->proj_mat);
    io_sendstr(buf);

    io_sendstr("\n");

    io_sendstr("View Matrix:\n");

    sprintf_mat4x4(buf, &camera->view_mat);
    io_sendstr(buf);
}

static void handle_user_request() {
    switch (user_request) {
        case SHOW_PLAYERS:
            show_players_request();
            break;
        case SHOW_IMPOSTORS:
            show_impostors_request();
            break;
        case SHOW_CAMERA:
            show_camera_request();
            break;
        default:
            break;
    }

    user_request = NO_REQUEST;
}

static void render_overlay() {
    game_hook_start_frame();

    handle_user_request();

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

    int result = false;

    uint8_t inited = false;
    uint8_t toggled = false;

    static char buf[512] = { 0 };

    pid_t pid = getpid();
    sprintf(buf, "/tmp/.cheat_io_%d.sock", pid);

    result = io_init(buf, sizeof(buf));
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
            if (!inited) {
                game_helpers_init();
                hooks_init(render_overlay, player_update);

                sprintf(buf, "Initialized hooks, gameassembly base: %p\n", find_gameassembly());
                io_sendstr(buf);

                inited = true;
            } else {
                sprintf(buf, "Already initialized!\n");
                io_sendstr(buf);
            }

            continue;
        }
        
        if (!inited) {
            sprintf(buf, "Run 'init' to initialize first\n");
            io_sendstr(buf);

            continue;
        }
        
        if (strcmp(buf, "toggle") == 0) {
            toggled = hooks_toggle();
            if (toggled) {
                sprintf(buf, "Enabled hooks\n");
                io_sendstr(buf);
            }
            else {
                sprintf(buf, "Disabled hooks\n");
                io_sendstr(buf);
            }

            continue;
        }

        if (!toggled) {
            sprintf(buf, "Run 'toggle' to toggle hooks first\n");
            io_sendstr(buf);

            continue;
        }

        if (strcmp(buf, "players") == 0) {
            user_request = SHOW_PLAYERS;
        } else if (strcmp(buf, "impostors") == 0) {
            user_request = SHOW_IMPOSTORS;
        } else if (strcmp(buf, "camera") == 0) {
            user_request = SHOW_CAMERA;
        }
    }
}

__attribute__((constructor))
void init() {
    pthread_t t = { 0 };
    pthread_create(&t, NULL, main_thread, NULL);
}
