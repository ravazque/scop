/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_project.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* The plane of a face: its Newell normal and a 2D projection onto it */

/* Newell's method: robust to concave and non-planar outlines */
t_vec3	obj_newell_normal(const t_obj *obj, const unsigned int *poly, size_t n)
{
	t_vec3	sum;
	t_vec3	a;
	t_vec3	b;
	size_t	i;

	sum = vec3(0.0f, 0.0f, 0.0f);
	i = 0;
	while (i < n)
	{
		a = obj->positions[poly[i]];
		b = obj->positions[poly[(i + 1) % n]];
		sum.x += (a.y - b.y) * (a.z + b.z);
		sum.y += (a.z - b.z) * (a.x + b.x);
		sum.z += (a.x - b.x) * (a.y + b.y);
		i++;
	}
	return (sum);
}

static float	coordinate(t_vec3 a, int axis)
{
	if (axis == 0)
		return (a.x);
	if (axis == 1)
		return (a.y);
	return (a.z);
}

static int	dominant_axis(t_vec3 n)
{
	const float	x = fabsf(n.x);
	const float	y = fabsf(n.y);
	const float	z = fabsf(n.z);

	if (x >= y && x >= z)
		return (0);
	if (y >= z)
		return (1);
	return (2);
}

/* Drops the dominant axis, mirrored so the outline turns counter-clockwise */
void	obj_project(const t_obj *obj, t_obj_parser *p, size_t n, t_vec3 normal)
{
	const int	drop = dominant_axis(normal);
	int			u;
	int			v;
	size_t		i;

	u = (drop + 1) % 3;
	v = (drop + 2) % 3;
	if (coordinate(normal, drop) < 0.0f)
	{
		u = v;
		v = (drop + 1) % 3;
	}
	i = 0;
	while (i < n)
	{
		p->flat[2 * i] = coordinate(obj->positions[p->polygon[i]], u);
		p->flat[2 * i + 1] = coordinate(obj->positions[p->polygon[i]], v);
		i++;
	}
}
