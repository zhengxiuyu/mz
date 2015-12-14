#include "gl_fluid.h"
#include "common.h"

void init_gl_fluid(
    struct gl_fluid *gl_fluid,
    const mz_fluid *fluid
) {
    gl_fluid->count = fluid->num_particles;
    gl_fluid->rest_density = fluid->rest_density;
    glGenVertexArrays(1, &gl_fluid->vao);
    glBindVertexArray(gl_fluid->vao);
    glGenBuffers(1, &gl_fluid->positions_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->positions_vbo);
    glBufferData(GL_ARRAY_BUFFER, gl_fluid->count * sizeof(GLfloat[2]),
        fluid->positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glGenBuffers(1, &gl_fluid->densities_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->densities_vbo);
    glBufferData(GL_ARRAY_BUFFER, gl_fluid->count * sizeof(GLfloat),
        fluid->densities, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
}

void deinit_gl_fluid(struct gl_fluid *gl_fluid) {
    glDeleteVertexArrays(1, &gl_fluid->vao);
    glDeleteBuffers(1, &gl_fluid->positions_vbo);
    glDeleteBuffers(1, &gl_fluid->densities_vbo);
}

void update_gl_fluid(
    struct gl_fluid *gl_fluid,
    const mz_fluid *fluid
) {
    assert(gl_fluid->count == fluid->num_particles);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->positions_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gl_fluid->count * sizeof(GLfloat[2]),
        fluid->positions);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->densities_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gl_fluid->count * sizeof(GLfloat),
        fluid->densities);
}

