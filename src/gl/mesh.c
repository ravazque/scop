/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Triangles on the GPU: one vertex buffer and how the shader reads it */

static void	attribute(unsigned int location, int size, size_t offset)
{
	const t_gl	*g = gl();
	const int	stride = MESH_VERTEX_FLOATS * sizeof(float);

	g->enable_attrib(location);
	g->attrib_pointer(location, size, GL_FLOAT, GL_FALSE, stride,
		(void *)(uintptr_t)(offset * sizeof(float)));
}

static void	upload(t_mesh *mesh, const float *vertices, size_t count)
{
	const t_gl		*g = gl();
	const size_t	bytes = count * MESH_VERTEX_FLOATS * sizeof(float);

	mesh->vertex_count = (int)count;
	g->gen_vertex_arrays(1, &mesh->vao);
	g->gen_buffers(1, &mesh->vbo);
	g->bind_vertex_array(mesh->vao);
	g->bind_buffer(GL_ARRAY_BUFFER, mesh->vbo);
	g->buffer_data(GL_ARRAY_BUFFER, (ptrdiff_t)bytes, vertices, GL_STATIC_DRAW);
	attribute(MESH_ATTR_POSITION, 3, 0);
	attribute(MESH_ATTR_NORMAL, 3, 3);
	attribute(MESH_ATTR_SHADE, 1, 6);
	g->bind_vertex_array(0);
}

/* 0 with a message when memory runs out; obj has at least one triangle */
int	mesh_build(t_mesh *mesh, const t_obj *obj)
{
	const size_t	count = obj->triangle_count * 3;
	float			*vertices;

	memset(mesh, 0, sizeof(*mesh));
	vertices = malloc(count * MESH_VERTEX_FLOATS * sizeof(float));
	if (!vertices)
		return (fprintf(stderr, "Error: out of memory\n"), 0);
	mesh_fill_vertices(vertices, obj);
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

/* glDelete* ignore the name 0, so a mesh never uploaded is safe here */
void	mesh_destroy(t_mesh *mesh)
{
	const t_gl	*g = gl();

	g->delete_buffers(1, &mesh->vbo);
	g->delete_vertex_arrays(1, &mesh->vao);
	memset(mesh, 0, sizeof(*mesh));
}
