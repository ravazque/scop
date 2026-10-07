/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* .obj loading in two passes: v lines first, so a face may name any vertex */

/* Bounding-box diagonal of every vertex: what "degenerate" is measured by */
static float	model_size(const t_obj *obj)
{
	t_vec3	lo;
	t_vec3	hi;
	size_t	i;

	lo = obj->positions[0];
	hi = lo;
	i = 1;
	while (i < obj->position_count)
	{
		lo = vec3_min(lo, obj->positions[i]);
		hi = vec3_max(hi, obj->positions[i]);
		i++;
	}
	return (vec3_length(vec3_sub(hi, lo)));
}

static int	second_pass(t_obj *obj, t_obj_parser *p)
{
	float	size;
	size_t	i;

	if (p->face_count == 0 || obj->position_count == 0)
		return (obj_error(p, 0, "no faces to draw", NULL));
	size = model_size(obj);
	obj->min_area = OBJ_DEGENERATE_EPSILON * size * size;
	i = 0;
	while (i < p->face_count)
	{
		if (!obj_parse_face(obj, p, &p->faces[i], (unsigned int)i))
			return (0);
		i++;
	}
	if (obj->triangle_count == 0)
		return (obj_error(p, 0, "every face is degenerate (no area)", NULL));
	if (obj->triangle_count > (size_t)INT_MAX / 3)
		return (obj_error(p, 0, "too many triangles", NULL));
	obj->face_count = p->face_count;
	return (1);
}

/* 1 with every face triangulated; 0 after a message with file and line */
int	obj_load(const char *path, t_obj *obj)
{
	t_obj_parser	p;
	char			*buf;
	int				ok;

	memset(obj, 0, sizeof(*obj));
	memset(&p, 0, sizeof(p));
	p.path = path;
	buf = file_read(path, NULL);
	if (!buf)
		return (0);
	obj_join_lines(buf);
	ok = obj_first_pass(obj, &p, buf) && second_pass(obj, &p);
	free(buf);
	free(p.faces);
	free(p.polygon);
	free(p.ring);
	free(p.flat);
	if (!ok)
		obj_free(obj);
	return (ok);
}

void	obj_free(t_obj *obj)
{
	free(obj->positions);
	free(obj->triangles);
	memset(obj, 0, sizeof(*obj));
}
