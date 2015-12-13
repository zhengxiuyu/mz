#ifndef GLH_H
#define GLH_H

#include <GL/glew.h>

int glh_attach_shader_with_source(
    GLuint program,
    GLenum type,
    const GLchar *source
);
int glh_link_program(GLuint program);
void glh_uniform1f(GLuint program, const GLchar * name, GLfloat f);

#endif /* end of include guard: GLH_H */
