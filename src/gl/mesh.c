#include "scop.h"

/* Triangles on the GPU: centered on the model's bounding box and scaled into the unit sphere, so any model turns around its center and fits the view. */

/* Center and scale from the corners actually drawn, ignoring unused vertices. */
static void	normalization(const t_obj *obj, t_vec3 *center, float *scale)
{
	t_vec3	lo;
	t_vec3	hi;
	size_t	i;
	int		k;

	lo = obj->positions[obj->triangles[0].corner[0]];
	hi = lo;
	i = 0;
	while (i < obj->triangle_count)
	{
		k = 0;
		while (k < 3)
		{
			lo = vec3_min(lo, obj->positions[obj->triangles[i].corner[k]]);
			hi = vec3_max(hi, obj->positions[obj->triangles[i].corner[k]]);
			k++;
		}
		i++;
	}
	*center = vec3_scale(vec3_add(lo, hi), 0.5f);
	*scale = 2.0f / vec3_length(vec3_sub(hi, lo));
}

/* The three corners of a triangle: position, face normal and the shade of its .obj face. */
static void	write_triangle(float *out, const t_obj *obj, const t_obj_triangle *t, t_vec3 center, float scale)
{
	t_vec3	p[3];
	t_vec3	n;
	float	shade;
	int		k;

	k = 0;
	while (k < 3)
	{
		p[k] = vec3_scale(vec3_sub(obj->positions[t->corner[k]], center), scale);
		k++;
	}
	n = vec3_normalize(vec3_cross(vec3_sub(p[1], p[0]), vec3_sub(p[2], p[0])));
	shade = (float)fmod((double)t->face * MESH_SHADE_STEP, 1.0);
	k = 0;
	while (k < 3)
	{
		memcpy(out, (float [MESH_VERTEX_FLOATS]){p[k].x, p[k].y, p[k].z, n.x, n.y, n.z, shade}, sizeof(float) * MESH_VERTEX_FLOATS);
		out += MESH_VERTEX_FLOATS;
		k++;
	}
}

static void	attribute(GLuint location, GLint size, size_t offset)
{
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, MESH_VERTEX_FLOATS * sizeof(float), (void *)(uintptr_t)(offset * sizeof(float)));
}

static void	upload(t_mesh *mesh, const float *vertices, size_t count)
{
	mesh->vertex_count = (GLsizei)count;
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glBindVertexArray(mesh->vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(count * MESH_VERTEX_FLOATS * sizeof(float)), vertices, GL_STATIC_DRAW);
	attribute(MESH_ATTR_POSITION, 3, 0);
	attribute(MESH_ATTR_NORMAL, 3, 3);
	attribute(MESH_ATTR_SHADE, 1, 6);
	glBindVertexArray(0);
}

/* 0 with a message when memory runs out; obj needs at least one triangle (obj_load guarantees it). */
int	mesh_build(t_mesh *mesh, const t_obj *obj)
{
	const size_t	count = obj->triangle_count * 3;
	float			*vertices;
	t_vec3			center;
	float			scale;
	size_t			i;

	memset(mesh, 0, sizeof(*mesh));
	vertices = malloc(count * MESH_VERTEX_FLOATS * sizeof(float));
	if (!vertices)
		return (fprintf(stderr, "Error: out of memory\n"), 0);
	normalization(obj, &center, &scale);
	i = 0;
	while (i < obj->triangle_count)
	{
		write_triangle(vertices + i * 3 * MESH_VERTEX_FLOATS, obj, &obj->triangles[i], center, scale);
		i++;
	}
	upload(mesh, vertices, count);
	free(vertices);
	return (1);
}

void	mesh_draw(const t_mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	glBindVertexArray(0);
}

/* glDelete* ignore the name 0, so a mesh that was never uploaded is safe here. */
void	mesh_destroy(t_mesh *mesh)
{
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteVertexArrays(1, &mesh->vao);
	memset(mesh, 0, sizeof(*mesh));
}
