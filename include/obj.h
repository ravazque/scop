/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_H
# define OBJ_H

# include <stddef.h>		/* size_t */
# include "vecmath.h"		/* t_vec3 */

/* Drop triangles under this doubled area, relative to the model size squared */
# define OBJ_DEGENERATE_EPSILON	1e-10f

/* Three position indices, in the winding of the .obj face (f line) */
typedef struct s_obj_triangle
{
	unsigned int	corner[3];
	unsigned int	face;
}	t_obj_triangle;

/* A .obj reduced to what scop draws: positions and triangles */
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

/* An f line kept by the first pass, and how many v lines came before it */
typedef struct s_obj_face
{
	char	*text;
	size_t	line;
	size_t	seen;
}	t_obj_face;

/* Parser state: the f lines to resolve and scratch buffers for each face */
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

/* ---- src/obj/obj_load.c ---- */
int		obj_load(const char *path, t_obj *obj);
void	obj_free(t_obj *obj);

/* ---- src/obj/obj_lines.c ---- */
int		obj_first_pass(t_obj *obj, t_obj_parser *p, char *line);

/* ---- src/obj/obj_token.c ---- */
char	*obj_token(char **cursor);
int		obj_error(const t_obj_parser *p, size_t line, const char *message,
			const char *token);
void	obj_join_lines(char *s);

/* ---- src/obj/obj_face.c ---- */
int		obj_parse_face(t_obj *obj, t_obj_parser *p, const t_obj_face *face,
			unsigned int id);

/* ---- src/obj/obj_triangulate.c ---- */
int		obj_triangulate(t_obj *obj, t_obj_parser *p, size_t n, unsigned int id);
int		obj_emit(t_obj *obj, const unsigned int *abc, unsigned int id);
int		obj_fan(t_obj *obj, const t_obj_parser *p, size_t count,
			unsigned int id);

/* ---- src/obj/obj_project.c ---- */
t_vec3	obj_newell_normal(const t_obj *obj, const unsigned int *poly, size_t n);
void	obj_project(const t_obj *obj, t_obj_parser *p, size_t n, t_vec3 normal);

/* ---- src/obj/obj_ear.c ---- */
int		obj_ear_clip(t_obj *obj, t_obj_parser *p, size_t count,
			unsigned int id);

#endif
