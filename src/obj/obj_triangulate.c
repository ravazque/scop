/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_triangulate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Faces to triangles that keep the winding of the face */

/* Appends a triangle, unless it has no area */
int	obj_emit(t_obj *obj, const unsigned int *abc, unsigned int id)
{
	t_obj_triangle	*grown;
	t_vec3			n;

	n = vec3_cross(vec3_sub(obj->positions[abc[1]], obj->positions[abc[0]]),
			vec3_sub(obj->positions[abc[2]], obj->positions[abc[0]]));
	if (!(vec3_length(n) > obj->min_area))
		return (1);
	grown = array_reserve(obj->triangles, &obj->triangle_cap,
			obj->triangle_count + 1, sizeof(t_obj_triangle));
	if (!grown)
		return (0);
	obj->triangles = grown;
	memcpy(obj->triangles[obj->triangle_count].corner, abc,
		3 * sizeof(unsigned int));
	obj->triangles[obj->triangle_count++].face = id;
	return (1);
}

/* Fan over what is left of the ring: the fallback for crossing outlines */
int	obj_fan(t_obj *obj, const t_obj_parser *p, size_t count, unsigned int id)
{
	unsigned int	abc[3];
	size_t			k;

	k = 1;
	while (k + 1 < count)
	{
		abc[0] = p->polygon[p->ring[0]];
		abc[1] = p->polygon[p->ring[k]];
		abc[2] = p->polygon[p->ring[k + 1]];
		if (!obj_emit(obj, abc, id))
			return (0);
		k++;
	}
	return (1);
}

/* The ring lists the corners still to clip; flat is their 2D projection */
static int	reserve_scratch(t_obj_parser *p, size_t n)
{
	size_t	*ring;
	float	*flat;
	size_t	i;

	ring = array_reserve(p->ring, &p->ring_cap, n, sizeof(size_t));
	if (!ring)
		return (0);
	p->ring = ring;
	flat = array_reserve(p->flat, &p->flat_cap, 2 * n, sizeof(float));
	if (!flat)
		return (0);
	p->flat = flat;
	i = 0;
	while (i < n)
	{
		p->ring[i] = i;
		i++;
	}
	return (1);
}

/* Polygons are ear-clipped in the plane of their Newell normal */
int	obj_triangulate(t_obj *obj, t_obj_parser *p, size_t n, unsigned int id)
{
	t_vec3	normal;

	if (n == 3)
		return (obj_emit(obj, p->polygon, id));
	if (!reserve_scratch(p, n))
		return (0);
	normal = obj_newell_normal(obj, p->polygon, n);
	if (!(vec3_length(normal) > obj->min_area))
		return (obj_fan(obj, p, n, id));
	obj_project(obj, p, n, normal);
	return (obj_ear_clip(obj, p, n, id));
}
