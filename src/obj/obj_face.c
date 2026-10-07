/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_face.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* f lines: corners v, v/vt, v//vn or v/vt/vn; only v is used, all checked */

static int	parse_optional_index(const char *s, const char **end)
{
	char	*stop;

	(void)strtol(s, &stop, 10);
	*end = stop;
	return (stop != s);
}

/* 1 when token is a well-formed corner, with its position index in *index */
static int	parse_corner(const char *token, long *index)
{
	char		*stop;
	const char	*s;

	errno = 0;
	*index = strtol(token, &stop, 10);
	if (stop == token || errno == ERANGE)
		return (0);
	if (*stop == '\0')
		return (1);
	if (*stop != '/')
		return (0);
	s = stop + 1;
	if (*s != '/')
	{
		if (!parse_optional_index(s, &s))
			return (0);
		if (*s == '\0')
			return (1);
		if (*s != '/')
			return (0);
	}
	return (parse_optional_index(s + 1, &s) && *s == '\0');
}

/* 0-based: 1..n from the start of the file, -1..-seen back from the line */
static int	resolve(const t_obj *obj, const t_obj_face *face, long index,
		unsigned int *out)
{
	size_t	back;

	if (index > 0 && (unsigned long)index <= obj->position_count)
	{
		*out = (unsigned int)(index - 1);
		return (1);
	}
	if (index >= 0)
		return (0);
	back = (size_t)(-(index + 1)) + 1;
	if (back > face->seen)
		return (0);
	*out = (unsigned int)(face->seen - back);
	return (1);
}

/* Appends a corner: 1, or 0 malformed, -1 no such vertex, -2 out of memory */
static int	add_corner(t_obj *obj, t_obj_parser *p, const t_obj_face *face,
		const char *token)
{
	unsigned int	*grown;
	long			index;

	grown = array_reserve(p->polygon, &p->polygon_cap, p->polygon_count + 1,
			sizeof(unsigned int));
	if (!grown)
		return (-2);
	p->polygon = grown;
	if (!parse_corner(token, &index))
		return (0);
	if (!resolve(obj, face, index, &p->polygon[p->polygon_count]))
		return (-1);
	p->polygon_count++;
	return (1);
}

int	obj_parse_face(t_obj *obj, t_obj_parser *p, const t_obj_face *face,
		unsigned int id)
{
	char	*cursor;
	char	*token;
	int		status;

	cursor = face->text;
	p->polygon_count = 0;
	token = obj_token(&cursor);
	while (token)
	{
		status = add_corner(obj, p, face, token);
		if (status == 0)
			return (obj_error(p, face->line, "invalid face corner", token));
		if (status == -1)
			return (obj_error(p, face->line, "vertex index out of range",
					token));
		if (status < 0)
			return (0);
		token = obj_token(&cursor);
	}
	if (p->polygon_count < 3)
		return (obj_error(p, face->line, "a face needs 3 vertices", NULL));
	return (obj_triangulate(obj, p, p->polygon_count, id));
}
