#include "renderer.h"
#include "io.h"
#include "glad/glad.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

static int compile_shader(char *vertex_filename, char *fragment_filename, int *success) {
    char *vertex_code = read_file(vertex_filename);
    char *fragment_code = read_file(fragment_filename);

    char infolog[512];
    GLuint vertex;
    GLuint fragment;
    GLuint id;
    GLint params = 0;
    
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

static void init_renderer(renderer_t *renderer) {
    int success;

    if (!gladLoadGL()) {
        io_sendstr("Failed to load GL pointers\n");
        
        return;
    }

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    renderer->shader = compile_shader("/home/ott/Documents/Amognus_Juustud/shaders/shader.vs", "/home/ott/Documents/Amognus_Juustud/shaders/shader.fs", &success);
    if (!success) {
        io_sendstr("Could not create shader\n");
        return;
    }

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(sizeof(GLfloat) * 0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    renderer->inited = 1;
}

void render_new(renderer_t *renderer) {
    renderer->inited = 0;
}

void render_scene(renderer_t *renderer) {
    static int i = 0;
    static char buf[512];

    if (!renderer->inited) {
        sprintf(buf, "#%d Init Renderer...\n", i++);
        io_sendstr(buf);

        init_renderer(renderer);
    }

    glUseProgram(renderer->shader);
    glBindVertexArray(renderer->vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
}
