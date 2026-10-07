/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_geometry.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Cross product, length, normalization and component-wise bounds */

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x});
}

float	vec3_length(t_vec3 a)
{
	return (sqrtf(vec3_dot(a, a)));
}

t_vec3	vec3_normalize(t_vec3 a)
{
	float	len;

	len = vec3_length(a);
	if (len > 0.0f)
		return (vec3_scale(a, 1.0f / len));
	return ((t_vec3){0.0f, 0.0f, 0.0f});
}

t_vec3	vec3_min(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)});
}

t_vec3	vec3_max(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)});
}
