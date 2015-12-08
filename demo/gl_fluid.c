#include "gl_fluid.h"
#include "common.h"

void init_gl_fluid(struct gl_fluid *fluid, GLsizei capacity) {
    glGenVertexArrays(1, &fluid->vao);
    glBindVertexArray(fluid->vao);
    glGenBuffers(1, &fluid->positions_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fluid->positions_vbo);
    glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(GLfloat[2]), NULL,
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    fluid->count = 0;
    fluid->capacity = capacity;
}

void deinit_gl_fluid(struct gl_fluid *fluid) {
    glDeleteVertexArrays(1, &fluid->vao);
    glDeleteBuffers(1, &fluid->positions_vbo);
}

void update_gl_fluid(
    struct gl_fluid *fluid,
    GLfloat (*positions)[2],
    GLsizei count
) {
    GLsizei c = 0;

    if (count > fluid->capacity) {
        warn("particle count (%d) is greater than the particle capacity (%d)\n",
            count, fluid->count);
        c = fluid->capacity;
    } else {
        c = count;
    }

    glBindBuffer(GL_ARRAY_BUFFER, fluid->positions_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, c * sizeof(GLfloat[2]), positions);
    fluid->count = c;
}

