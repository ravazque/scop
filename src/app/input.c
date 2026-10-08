#include "scop.h"

static volatile sig_atomic_t	g_interrupted = 0;

static void	on_interrupt(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

/* Keys that act once per press; the held ones that move the model are read each frame in view.c */
static void	on_key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	t_app	*app;

	(void)scancode;
	(void)mods;
	if (action != GLFW_PRESS)
		return ;
	app = glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if (key == GLFW_KEY_H)
		hud_toggle(app);
	else if (key == GLFW_KEY_T)
		app->textured.on = !app->textured.on;
	else if (key == GLFW_KEY_U)
		app->triplanar.on = !app->triplanar.on;
	else if (key == GLFW_KEY_L)
		app->lit.on = !app->lit.on;
	else if (key == GLFW_KEY_M)
		app->draw_mode = (app->draw_mode + 1) % DRAW_MODES;
	else if (key == GLFW_KEY_SPACE)
		app->view.paused = !app->view.paused;
	else if (key == GLFW_KEY_BACKSPACE)
		view_reset(&app->view);
}

/* Installed before anything else, so a Ctrl+C during start-up also ends cleanly */
void	input_catch_interrupt(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_interrupt;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	input_init(t_app *app)
{
	glfwSetWindowUserPointer(app->window, app);
	glfwSetKeyCallback(app->window, on_key);
}

int	input_interrupted(void)
{
	return (g_interrupted != 0);
}
