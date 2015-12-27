#include <stdlib.h>
#include "gl_fluid.h"
#include "common.h"

static void copy(struct gl_fluid *gl_fluid, const struct mz_fluid *fluid) {
    int i = 0;

    for (; i < gl_fluid->count; i++) {
        gl_fluid->positions[i][0] = (float)fluid->positions[i][0];
        gl_fluid->positions[i][1] = (float)fluid->positions[i][1];
        gl_fluid->densities[i] = (float)fluid->densities[i];
    }
}

void init_gl_fluid(
    struct gl_fluid *gl_fluid,
    const struct mz_fluid *fluid
) {
    gl_fluid->count = fluid->num_particles;
    gl_fluid->rest_density = fluid->rest_density;
    gl_fluid->positions = calloc(fluid->num_particles, sizeof(*fluid->positions));
    gl_fluid->densities = calloc(fluid->num_particles, sizeof(*fluid->densities));
    glGenVertexArrays(1, &gl_fluid->vao);
    glBindVertexArray(gl_fluid->vao);
    glGenBuffers(1, &gl_fluid->positions_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->positions_vbo);
    glBufferData(GL_ARRAY_BUFFER, gl_fluid->count * sizeof(GLfloat[2]),
        NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glGenBuffers(1, &gl_fluid->densities_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->densities_vbo);
    glBufferData(GL_ARRAY_BUFFER, gl_fluid->count * sizeof(GLfloat),
        NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
}

void deinit_gl_fluid(struct gl_fluid *gl_fluid) {
    free(gl_fluid->positions);
    free(gl_fluid->densities);
    glDeleteVertexArrays(1, &gl_fluid->vao);
    glDeleteBuffers(1, &gl_fluid->positions_vbo);
    glDeleteBuffers(1, &gl_fluid->densities_vbo);
}

void update_gl_fluid(
    struct gl_fluid *gl_fluid,
    const struct mz_fluid *fluid
) {
    assert(gl_fluid->count == fluid->num_particles);
    copy(gl_fluid, fluid);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->positions_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gl_fluid->count * sizeof(GLfloat[2]),
        gl_fluid->positions);
    glBindBuffer(GL_ARRAY_BUFFER, gl_fluid->densities_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gl_fluid->count * sizeof(GLfloat),
        gl_fluid->densities);
}

