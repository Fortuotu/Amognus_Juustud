#include "renderer.h"
#include "io.h"
#include "glad/glad.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void renderer_create(renderer_t *renderer) {
    renderer->pointers_loaded = false;
    renderer->fully_loaded = false;

    renderer->vp_width = 0;
    renderer->vp_heigth = 0;

    renderer->buffer_offset = 0;
    renderer->draw_count = 0;
}

void renderer_destroy(renderer_t *renderer) {
    if (!renderer->fully_loaded) {
        return;
    }

    glDeleteVertexArrays(1, &renderer->ctx.vao);
    glDeleteBuffers(1, &renderer->ctx.vbo);
    glDeleteProgram(renderer->ctx.shader);
}

static char *read_file(const char *filename) {
    char *buffer;
    long length;
    FILE *f;

    f = fopen(filename, "r");
    if (f == NULL) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(length + 1);
    if (buffer == NULL) {
        fclose(f);

        return NULL;
    }

    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    
    fclose (f);

    return buffer;
}

static GLuint compile_shader(char *vertex_filename, char *fragment_filename, int *success) {
    char *vertex_code = read_file(vertex_filename);
    char *fragment_code = read_file(fragment_filename);

    char infolog[512];
    GLuint vertex;
    GLuint fragment;
    GLuint id;
    GLint params;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (char const * const *)&vertex_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &params);
    if (!params) {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        
        *success = 0;
        return 0;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (char const * const *)&fragment_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &params);
    if (!params) {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        
        *success = 0;
        return 0;
    }
    
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &params);
    if (!params) {
        glGetProgramInfoLog(id, 512, NULL, infolog);

        *success = 0;
        return 0;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertex_code);
    free(fragment_code);

    *success = 1;

    return id;
}

static void backup_ctx(gl_context_t *ctx) {
    glGetIntegerv(GL_CURRENT_PROGRAM, &ctx->shader);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint *)&ctx->vao);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&ctx->vbo);
    ctx->depth_test = glIsEnabled(GL_DEPTH_TEST);
}

static void use_ctx(const gl_context_t *ctx) {
    glUseProgram(ctx->shader);
    glBindVertexArray(ctx->vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
    if (ctx->depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
}

static void renderer_lazyload_pointers(renderer_t *renderer) {
    if (renderer->pointers_loaded) {
        return;
    }
    
    gladLoadGL();

    renderer->pointers_loaded = true;
}

static void renderer_lazyload(renderer_t *renderer) {
    if (renderer->fully_loaded) {
        return;
    }

    int success;

    if (!gladLoadGL()) {
        io_sendstr("Failed to load GL pointers\n");
        
        return;
    }

    gl_context_t last;
    backup_ctx(&last);

    renderer->ctx.shader = compile_shader("/home/ott/Documents/Amognus_Juustud/shaders/shader.vs", "/home/ott/Documents/Amognus_Juustud/shaders/shader.fs", &success);
    if (!success) {
        io_sendstr("Could not create shader\n");

        return;
    }

    glGenVertexArrays(1, &renderer->ctx.vao);
    glBindVertexArray(renderer->ctx.vao);
    glGenBuffers(1, &renderer->ctx.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->ctx.vbo);
    glBufferData(GL_ARRAY_BUFFER, 4096, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, sizeof(vertex_t) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)(sizeof(float) * 0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    renderer->fully_loaded = true;
}

static void renderer_update_vp(renderer_t *renderer) {
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    renderer->vp_x = (float)vp[0];
    renderer->vp_y = (float)vp[1];
    renderer->vp_width = (float)vp[2];
    renderer->vp_heigth = (float)vp[3];
}

void renderer_add_line(renderer_t *renderer, float x1, float y1, float x2, float y2) {
    const float half_width = 5.0f;

    vec2f_t diff;
    float diff_len;

    vec2f_t normal;
    vec2f_t normal_inv;

    vertex_t vertices[6];

    diff.x = x2 - x1;
    diff.y = y2 - y1;
    diff_len = sqrtf(diff.x * diff.x + diff.y * diff.y);
    if (diff_len < 0.1f) {
        return;
    }

    normal.x = -diff.y / diff_len;
    normal.y = diff.x / diff_len;
    normal_inv.x = -normal.x;
    normal_inv.y = -normal.y;

    vertices[0].x = x1 + normal.x * half_width;
    vertices[0].y = y1 + normal.y * half_width;
    vertices[1].x = x1 + normal_inv.x * half_width;
    vertices[1].y = y1 + normal_inv.y * half_width;
    vertices[2].x = x2 + normal.x * half_width;
    vertices[2].y = y2 + normal.y * half_width;
    vertices[3].x = x2 + normal.x * half_width;
    vertices[3].y = y2 + normal.y * half_width;
    vertices[4].x = x2 + normal_inv.x * half_width;
    vertices[4].y = y2 + normal_inv.y * half_width;
    vertices[5].x = x1 + normal_inv.x * half_width;
    vertices[5].y = y1 + normal_inv.y * half_width;

    for (int i = 0; i < 6; i++) {
        vp_transform_inv(
            renderer->vp_x,
            renderer->vp_y,
            renderer->vp_width,
            renderer->vp_heigth,
            vertices[i].x,
            vertices[i].y,
            &vertices[i].x,
            &vertices[i].y
        );
    }

    glBufferSubData(GL_ARRAY_BUFFER, renderer->buffer_offset, sizeof(vertices), vertices);
    renderer->buffer_offset += sizeof(vertices);
    renderer->draw_count += 6;
}

void renderer_start(renderer_t *renderer) {
    renderer_lazyload_pointers(renderer);

    backup_ctx(&renderer->last_ctx);
    use_ctx(&renderer->ctx);
    renderer_update_vp(renderer);
    renderer_lazyload(renderer);
}

void renderer_finish(renderer_t *renderer) {
    glDrawArrays(GL_TRIANGLES, 0, renderer->draw_count);
    renderer->buffer_offset = 0;
    renderer->draw_count = 0;

    use_ctx(&renderer->last_ctx);
}
