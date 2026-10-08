#include "scop.h"

/* Center and scale come from the corners actually drawn, not from unused vertices */
static t_fit	fit_model(const t_obj *obj)
{
	t_vec3	lo;
	t_vec3	hi;
	t_vec3	p;
	size_t	i;

	lo = obj->positions[obj->triangles[0].corner[0]];
	hi = lo;
	for (i = 0; i < obj->triangle_count * 3; i++)
	{
		p = obj->positions[obj->triangles[i / 3].corner[i % 3]];
		lo = vec3_min(lo, p);
		hi = vec3_max(hi, p);
	}
	return ((t_fit){vec3_scale(vec3_add(lo, hi), 0.5f), 2.0f / vec3_length(vec3_sub(hi, lo))});
}

static void	write_vertex(float *out, t_vec3 p, t_vec3 n, float shade)
{
	out[0] = p.x;
	out[1] = p.y;
	out[2] = p.z;
	out[3] = n.x;
	out[4] = n.y;
	out[5] = n.z;
	out[6] = shade;
}

/* A face's gray is its .obj index times the golden ratio, wrapped to [0, 1): neighbours never match */
static void	write_triangle(float *out, const t_obj *obj, const t_obj_triangle *t, const t_fit *fit)
{
	const float	shade = (float)fmod((double)t->face * MESH_SHADE_STEP, 1.0);
	t_vec3		p[3];
	t_vec3		n;
	int			k;

	for (k = 0; k < 3; k++)
		p[k] = vec3_scale(vec3_sub(obj->positions[t->corner[k]], fit->center), fit->scale);
	n = vec3_normalize(vec3_cross(vec3_sub(p[1], p[0]), vec3_sub(p[2], p[0])));
	for (k = 0; k < 3; k++)
		write_vertex(out + k * MESH_VERTEX_FLOATS, p[k], n, shade);
}

static void	attribute(unsigned int location, int size, size_t offset)
{
	const int	stride = MESH_VERTEX_FLOATS * sizeof(float);

	gl()->enable_attrib(location);
	gl()->attrib_pointer(location, size, GL_FLOAT, GL_FALSE, stride, (void *)(uintptr_t)(offset * sizeof(float)));
}

static void	upload(t_mesh *mesh, const float *vertices, size_t count)
{
	const t_gl	*g = gl();

	mesh->vertex_count = (int)count;
	g->gen_vertex_arrays(1, &mesh->vao);
	g->gen_buffers(1, &mesh->vbo);
	g->bind_vertex_array(mesh->vao);
	g->bind_buffer(GL_ARRAY_BUFFER, mesh->vbo);
	g->buffer_data(GL_ARRAY_BUFFER, (ptrdiff_t)(count * MESH_VERTEX_FLOATS * sizeof(float)), vertices, GL_STATIC_DRAW);
	attribute(MESH_ATTR_POSITION, 3, 0);
	attribute(MESH_ATTR_NORMAL, 3, 3);
	attribute(MESH_ATTR_SHADE, 1, 6);
	g->bind_vertex_array(0);
}

int	mesh_build(t_mesh *mesh, const t_obj *obj)
{
	const size_t	count = obj->triangle_count * 3;
	const t_fit		fit = fit_model(obj);
	float			*vertices;
	size_t			i;

	memset(mesh, 0, sizeof(*mesh));
	vertices = malloc(count * MESH_VERTEX_FLOATS * sizeof(float));
	if (!vertices)
		return (fprintf(stderr, "Error: out of memory\n"), 0);
	for (i = 0; i < obj->triangle_count; i++)
		write_triangle(vertices + i * 3 * MESH_VERTEX_FLOATS, obj, &obj->triangles[i], &fit);
	upload(mesh, vertices, count);
	free(vertices);
	return (1);
}

void	mesh_draw(const t_mesh *mesh)
{
	const t_gl	*g = gl();

	g->bind_vertex_array(mesh->vao);
	g->draw_arrays(GL_TRIANGLES, 0, mesh->vertex_count);
	g->bind_vertex_array(0);
}

void	mesh_destroy(t_mesh *mesh)
{
	gl()->delete_buffers(1, &mesh->vbo);
	gl()->delete_vertex_arrays(1, &mesh->vao);
	memset(mesh, 0, sizeof(*mesh));
}
