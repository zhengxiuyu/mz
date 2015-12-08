#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mz.h>
#include "common.h"
#include "render.h"

static GLFWwindow *_window = NULL;
static struct render_state _state;
static struct gl_fluid _gl_fluid;
static mz_fluid _fluid;

static void init_fluid()
{
    int i, j;

    mz_init_fluid(&_fluid, 10000);
    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            int idx = 100 * i + j;
            _fluid.positions[idx][0] = -0.5 + i / 100.0;
            _fluid.positions[idx][1] = -0.5 + j / 100.0;
        }
    }

    init_gl_fluid(&_gl_fluid, 10000);
}

static void init() {
    init_render_state(&_state);
    init_fluid();
}

static void update() {
    update_gl_fluid(&_gl_fluid, _fluid.positions, _fluid.num_particles);
    render(&_state, &_gl_fluid);
}

static void deinit() {
    deinit_render_state(&_state);
    mz_deinit_fluid(&_fluid);
    deinit_gl_fluid(&_gl_fluid);
}

int main(int argc, const char *argv[]) {
    double tl = 0.0;

    assert(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(800, 800, "Position Based Fluids", NULL, NULL);
    assert(_window);

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    assert(GLEW_OK == glewInit());

    glGetError();

    init();
    tl = glfwGetTime();
    while (!glfwWindowShouldClose(_window)) {
        double tc = glfwGetTime() - tl;
        tl = glfwGetTime();
        update();
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    deinit();
    glfwTerminate();
    return 0;
}
