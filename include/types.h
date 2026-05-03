#pragma once

typedef struct vec2f_s {
    float x;
    float y;
} vec2f_t;

#define STATIC_SRING_MAX_LEN 128

typedef struct static_string_s {
    char data[STATIC_SRING_MAX_LEN];
    int len;
} static_string_t;
