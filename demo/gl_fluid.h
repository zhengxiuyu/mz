#ifndef GL_FLUID_H
#define GL_FLUID_H

#include <mz.h>
#include "glh.h"

struct gl_fluid {
    GLuint vao;
    GLuint positions_vbo;
    GLuint densities_vbo;
    GLfloat rest_density;
    GLsizei count;
};

void init_gl_fluid(
    struct gl_fluid *gl_fluid,
    const mz_fluid *fluid
);
void deinit_gl_fluid(struct gl_fluid *gl_fluid);
void update_gl_fluid(
    struct gl_fluid *gl_fluid,
    const mz_fluid *fluid
);

#endif /* end of include guard: GL_FLUID_H */
