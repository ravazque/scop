#include "scop.h"

static int	scene_init(t_app *app, const t_obj *obj, const t_image *image)
{
	const t_gl	*g = gl();

	app->program = shader_load(MESH_VERT, MESH_FRAG);
	if (!app->program || !mesh_build(&app->mesh, obj))
		return (0);
	app->texture = texture_upload(image);
	app->texture_scale[0] = fminf(1.0f, (float)image->height / image->width);
	app->texture_scale[1] = fminf(1.0f, (float)image->width / image->height);
	g->use_program(app->program);
	shader_set_int(app->program, "uTexture", 0);
	g->enable(GL_DEPTH_TEST);
	g->point_size(POINT_SIZE);
	g->clear_color(CLEAR_R, CLEAR_G, CLEAR_B, 1.0f);
	return (1);
}

/* Model and texture are read before the window opens, so a bad file fails without one */
int	app_init(t_app *app, int argc, char **argv)
{
	t_obj	obj;
	t_image	image;
	int		ok;

	memset(app, 0, sizeof(*app));
	app->triplanar = (t_fade){1.0f, 1};
	input_catch_interrupt();
	if (!args_parse(app, argc, argv) || !obj_load(app->obj_path, &obj))
		return (0);
	ok = bmp_load(app->texture_path, &image) && window_init(app);
	if (ok)
	{
		input_init(app);
		ok = scene_init(app, &obj, &image);
	}
	obj_free(&obj);
	image_free(&image);
	return (ok);
}

static void	limit_frame_rate(double frame_start)
{
	const double	remaining = frame_start + 1.0 / FPS_CAP - glfwGetTime();
	struct timespec	pause;

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
	float	dt;

	prev = glfwGetTime();
	while (!glfwWindowShouldClose(app->window) && !input_interrupted())
	{
		glfwPollEvents();
		now = glfwGetTime();
		dt = fminf((float)(now - prev), MAX_FRAME_SECONDS);
		prev = now;
		view_update(&app->view, app->window, dt);
		fade_update(&app->textured, dt);
		fade_update(&app->triplanar, dt);
		fade_update(&app->lit, dt);
		hud_update(app, dt);
		draw_frame(app);
		glfwSwapBuffers(app->window);
		limit_frame_rate(now);
	}
}

/* Safe after a failed app_init: GL objects are deleted only while the context exists */
void	app_destroy(t_app *app)
{
	if (app->window)
	{
		mesh_destroy(&app->mesh);
		gl()->delete_textures(1, &app->texture);
		gl()->delete_program(app->program);
	}
	window_destroy(app);
}
