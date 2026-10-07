/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* One frame: the model in perspective over the cleared background */

static void	set_uniforms(t_app *app, float aspect)
{
	const unsigned int	p = app->program;
	t_mat4				view;
	t_mat4				projection;

	view = mat4_look_at(vec3(0.0f, CAMERA_HEIGHT, CAMERA_DISTANCE),
			vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4_perspective(FOV, aspect, NEAR_PLANE, FAR_PLANE);
	shader_set_mat4(p, "uModel", view_model_matrix(&app->view));
	shader_set_mat4(p, "uView", view);
	shader_set_mat4(p, "uProjection", projection);
	shader_set_vec2(p, "uTextureScale", app->texture_scale[0],
		app->texture_scale[1]);
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
