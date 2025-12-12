#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void get_position(const void *base, const void *rb, vec2f_t *ret) {
    const void *rb_get_pos = base + 0x1F798C0;

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

static void get_name(const void *base, const void *info, char *name, int *len) {
    const void *info_get_name = base + 0x7D6160;

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

static void get_role(const void *base, const void *info, player_role_t *ret, char *role_str) {
    (void)base;

    *ret = *(uint16_t *)(info + 0x38);
    str_for_role(*ret, role_str);
}

static void get_id(const void *base, const void *raw, uint8_t *id) {
    (void)base;

    *id = *(uint8_t *)(raw + 0x28);
}

void parse_player(const void *base, const void *raw, player_t *parsed) {
    void *info = *(void **)(raw + 0x58);
    void *rb = *(void **)(raw + 0xD0);

    if (rb == NULL || info == NULL) {
        memset(parsed, 0, sizeof(player_t));
        return;
    }

    get_id(base, raw, &parsed->id);
    get_position(base, rb, &parsed->pos);
    get_name(base, info, parsed->name, &parsed->name_len);
    get_role(base, info, &parsed->role, parsed->role_str);
}

void parse_player_id(const void *base, const void *raw, uint8_t *parsed) {
    get_id(base, raw, parsed);
}