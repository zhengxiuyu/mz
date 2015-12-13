#include <stdio.h>
#include <stdlib.h>
#include "glh.h"

static char* read_file(const char * filename) {
    FILE* f = NULL;
    size_t numchars = 1; /* # of chars in the file */
    char* contents = NULL;
    char ch = 0;
    size_t count  = 0;

    f = fopen(filename, "r");
    if (!f)
        return NULL;

    /* count # of chars in the file */
    while (EOF != fgetc(f))
        numchars++;

    contents = (char*)malloc(numchars);

    if (!contents) {
        fclose(f);
        return NULL;
    }

    /* rewind and load the file's contents */
    rewind(f);

    while(1) {
        ch = fgetc(f);

        /* break if end of file is reached, don't forget to finish the string */
        if (ch == EOF) {
            contents[count] = '\0';
            break;
        }
        contents[count] = ch;
        count++;
    }

    /* clean up and return the contents */
    fclose(f);
    return contents;
}

int glh_attach_shader_with_source(
    GLuint program,
    GLenum type,
    const GLchar *source
) {
    GLuint shader = 0;
    GLint status;
    GLint infologlen;    /* length of the info log */
    GLchar* infolog;        /* the info log */

    shader = glCreateShader(type);
    if (!shader)
        return 1;

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    /* check if the shader compiled correctly */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    /* complain if something went wrong */
    if (status == GL_FALSE) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologlen);
        infolog = (GLchar*)malloc(infologlen + 1);
        glGetShaderInfoLog(shader, infologlen, NULL, infolog);
        puts(infolog);
        free(infolog);
        glDeleteShader(shader);
        return 1;
    }
    glAttachShader(program, shader);
    return 0;
}

int glh_link_program(GLuint program) {
    GLint status = 0;
    GLint infologlen;       /* length of the info log */
    GLchar* infolog;        /* the info log */
    GLint nshaders;         /* # of shader attached to the program */
    GLuint* shaders;        /* shaders attached to the program */
    int i = 0;

    /* link the program */
    glLinkProgram(program);

    /* check if everything went well */
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    /* complain if s.th went wrong */
    if (status == GL_FALSE){
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologlen);
        infolog = (char*)malloc(infologlen + 1);
        glGetProgramInfoLog(program, infologlen, NULL, infolog);
        puts(infolog);
        free(infolog);
        return 1;
    }

    /* detach and delete the shaders from the program */
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &nshaders);
    shaders = (GLuint*)malloc(sizeof(GLuint) * nshaders);
    glGetAttachedShaders(program, nshaders, NULL, shaders);
    for (i = 0; i < nshaders; i++) {
        glDetachShader(program, shaders[i]);
        glDeleteShader(shaders[i]);
    }
    free(shaders);
    return 0;
}

void glh_uniform1f(GLuint program, const GLchar * name, GLfloat f) {
	GLint loc = glGetUniformLocation(program, name);
	
	if (loc < 0)
		return;
	glUniform1f(loc, f);
}
