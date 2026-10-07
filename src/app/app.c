#include "scop.h"

/* Start-up, main loop and shut-down; the loop sleeps off the rest of each frame to hold FPS_CAP. */

static int	scene_init(t_app *app, const t_obj *obj)
{
	app->program = shader_load(MESH_VERT, MESH_FRAG);
	if (!app->program || !mesh_build(&app->mesh, obj))
		return (0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, 1.0f);
	return (1);
}

/* The model is read before the window opens, so a bad file fails fast; its CPU copy is freed once on the GPU. */
int	app_init(t_app *app, int argc, char **argv)
{
	t_obj	obj;
	int		ok;

	memset(app, 0, sizeof(*app));
	input_catch_interrupt();
	if (!args_parse(app, argc, argv) || !obj_load(app->obj_path, &obj))
		return (0);
	ok = window_init(app);
	if (ok)
	{
		input_init(app);
		ok = scene_init(app, &obj);
	}
	obj_free(&obj);
	return (ok);
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
