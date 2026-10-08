#ifndef RENDER_H
#define RENDER_H

#include <stddef.h>
#include "vecmath.h"
#include "obj.h"
#include "image.h"

#define SHADER_LOG_SIZE		1024

/* One vertex per triangle corner: position, face normal and face shade, at the locations of mesh.vert */
#define MESH_ATTR_POSITION	0
#define MESH_ATTR_NORMAL	1
#define MESH_ATTR_SHADE		2
#define MESH_VERTEX_FLOATS	7
#define MESH_SHADE_STEP		0.6180339887

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

unsigned int	shader_load(const char *vert_path, const char *frag_path);
void			shader_set_mat4(unsigned int program, const char *name, t_mat4 value);
void			shader_set_float(unsigned int program, const char *name, float value);
void			shader_set_vec2(unsigned int program, const char *name, float x, float y);
void			shader_set_int(unsigned int program, const char *name, int value);

int				mesh_build(t_mesh *mesh, const t_obj *obj);
void			mesh_draw(const t_mesh *mesh);
void			mesh_destroy(t_mesh *mesh);

unsigned int	texture_upload(const t_image *image);

#endif
