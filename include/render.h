/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <stddef.h>		/* size_t */
# include "vecmath.h"		/* t_vec3, t_mat4 */
# include "obj.h"			/* t_obj, the source of a mesh */
# include "image.h"			/* t_image, the source of a texture */

# define SHADER_LOG_SIZE	1024	/* bytes kept from a compile or link log */

/* ---- Mesh vertex: one per triangle corner, locations as in mesh.vert ---- */
# define MESH_ATTR_POSITION	0		/* xyz inside the unit sphere */
# define MESH_ATTR_NORMAL	1		/* xyz of the face normal */
# define MESH_ATTR_SHADE	2		/* gray level of the .obj face, [0, 1) */
# define MESH_VERTEX_FLOATS	7
# define MESH_SHADE_STEP	0.6180339887	/* golden ratio: spread shades */

/* Geometry in GPU memory: vbo = the vertices, vao = how they are read */
typedef struct s_mesh
{
	unsigned int	vao;
	unsigned int	vbo;
	int				vertex_count;
}	t_mesh;

/* Moves a model into the unit sphere: p' = (p - center) * scale */
typedef struct s_fit
{
	t_vec3	center;
	float	scale;
}	t_fit;

/* ---- src/gl/shader.c ---- */
unsigned int	shader_load(const char *vert_path, const char *frag_path);

/* ---- src/gl/shader_uniform.c ---- */
void			shader_set_mat4(unsigned int program, const char *name,
					t_mat4 value);
void			shader_set_float(unsigned int program, const char *name,
					float value);
void			shader_set_vec2(unsigned int program, const char *name,
					float x, float y);
void			shader_set_int(unsigned int program, const char *name,
					int value);

/* ---- src/gl/mesh.c ---- */
int				mesh_build(t_mesh *mesh, const t_obj *obj);
void			mesh_draw(const t_mesh *mesh);
void			mesh_destroy(t_mesh *mesh);

/* ---- src/gl/mesh_vertices.c ---- */
void			mesh_fill_vertices(float *vertices, const t_obj *obj);

/* ---- src/gl/texture.c ---- */
unsigned int	texture_upload(const t_image *image);

#endif
