/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_vertices.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* One vertex per triangle corner: position, face normal and face shade */

/* Center and scale from the corners actually drawn, not unused vertices */
static t_fit	fit_model(const t_obj *obj)
{
	t_vec3	lo;
	t_vec3	hi;
	t_vec3	p;
	size_t	i;

	lo = obj->positions[obj->triangles[0].corner[0]];
	hi = lo;
	i = 0;
	while (i < obj->triangle_count * 3)
	{
		p = obj->positions[obj->triangles[i / 3].corner[i % 3]];
		lo = vec3_min(lo, p);
		hi = vec3_max(hi, p);
		i++;
	}
	return ((t_fit){vec3_scale(vec3_add(lo, hi), 0.5f),
		2.0f / vec3_length(vec3_sub(hi, lo))});
}

static void	write_vertex(float *out, t_vec3 p, t_vec3 n, float shade)
{
	out[0] = p.x;
	out[1] = p.y;
	out[2] = p.z;
	out[3] = n.x;
	out[4] = n.y;
	out[5] = n.z;
	out[6] = shade;
}

/* The shade of a face comes from its .obj index, spread by the golden ratio */
static void	write_triangle(float *out, const t_obj *obj,
		const t_obj_triangle *t, const t_fit *fit)
{
	t_vec3	p[3];
	t_vec3	n;
	float	shade;
	int		k;

	k = 0;
	while (k < 3)
	{
		p[k] = vec3_sub(obj->positions[t->corner[k]], fit->center);
		p[k] = vec3_scale(p[k], fit->scale);
		k++;
	}
	n = vec3_normalize(vec3_cross(vec3_sub(p[1], p[0]),
				vec3_sub(p[2], p[0])));
	shade = (float)fmod((double)t->face * MESH_SHADE_STEP, 1.0);
	k = 0;
	while (k < 3)
	{
		write_vertex(out + k * MESH_VERTEX_FLOATS, p[k], n, shade);
		k++;
	}
}

/* Centered on the bounding box and scaled into the unit sphere */
void	mesh_fill_vertices(float *vertices, const t_obj *obj)
{
	const t_fit	fit = fit_model(obj);
	size_t		i;

	i = 0;
	while (i < obj->triangle_count)
	{
		write_triangle(vertices + i * 3 * MESH_VERTEX_FLOATS, obj,
			&obj->triangles[i], &fit);
		i++;
	}
}
