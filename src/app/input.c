#include "scop.h"

/* One-shot keys arrive through the GLFW callback (held keys are polled in view.c); Ctrl+C also closes cleanly, so valgrind sees a full shut-down. */

static volatile sig_atomic_t	g_interrupted = 0;

static void	on_interrupt(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

static void	key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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
	else if (key == GLFW_KEY_SPACE)
		app->view.paused = !app->view.paused;
	else if (key == GLFW_KEY_BACKSPACE)
		view_reset(&app->view);
}

/* Installed first, so a Ctrl+C during start-up also ends cleanly; sigaction keeps the handler, signal() may reset it. */
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
	glfwSetKeyCallback(app->window, key_callback);
}

int	input_interrupted(void)
{
	return (g_interrupted != 0);
}
