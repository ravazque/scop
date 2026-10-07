#include "scop.h"

/* f lines: corners written v, v/vt, v//vn or v/vt/vn. Only the position index is used; the others are checked for form. */

static int	parse_optional_index(const char *s, const char **end)
{
	char	*stop;

	(void)strtol(s, &stop, 10);
	*end = stop;
	return (stop != s);
}

/* 1 when token is a well-formed corner, with its position index in *index. */
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

/* 0-based position: 1..n count from the start of the file, -1..-seen count back from the face's own line. */
static int	resolve(const t_obj *obj, const t_obj_face *face, long index, unsigned int *out)
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

int	obj_parse_face(t_obj *obj, t_obj_parser *p, const t_obj_face *face, unsigned int id)
{
	char			*cursor;
	char			*token;
	long			index;
	unsigned int	*grown;
	size_t			n;

	cursor = face->text;
	n = 0;
	token = obj_token(&cursor);
	while (token)
	{
		if (!parse_corner(token, &index))
			return (obj_error(p, face->line, "invalid face corner", token));
		grown = array_reserve(p->polygon, &p->polygon_cap, n + 1, sizeof(unsigned int));
		if (!grown)
			return (0);
		p->polygon = grown;
		if (!resolve(obj, face, index, &p->polygon[n]))
			return (obj_error(p, face->line, "vertex index out of range", token));
		n++;
		token = obj_token(&cursor);
	}
	if (n < 3)
		return (obj_error(p, face->line, "a face needs at least 3 vertices", NULL));
	return (obj_triangulate(obj, p, n, id));
}
