#ifndef RENDER_H
# define RENDER_H

# include <stddef.h>		/* size_t */
# include "gl_loader.h"		/* GLuint, GLsizei */
# include "vecmath.h"		/* t_mat4 */
# include "obj.h"			/* t_obj, the source of a mesh */

# define SHADER_LOG_SIZE	1024	/* bytes kept from a compile or link error log */

/* ---- Mesh vertex: one per triangle corner, attribute locations as in shaders/mesh.vert ---- */
# define MESH_ATTR_POSITION	0			/* xyz inside the unit sphere around the model's center */
# define MESH_ATTR_NORMAL	1			/* xyz of the face normal */
# define MESH_ATTR_SHADE	2			/* gray level of the .obj face, in [0, 1) */
# define MESH_VERTEX_FLOATS	7
# define MESH_SHADE_STEP	0.6180339887	/* golden ratio: consecutive faces get well-spread shades */

/* Geometry in GPU memory: vbo = the vertices, vao = how the vertex shader reads them. */
typedef struct s_mesh
{
	GLuint	vao;
	GLuint	vbo;
	GLsizei	vertex_count;
}	t_mesh;

/* ---- src/gl/shader.c ---- */
GLuint	shader_load(const char *vert_path, const char *frag_path);
void	shader_set_mat4(GLuint program, const char *name, t_mat4 value);
void	shader_set_float(GLuint program, const char *name, float value);
void	shader_set_int(GLuint program, const char *name, int value);

/* ---- src/gl/mesh.c ---- */
int		mesh_build(t_mesh *mesh, const t_obj *obj);
void	mesh_draw(const t_mesh *mesh);
void	mesh_destroy(t_mesh *mesh);

#endif
