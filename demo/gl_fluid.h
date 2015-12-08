#ifndef GL_FLUID_H
#define GL_FLUID_H

#include <GL/glew.h>

struct gl_fluid {
    GLuint vao;
    GLuint positions_vbo;
    GLsizei capacity;
    GLsizei count;
};

void init_gl_fluid(struct gl_fluid *fluid, GLsizei capacity);
void deinit_gl_fluid(struct gl_fluid *fluid);
void update_gl_fluid(
    struct gl_fluid *fluid,
    GLfloat (*positions)[2],
    GLsizei count
);

#endif /* end of include guard: GL_FLUID_H */
