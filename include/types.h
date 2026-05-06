#pragma once

typedef struct vec2f_s {
    float x;
    float y;
} vec2f_t;

typedef struct mat4x4f_s {
    float m1;
    float m2;
    float m3;
    float m4;
    float m5;
    float m6;
    float m7;
    float m8;
    float m9;
    float m10;
    float m11;
    float m12;
    float m13;
    float m14;
    float m15;
    float m16;
} mat4x4f_t;

#define STATIC_SRING_MAX_LEN 128

typedef struct static_string_s {
    char data[STATIC_SRING_MAX_LEN];
    int len;
} static_string_t;
