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

void *find_gameassembly() {
    FILE *maps_file = fopen("/proc/self/maps", "r");
    char *line = NULL;
    size_t size = 0;

    void *start = NULL;
    void *end = NULL;
    char filename[1028] = { 0 };

    void *smallest = NULL;

    while (getline(&line, &size, maps_file) != -1) {
        sscanf(line, "%lx-%lx %*s %*s %*s %*s %1027[^\n]", &start, &end, filename);
        
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

void *gameassembly = NULL;

static int pcfu_idx = 0;
static void (*pcfu)(void *self) = NULL;

static void *latest_player = NULL;
int pcfu_call_counter = 0;

__attribute__((ms_abi))
void pcfu_override(void *self) {
    static uint8_t buf[512] = { 0 };
    static player_t player = { 0 };

    override_disable(pcfu_idx);
    pcfu(self);
    override_enable(pcfu_idx);

    parse_player(gameassembly, self, &player);   
    sprintf(buf, "name: %s, x: %f, y: %f, role: %s\n", player.name, player.pos.x, player.pos.y, player.role_str);
    io_sendstr(buf);

    pcfu_call_counter++;
}

void *main_thread(void *arg) {
    (void)arg;

    int result = 0;

    player_t player = { 0 };

    uint8_t buf[512] = { 0 };

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

        if (strcmp(buf, "pid") == 0) {
            int pid = getpid();
            
            sprintf(buf, "%d\n", pid);
        }
        else if (strcmp(buf, "install") == 0) {
            gameassembly = find_gameassembly();
            pcfu = gameassembly + 0x5C2520;

            pcfu_idx = override_install(pcfu, pcfu_override);
            override_enable(pcfu_idx);

            sprintf(buf, "Override succesfully installed, gameassembly base: %p\n", gameassembly);
        }
        else if (strcmp(buf, "count") == 0) {
            sprintf(buf, "Calls made: %d\n", pcfu_call_counter);
        }
        else if (strcmp(buf, "latest") == 0) {
            sprintf(buf, "latest: %p\n", latest_player);
        } 
        else if (strcmp(buf, "test") == 0) {
            sprintf(buf, "Hello from server\n");
        }
        else {
            sprintf(buf, "No such command\n");
        }

        io_sendstr(buf);
    }
}

__attribute__((constructor))
void init() {
    pthread_t t = { 0 };
    pthread_create(&t, NULL, main_thread, NULL);
}
