#ifndef RENDER_H
# define RENDER_H

# include <stddef.h>		/* size_t */
# include "gl_loader.h"		/* GLuint, GLsizei */
# include "vecmath.h"		/* t_mat4 */

# define SHADER_LOG_SIZE	1024	/* bytes kept from a compile or link error log */

/* Geometry in GPU memory: vbo = vertex positions, ebo = triangle indices, vao = how the vertex shader reads the vbo. */
typedef struct s_mesh
{
	GLuint	vao;
	GLuint	vbo;
	GLuint	ebo;
	GLsizei	index_count;
}	t_mesh;

/* ---- src/gl/shader.c ---- */
GLuint	shader_load(const char *vert_path, const char *frag_path);
void	shader_set_mat4(GLuint program, const char *name, t_mat4 value);
void	shader_set_float(GLuint program, const char *name, float value);
void	shader_set_int(GLuint program, const char *name, int value);

/* ---- src/gl/mesh.c ---- */
t_mesh	mesh_upload(const float *positions, size_t vertex_count, const GLuint *indices, size_t index_count);
t_mesh	mesh_cube(void);
void	mesh_draw(const t_mesh *mesh);
void	mesh_destroy(t_mesh *mesh);

#endif
