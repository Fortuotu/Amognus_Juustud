#pragma once

#include "glad/glad.h"

typedef struct renderer_s {
    int shader;
    GLuint vbo;
    GLuint vao;
} renderer_t;

void render_init(renderer_t *renderer);
void render_scene(renderer_t *renderer);
