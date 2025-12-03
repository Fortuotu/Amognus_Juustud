#pragma once

typedef struct vec2f_s {
    float x;
    float y;
} vec2f_t;

typedef enum player_role_e {
    CREWMATE = 0,
    IMPOSTOR = 1,
    SCIENTIST = 2,
    ENGINEER = 3,
    GUARDIAN_ANGEL = 4,
    SHAPESHIFTER = 5,
    CREWMATE_GHOST = 6,
    IMPOSTOR_GHOST = 7,
    NOISEMAKER = 8,
    PHANTOM = 9,
    TRACKER = 10,
    DETECTIVE = 12,
    VIPER = 18
} player_role_t;

typedef struct player_s {
    char name[128];
    int name_len;
    vec2f_t pos;
    player_role_t role;
    char role_str[64];
} player_t;

void parse_player(const void *base, const void *raw, player_t *parsed);
