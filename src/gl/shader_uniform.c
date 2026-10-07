/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_uniform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Uniforms set by name on the program in use */

void	shader_set_mat4(unsigned int program, const char *name, t_mat4 value)
{
	const t_gl	*g = gl();
	const int	location = g->uniform_location(program, name);

	g->uniform_matrix4fv(location, 1, GL_FALSE, value.m);
}

void	shader_set_float(unsigned int program, const char *name, float value)
{
	const t_gl	*g = gl();
	const int	location = g->uniform_location(program, name);

	g->uniform1f(location, value);
}

void	shader_set_vec2(unsigned int program, const char *name, float x,
			float y)
{
	const t_gl	*g = gl();
	const int	location = g->uniform_location(program, name);

	g->uniform2f(location, x, y);
}

void	shader_set_int(unsigned int program, const char *name, int value)
{
	const t_gl	*g = gl();
	const int	location = g->uniform_location(program, name);

	g->uniform1i(location, value);
}
