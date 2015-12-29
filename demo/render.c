#include "render.h"

#define to_string(x) #x

static const GLchar *vsh_source =
    "#version 410\n"
to_string(
    layout (location = 0) in vec2 position;
    layout (location = 1) in float density;
    out vec3 color_vout;
    uniform float rest_density;
    uniform float dens_diff_max;

    vec3 get_color(float v, float vmin, float vmax) {
        vec3 c = vec3(1.0);
        float dv;

        if (v < vmin)
            v = vmin;
        if (v > vmax)
            v = vmax;
        dv = vmax - vmin;

        if (v < (vmin + 0.25 * dv)) {
            c.r = 0;
            c.g = 4 * (v - vmin) / dv;
        } else if (v < (vmin + 0.5 * dv)) {
            c.r = 0;
            c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
        } else if (v < (vmin + 0.75 * dv)) {
            c.r = 4 * (v - vmin - 0.5 * dv) / dv;
            c.b = 0;
        } else {
            c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
            c.b = 0;
        }
        return c;
    }

    void main() {
        gl_PointSize = 4.0;
        gl_Position = vec4(position / 20.0, 0.0, 1.0);
        float diff = abs(density - rest_density);
        color_vout = get_color(diff, 0.0, dens_diff_max);
//        color_vout = vec3(0.0, 0.5, 0.8);
    }
);

static const GLchar *fsh_source =
    "#version 410\n"
to_string(
    layout (location = 0) out vec3 color;
    in vec3 color_vout;

    void main() {
        color = color_vout;
    }
);

void init_render_state(
    struct render_state *state,
    GLfloat dens_diff_max
) {
    state->dens_diff_max = dens_diff_max;
    state->program = glCreateProgram();
    glh_attach_shader_with_source(state->program, GL_VERTEX_SHADER, vsh_source);
    glh_attach_shader_with_source(state->program, GL_FRAGMENT_SHADER, fsh_source);
    glh_link_program(state->program);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void deinit_render_state(struct render_state *state) {
    glDeleteProgram(state->program);
}

void render(struct render_state *state, struct gl_fluid *fluid) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(state->program);
    glh_uniform1f(state->program, "rest_density", fluid->rest_density);
    glh_uniform1f(state->program, "dens_diff_max", state->dens_diff_max);
    glBindVertexArray(fluid->vao);
    glDrawArrays(GL_POINTS, 0, fluid->count);
}

