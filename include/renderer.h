#pragma once

#include "glad/glad.h"

typedef struct renderer_s {
    int inited;
    GLuint shader;
    GLuint vbo;
    GLuint vao;
} renderer_t;

void render_new(renderer_t *renderer);
void render_scene(renderer_t *renderer);
