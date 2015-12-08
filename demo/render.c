#include "render.h"

#define to_string(x) #x

static const GLchar *vsh_source =
    "#version 410\n"
to_string(
    layout (location = 0) in vec2 position;

    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
);

static const GLchar *fsh_source =
    "#version 410\n"
to_string(
    layout (location = 0) out vec3 color;

    void main() {
        color = vec3(1.0, 0.0, 0.0);
    }
);

void init_render_state(struct render_state *state) {
    state->program = glCreateProgram();
    glh_attach_shader_with_source(state->program, GL_VERTEX_SHADER, vsh_source);
    glh_attach_shader_with_source(state->program, GL_FRAGMENT_SHADER, fsh_source);
    glh_link_program(state->program);
    glClearColor(0.2, 0.2, 0.2, 1.0);
}

void deinit_render_state(struct render_state *state) {
    glDeleteProgram(state->program);
}

void render(struct render_state *state, struct gl_fluid *fluid) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(state->program);
//    glBindVertexArray(fluid->vao);
//    glDrawArrays(GL_POINTS, 0, fluid->count);
}

