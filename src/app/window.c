#include "scop.h"

/* The window and its OpenGL 4.1 core context: GLFW creates them, gl_loader fills the GL entry points. */

static void	glfw_error_callback(int code, const char *description)
{
	fprintf(stderr, "GLFW error %d: %s\n", code, description);
}

int	window_init(t_app *app)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return (fprintf(stderr, "Error: cannot initialize GLFW\n"), 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	app->window = glfwCreateWindow(app->width, app->height, WIN_TITLE, NULL, NULL);
	if (!app->window)
	{
		fprintf(stderr, "Error: cannot create an OpenGL 4.1 core window\n");
		return (glfwTerminate(), 0);
	}
	glfwMakeContextCurrent(app->window);
	glfwSwapInterval(0);
	if (!gl_load_functions())
		return (window_destroy(app), 0);
	return (1);
}

/* Safe at any stage of window_init: glfwTerminate is a no-op before glfwInit. */
void	window_destroy(t_app *app)
{
	if (app->window)
		glfwDestroyWindow(app->window);
	app->window = NULL;
	glfwTerminate();
}
