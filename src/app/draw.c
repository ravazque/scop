#include "scop.h"

void	fade_update(t_fade *fade, float dt)
{
	const float	step = dt / FADE_SECONDS;

	if (fade->on)
		fade->value = fminf(fade->value + step, 1.0f);
	else
		fade->value = fmaxf(fade->value - step, 0.0f);
}

/* Smoothstep of the linear value, so every transition starts and ends gently */
static float	fade_eased(const t_fade *fade)
{
	return (fade->value * fade->value * (3.0f - 2.0f * fade->value));
}

static void	set_uniforms(t_app *app, float aspect)
{
	const unsigned int	p = app->program;
	const t_vec3		eye = vec3(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE);

	shader_set_mat4(p, "uModel", view_model_matrix(&app->view));
	shader_set_mat4(p, "uView", mat4_look_at(eye, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	shader_set_mat4(p, "uProjection", mat4_perspective(FOV, aspect, NEAR_PLANE, FAR_PLANE));
	shader_set_vec2(p, "uTextureScale", app->texture_scale[0], app->texture_scale[1]);
	shader_set_float(p, "uTextureMix", fade_eased(&app->textured));
	shader_set_float(p, "uTriplanar", fade_eased(&app->triplanar));
	shader_set_float(p, "uLighting", fade_eased(&app->lit));
}

void	draw_frame(t_app *app)
{
	static const int	modes[DRAW_MODES] = {GL_FILL, GL_LINE, GL_POINT};
	const t_gl			*g = gl();
	int					width;
	int					height;

	glfwGetFramebufferSize(app->window, &width, &height);
	if (width <= 0 || height <= 0)
		return ;
	g->viewport(0, 0, width, height);
	g->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g->use_program(app->program);
	set_uniforms(app, (float)width / (float)height);
	g->bind_texture(GL_TEXTURE_2D, app->texture);
	g->polygon_mode(GL_FRONT_AND_BACK, modes[app->draw_mode]);
	mesh_draw(&app->mesh);
}
