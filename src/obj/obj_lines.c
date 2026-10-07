/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_lines.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* First pass over a .obj: v lines are parsed, f lines kept for later */

static int	parse_vertex(t_obj *obj, t_obj_parser *p, char *cursor,
		size_t line)
{
	float	xyz[3];
	char	*token;
	char	*end;
	t_vec3	*grown;
	int		i;

	i = -1;
	while (++i < 3)
	{
		token = obj_token(&cursor);
		if (!token)
			return (obj_error(p, line, "a vertex needs x, y and z", NULL));
		xyz[i] = strtof(token, &end);
		if (end == token || *end || !isfinite(xyz[i]))
			return (obj_error(p, line, "invalid vertex coordinate", token));
	}
	if (obj->position_count >= UINT_MAX)
		return (obj_error(p, line, "too many vertices", NULL));
	grown = array_reserve(obj->positions, &obj->position_cap,
			obj->position_count + 1, sizeof(t_vec3));
	if (!grown)
		return (0);
	obj->positions = grown;
	obj->positions[obj->position_count++] = vec3(xyz[0], xyz[1], xyz[2]);
	return (1);
}

static int	keep_face(t_obj_parser *p, char *cursor, size_t line, size_t seen)
{
	t_obj_face	*grown;

	if (p->face_count >= UINT_MAX)
		return (obj_error(p, line, "too many faces", NULL));
	grown = array_reserve(p->faces, &p->face_cap, p->face_count + 1,
			sizeof(t_obj_face));
	if (!grown)
		return (0);
	p->faces = grown;
	p->faces[p->face_count++] = (t_obj_face){cursor, line, seen};
	return (1);
}

/* Ends the line at its newline and drops its comment; returns the next one */
static char	*cut_line(char *line)
{
	char	*next;
	char	*comment;

	next = strchr(line, '\n');
	if (next)
		*next++ = '\0';
	else
		next = line + strlen(line);
	comment = strchr(line, '#');
	if (comment)
		*comment = '\0';
	return (next);
}

/* Other keywords (vt, vn, o, g, s, usemtl, mtllib...) are ignored */
int	obj_first_pass(t_obj *obj, t_obj_parser *p, char *line)
{
	char	*next;
	char	*cursor;
	char	*key;
	size_t	number;
	int		ok;

	number = 0;
	ok = 1;
	while (ok && *line)
	{
		number++;
		next = cut_line(line);
		cursor = line;
		key = obj_token(&cursor);
		if (key && strcmp(key, "v") == 0)
			ok = parse_vertex(obj, p, cursor, number);
		else if (key && strcmp(key, "f") == 0)
			ok = keep_face(p, cursor, number, obj->position_count);
		line = next;
	}
	return (ok);
}
