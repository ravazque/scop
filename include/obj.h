#ifndef OBJ_H
#define OBJ_H

#include <stddef.h>
#include "vecmath.h"

/* Triangles whose doubled area is below this, times the squared model size, are dropped */
#define OBJ_DEGENERATE_EPSILON	1e-10f

typedef struct s_obj_triangle
{
	unsigned int	corner[3];
	unsigned int	face;
}	t_obj_triangle;

typedef struct s_obj
{
	t_vec3			*positions;
	size_t			position_count;
	size_t			position_cap;
	t_obj_triangle	*triangles;
	size_t			triangle_count;
	size_t			triangle_cap;
	size_t			face_count;
	float			min_area;
}	t_obj;

/* An f line kept by the first pass; seen is how many v lines precede it, for negative indices */
typedef struct s_obj_face
{
	char	*text;
	size_t	line;
	size_t	seen;
}	t_obj_face;

typedef struct s_obj_parser
{
	const char		*path;
	t_obj_face		*faces;
	size_t			face_count;
	size_t			face_cap;
	unsigned int	*polygon;
	size_t			polygon_count;
	size_t			polygon_cap;
	size_t			*ring;
	size_t			ring_cap;
	float			*flat;
	size_t			flat_cap;
}	t_obj_parser;

int		obj_load(const char *path, t_obj *obj);
void	obj_free(t_obj *obj);
char	*obj_token(char **cursor);
int		obj_error(const t_obj_parser *p, size_t line, const char *message, const char *token);
int		obj_parse_face(t_obj *obj, t_obj_parser *p, const t_obj_face *face, unsigned int id);
int		obj_triangulate(t_obj *obj, t_obj_parser *p, size_t n, unsigned int id);

#endif
