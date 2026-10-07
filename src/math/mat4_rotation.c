/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4_rotation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Rotations, counter-clockwise looking down the axis towards the origin */

t_mat4	mat4_rotation_x(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[5] = c;
	r.m[6] = s;
	r.m[9] = -s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_y(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[0] = c;
	r.m[2] = -s;
	r.m[8] = s;
	r.m[10] = c;
	return (r);
}

t_mat4	mat4_rotation_z(float radians)
{
	t_mat4	r;
	float	c;
	float	s;

	c = cosf(radians);
	s = sinf(radians);
	r = mat4_identity();
	r.m[0] = c;
	r.m[1] = s;
	r.m[4] = -s;
	r.m[5] = c;
	return (r);
}
