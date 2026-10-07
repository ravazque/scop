#include "scop.h"

/* Start-up, main loop and shut-down; the loop sleeps off the rest of each frame to hold FPS_CAP. */

int	app_init(t_app *app, int argc, char **argv)
{
	memset(app, 0, sizeof(*app));
	input_catch_interrupt();
	if (!args_parse(app, argc, argv) || !window_init(app))
		return (0);
	input_init(app);
	app->program = shader_load(MESH_VERT, MESH_FRAG);
	if (!app->program)
		return (0);
	app->mesh = mesh_cube();
	glEnable(GL_DEPTH_TEST);
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, 1.0f);
	return (1);
}

static void	limit_frame_rate(double frame_start)
{
	struct timespec	pause;
	double			remaining;

	remaining = frame_start + 1.0 / FPS_CAP - glfwGetTime();
	if (remaining <= 0.0)
		return ;
	pause.tv_sec = 0;
	pause.tv_nsec = (long)(remaining * 1e9);
	nanosleep(&pause, NULL);
}

void	app_run(t_app *app)
{
	double	prev;
	double	now;
	float	frame_time;

	prev = glfwGetTime();
	while (!glfwWindowShouldClose(app->window) && !input_interrupted())
	{
		glfwPollEvents();
		now = glfwGetTime();
		frame_time = fminf((float)(now - prev), MAX_FRAME_TIME);
		prev = now;
		app->angle = fmodf(app->angle + SPIN_SPEED * frame_time, 2.0f * SCOP_PI);
		hud_update(app, frame_time);
		draw_frame(app);
		glfwSwapBuffers(app->window);
		limit_frame_rate(now);
	}
}

/* Safe after a failed app_init: GL objects are released only while the context exists, and glDelete* ignore 0. */
void	app_destroy(t_app *app)
{
	if (app->window)
	{
		mesh_destroy(&app->mesh);
		glDeleteProgram(app->program);
	}
	window_destroy(app);
}
