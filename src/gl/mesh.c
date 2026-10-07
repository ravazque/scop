#include "scop.h"

/* Indexed triangle meshes on the GPU: positions (attribute 0) in a VBO, indices in an EBO. */

t_mesh	mesh_upload(const float *positions, size_t vertex_count, const GLuint *indices, size_t index_count)
{
	t_mesh	mesh;

	mesh.index_count = (GLsizei)index_count;
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(1, &mesh.vbo);
	glGenBuffers(1, &mesh.ebo);
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertex_count * 3 * sizeof(float)), positions, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(index_count * sizeof(GLuint)), indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glBindVertexArray(0);
	return (mesh);
}

/* Unit cube centered on the origin, counter-clockwise triangles seen from outside. */
t_mesh	mesh_cube(void)
{
	const float		h = 0.5f;
	const float		positions[] = {
		-h, -h, -h, h, -h, -h, h, h, -h, -h, h, -h,
		-h, -h, h, h, -h, h, h, h, h, -h, h, h};
	const GLuint	indices[] = {
		4, 5, 6, 4, 6, 7,
		1, 0, 3, 1, 3, 2,
		5, 1, 2, 5, 2, 6,
		0, 4, 7, 0, 7, 3,
		7, 6, 2, 7, 2, 3,
		0, 1, 5, 0, 5, 4};

	return (mesh_upload(positions, 8, indices, 36));
}

void	mesh_draw(const t_mesh *mesh)
{
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

/* glDelete* ignore the name 0, so a mesh that was never uploaded is safe here. */
void	mesh_destroy(t_mesh *mesh)
{
	glDeleteBuffers(1, &mesh->ebo);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteVertexArrays(1, &mesh->vao);
	memset(mesh, 0, sizeof(*mesh));
}
