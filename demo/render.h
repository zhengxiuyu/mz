#ifndef RENDER_H
#define RENDER_H

#include "gl_fluid.h"

struct render_state {
    GLuint program;
};

void init_render_state(struct render_state *state);
void deinit_render_state(struct render_state *state);
void render(struct render_state *state, struct gl_fluid *fluid);

#endif /* end of include guard: RENDER_H */
