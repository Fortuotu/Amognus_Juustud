#include "parse.h"

#include "offsets.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void get_position(const void *rb, vec2f_t *ret) {
    if (rb == NULL) {
        ret->x = 0.0f;
        ret->y = 0.0f;

        return;
    }

    const void *rb_get_pos = find_gameassembly() + RIGIDBODY_2D_GET_POSITION;

    asm volatile (
        ".intel_syntax noprefix\n"
        "push %0\n"
        "push %1\n"
        "push %2\n"
        "call %3\n"
        "add esp, 12\n"
        ".att_syntax prefix\n"
        :
        : "r" (NULL), "r" (ret), "r" (rb), "r" (rb_get_pos)
        : "memory"
    );
}

static void get_name(const void *info, char *name, int *len) {
    if (info == NULL) {
        name[0] = '\0';
        *len = 0;

        return;
    }

    const void *info_get_name = find_gameassembly() + NETWORKED_PLAYER_INFO_GET_PLAYER_NAME;

    void *sys_str = NULL;

    uint16_t *ch = NULL;
    int i = 0;

    asm volatile (
        ".intel_syntax noprefix\n"
        "push %1\n"
        "push %2\n"
        "call %3\n"
        "add esp, 8\n"
        ".att_syntax prefix\n"
        : "=a" (sys_str)
        : "r" (NULL), "r" (info), "r" (info_get_name)
        : "memory"
    );

    if (sys_str == NULL) {
        name[0] = '\0';
        *len = 0;
        return;
    }

    *len = *(int *)(sys_str + 0x8);

    ch = (uint16_t *)(sys_str + 0xc);
    for (; i < *len; i++) {
        name[i] = (char)*ch++;
    }
    name[i] = '\0';
}

static void str_for_role(player_role_t role, char *out) {
    const char *s;

    switch (role) {
        case CREWMATE:         s = "Crewmate"; break;
        case IMPOSTOR:         s = "Impostor"; break;
        case SCIENTIST:        s = "Scientist"; break;
        case ENGINEER:         s = "Engineer"; break;
        case GUARDIAN_ANGEL:   s = "Guardian Angel"; break;
        case SHAPESHIFTER:     s = "Shapeshifter"; break;
        case CREWMATE_GHOST:   s = "Crewmate (Ghost)"; break;
        case IMPOSTOR_GHOST:   s = "Impostor (Ghost)"; break;
        case NOISEMAKER:       s = "Noisemaker"; break;
        case PHANTOM:          s = "Phantom"; break;
        case TRACKER:          s = "Tracker"; break;
        case DETECTIVE:        s = "Detective"; break;
        case VIPER:            s = "Viper"; break;
        default:               s = "Unknown"; break;
    }

    strcpy(out, s);
}

static void get_role(const void *info, player_role_t *ret, char *role_str) {
    if (info == NULL) {
        role_str[0] = '\0';
        *ret = -1;

        return;
    }

    *ret = *(uint16_t *)(info + NETWORKED_PLAYER_INFO_ROLE);
    str_for_role(*ret, role_str);
}

static void get_id(const void *control, uint8_t *id) {
    *id = *(uint8_t *)(control + PLAYER_CONTROL_PLAYER_ID);
}

void parse_player(const void *control, player_t *parsed) {
    void *info = *(void **)(control + PLAYER_CONTROL_NETWORKED_PLAYER_INFO);
    void *rb = *(void **)(control + PLAYER_CONTROL_RIGIDBODY_2D);

    get_id(control, &parsed->id);
    get_position(rb, &parsed->pos);
    get_name(info, parsed->name, &parsed->name_len);
    get_role(info, &parsed->role, parsed->role_str);
}

void parse_player_id(const void *control, uint8_t *parsed) {
    get_id(control, parsed);
}
