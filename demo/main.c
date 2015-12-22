#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mz.h>
#include "common.h"
#include "render.h"
#include <unistd.h>

/* simulation parameters */
#define REST_DENSITY 1000
#define NUM_PARTICLES_SUPP 15
#define SUPPORT 0.1
#define NUM_PARTICLES_SQRT 33
#define NUM_PARTICLES NUM_PARTICLES_SQRT * NUM_PARTICLES_SQRT

static GLFWwindow *_window = NULL;
static struct render_state _state;
static struct gl_fluid _gl_fluid;
static mz_fluid _fluid;
static mz_fluid _fluid_2;
static mz_grid _grid;
static mz_domain _domain;

static void init_fluid()
{
    int i, j;

    mz_init_fluid(&_fluid, REST_DENSITY, NUM_PARTICLES);
    for (i = 0; i < NUM_PARTICLES_SQRT; i++) {
        for (j = 0; j < NUM_PARTICLES_SQRT; j++) {
            int idx = NUM_PARTICLES_SQRT * i + j;

            _fluid.positions[idx][0] = -0.5 + i / ((float)NUM_PARTICLES_SQRT - 1.0);
            _fluid.positions[idx][1] = -0.5 + j / ((float)NUM_PARTICLES_SQRT - 1.0);
        }
    }

    init_gl_fluid(&_gl_fluid, &_fluid);
    mz_make_domain(&_domain, -1.0, -1.0, 1.0, 1.0);
    mz_init_grid(&_grid, &_fluid, &_domain, SUPPORT);
    printf("SUPPORT = %f\n", SUPPORT);
}

static void init() {
    init_render_state(&_state, 500.0);
    init_fluid();
}

static void update() {
    sleep(1.0);
    mz_update_grid(&_grid, &_fluid);
    mz_calc_lambdas_naive(&_fluid, SUPPORT);
    mz_calc_dpositions_naive(&_fluid, SUPPORT);
    mz_update_positions(&_fluid);


    printf("0  [%f %f] [%f %f] %f\n", _fluid.positions[0][0], _fluid.positions[0][1], _fluid.dpositions[0][0], _fluid.dpositions[0][1], _fluid.densities[0]);
    printf("32 [%f %f] [%f %f] %f\n", _fluid.positions[32][0], _fluid.positions[32][1], _fluid.dpositions[32][0], _fluid.dpositions[32][1], _fluid.densities[32]);
//    for (int i = 0; i < _fluid.num_particles; i++) {
//        float *dpos = _fluid.dpositions[i];
//        float len = sqrtf(dpos[0] * dpos[0] + dpos[1] * dpos[1]);
//        printf("%-3d %-10.5f %-10.5f %-10.5f %-10.5f %-10.5f\n", i, _fluid.densities[i], _fluid.lambdas[i], _fluid.dpositions[i][0], _fluid.dpositions[i][1], len);
//    }

//    for (int i = 0; i < _grid.num_cells_total; i++)
//        printf("%d\n", _grid.num_particles[i]);

    update_gl_fluid(&_gl_fluid, &_fluid);
    render(&_state, &_gl_fluid);
}

static void deinit() {
    deinit_render_state(&_state);
    mz_deinit_fluid(&_fluid);
    mz_deinit_grid(&_grid);
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

