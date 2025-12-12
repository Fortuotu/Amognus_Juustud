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

static int compile_shader(char *vertex_filename, char *fragment_filename, char *infolog) {
    char *vertex_code = read_file(vertex_filename);
    char *fragment_code = read_file(fragment_filename);

    unsigned int vertex, fragment;
    int id;
    int success;
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (char const * const *)&vertex_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        return -1;
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (char const * const *)&fragment_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        return -1;
    };
    
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infolog);
        return -1;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertex_code);
    free(fragment_code);

    return id;
}

void render_init(renderer_t *renderer) {
    static char infolog[512];

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,

        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    renderer->shader = compile_shader("/home/ott/Documents/Amognus_Juustud/shaders/shader.vs", "/home/ott/Documents/Amognus_Juustud/shaders/shader.fs", infolog);
    if (renderer->shader == -1) {
        io_sendstr(infolog);
        return;
    }

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)(sizeof(GLfloat) * 0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void render_scene(renderer_t *renderer) {
    glUseProgram(renderer->shader);
    glBindVertexArray(renderer->vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
}
