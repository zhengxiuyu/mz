#ifndef RENDER_H
#define RENDER_H

#include "gl_fluid.h"

struct render_state {
    GLuint program;
    GLfloat dens_diff_max;
};

void init_render_state(
    struct render_state *state,
    float dens_diff_max
);
void deinit_render_state(struct render_state *state);
void render(struct render_state *state, struct gl_fluid *fluid);

#endif /* end of include guard: RENDER_H */
