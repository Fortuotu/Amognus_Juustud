#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "glad/glad.h"

typedef struct gl_context_s {
    GLuint vbo;
    GLuint vao;
    GLuint shader;
    GLboolean depth_test;
} gl_context_t;

typedef struct vec2f_s {
    float x;
    float y;
} vec2f_t;

typedef struct vertex_s {
    float x;
    float y;
} vertex_t;

typedef struct renderer_s {
    gl_context_t ctx;
    gl_context_t last_ctx;

    size_t buffer_offset;
    size_t draw_count;

    uint8_t pointers_loaded;
    uint8_t fully_loaded;
} renderer_t;

void renderer_create(renderer_t *renderer);
void renderer_destroy(renderer_t *renderer);

void renderer_start(renderer_t *renderer);
void renderer_add_line(renderer_t *renderer, float x1, float y1, float x2, float y2);
void renderer_finish(renderer_t *renderer);
