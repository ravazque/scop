#include "scop.h"

/* One frame: the model in perspective over the cleared background. */

static void	draw_model(t_app *app, float aspect)
{
	t_mat4	view;
	t_mat4	projection;

	view = mat4_look_at(vec3(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4_perspective(DEG2RAD(FOV_DEGREES), aspect, NEAR_PLANE, FAR_PLANE);
	glUseProgram(app->program);
	shader_set_mat4(app->program, "uModel", view_model_matrix(&app->view));
	shader_set_mat4(app->program, "uView", view);
	shader_set_mat4(app->program, "uProjection", projection);
	shader_set_vec2(app->program, "uTextureScale", app->texture_scale[0], app->texture_scale[1]);
	shader_set_float(app->program, "uTextureMix", fade_eased(&app->textured));
	shader_set_float(app->program, "uTriplanar", fade_eased(&app->triplanar));
	shader_set_float(app->program, "uLighting", fade_eased(&app->lit));
	glBindTexture(GL_TEXTURE_2D, app->texture);
	mesh_draw(&app->mesh);
}

void	draw_frame(t_app *app)
{
	int	width;
	int	height;

	glfwGetFramebufferSize(app->window, &width, &height);
	if (width <= 0 || height <= 0)
		return ;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_model(app, (float)width / (float)height);
}
