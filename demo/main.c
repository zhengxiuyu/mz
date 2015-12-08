#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include "render.h"

static GLFWwindow *_window = NULL;
static struct render_state _state;

static void init() {
    init_render_state(&_state);
}

static void update() {
    render(&_state, NULL);
}

static void deinit() {
    deinit_render_state(&_state);
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
