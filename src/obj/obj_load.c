#include "scop.h"

/* .obj loading in two passes: the v lines first, so a face may name any vertex of the file, then the f lines kept on the way. Other keywords are ignored. */

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

/* Next blank-separated token of a line, NUL-terminated in place; NULL at the end of the line. */
char	*obj_token(char **cursor)
{
	char	*s;
	char	*start;

	s = *cursor;
	while (is_blank(*s))
		s++;
	if (*s == '\0')
	{
		*cursor = s;
		return (NULL);
	}
	start = s;
	while (*s && !is_blank(*s))
		s++;
	if (*s)
		*s++ = '\0';
	*cursor = s;
	return (start);
}

/* 0 always, after "Error: path:line: message 'token'" (line 0 and a NULL token are left out). */
int	obj_error(const t_obj_parser *p, size_t line, const char *message, const char *token)
{
	fprintf(stderr, "Error: %s", p->path);
	if (line)
		fprintf(stderr, ":%zu", line);
	fprintf(stderr, ": %s", message);
	if (token)
		fprintf(stderr, " '%s'", token);
	fprintf(stderr, "\n");
	return (0);
}

static int	parse_vertex(t_obj *obj, t_obj_parser *p, char *cursor, size_t line)
{
	float	xyz[3];
	char	*token;
	char	*end;
	t_vec3	*grown;
	int		i;

	i = 0;
	while (i < 3)
	{
		token = obj_token(&cursor);
		if (!token)
			return (obj_error(p, line, "a vertex needs x, y and z", NULL));
		xyz[i] = strtof(token, &end);
		if (end == token || *end || !isfinite(xyz[i]))
			return (obj_error(p, line, "invalid vertex coordinate", token));
		i++;
	}
	if (obj->position_count >= UINT_MAX)
		return (obj_error(p, line, "too many vertices", NULL));
	grown = array_reserve(obj->positions, &obj->position_cap, obj->position_count + 1, sizeof(t_vec3));
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
	grown = array_reserve(p->faces, &p->face_cap, p->face_count + 1, sizeof(t_obj_face));
	if (!grown)
		return (0);
	p->faces = grown;
	p->faces[p->face_count++] = (t_obj_face){cursor, line, seen};
	return (1);
}

/* A backslash at the end of a line joins it with the next one. */
static void	join_continued_lines(char *s)
{
	while (*s)
	{
		if (s[0] == '\\' && s[1] == '\n')
			memset(s, ' ', 2);
		else if (s[0] == '\\' && s[1] == '\r' && s[2] == '\n')
			memset(s, ' ', 3);
		s++;
	}
}

static int	first_pass(t_obj *obj, t_obj_parser *p, char *line)
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
		next = strchr(line, '\n');
		if (next)
			*next++ = '\0';
		else
			next = line + strlen(line);
		cursor = strchr(line, '#');
		if (cursor)
			*cursor = '\0';
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

/* Bounding-box diagonal of every vertex: what "degenerate" is measured against. */
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

/* 1 with every face triangulated in obj; 0 after an error message naming the file and line. */
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
	join_continued_lines(buf);
	ok = first_pass(obj, &p, buf) && second_pass(obj, &p);
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
