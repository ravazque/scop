#include "scop.h"

static const struct s_gl_proc
{
	size_t		offset;
	const char	*name;
}	g_procs[] = {
	{offsetof(t_gl, viewport), "glViewport"},
	{offsetof(t_gl, clear_color), "glClearColor"},
	{offsetof(t_gl, clear), "glClear"},
	{offsetof(t_gl, enable), "glEnable"},
	{offsetof(t_gl, polygon_mode), "glPolygonMode"},
	{offsetof(t_gl, point_size), "glPointSize"},
	{offsetof(t_gl, draw_arrays), "glDrawArrays"},
	{offsetof(t_gl, gen_vertex_arrays), "glGenVertexArrays"},
	{offsetof(t_gl, bind_vertex_array), "glBindVertexArray"},
	{offsetof(t_gl, delete_vertex_arrays), "glDeleteVertexArrays"},
	{offsetof(t_gl, gen_buffers), "glGenBuffers"},
	{offsetof(t_gl, bind_buffer), "glBindBuffer"},
	{offsetof(t_gl, buffer_data), "glBufferData"},
	{offsetof(t_gl, delete_buffers), "glDeleteBuffers"},
	{offsetof(t_gl, enable_attrib), "glEnableVertexAttribArray"},
	{offsetof(t_gl, attrib_pointer), "glVertexAttribPointer"},
	{offsetof(t_gl, create_shader), "glCreateShader"},
	{offsetof(t_gl, shader_source), "glShaderSource"},
	{offsetof(t_gl, compile_shader), "glCompileShader"},
	{offsetof(t_gl, get_shader_iv), "glGetShaderiv"},
	{offsetof(t_gl, get_shader_log), "glGetShaderInfoLog"},
	{offsetof(t_gl, delete_shader), "glDeleteShader"},
	{offsetof(t_gl, create_program), "glCreateProgram"},
	{offsetof(t_gl, attach_shader), "glAttachShader"},
	{offsetof(t_gl, link_program), "glLinkProgram"},
	{offsetof(t_gl, get_program_iv), "glGetProgramiv"},
	{offsetof(t_gl, get_program_log), "glGetProgramInfoLog"},
	{offsetof(t_gl, delete_program), "glDeleteProgram"},
	{offsetof(t_gl, use_program), "glUseProgram"},
	{offsetof(t_gl, uniform_location), "glGetUniformLocation"},
	{offsetof(t_gl, uniform_matrix4fv), "glUniformMatrix4fv"},
	{offsetof(t_gl, uniform1f), "glUniform1f"},
	{offsetof(t_gl, uniform2f), "glUniform2f"},
	{offsetof(t_gl, uniform1i), "glUniform1i"},
	{offsetof(t_gl, gen_textures), "glGenTextures"},
	{offsetof(t_gl, bind_texture), "glBindTexture"},
	{offsetof(t_gl, active_texture), "glActiveTexture"},
	{offsetof(t_gl, tex_image_2d), "glTexImage2D"},
	{offsetof(t_gl, tex_parameteri), "glTexParameteri"},
	{offsetof(t_gl, generate_mipmap), "glGenerateMipmap"},
	{offsetof(t_gl, delete_textures), "glDeleteTextures"},
};

t_gl	*gl(void)
{
	static t_gl	table;

	return (&table);
}

/* Needs a current context. Function pointers share one representation, so each address is copied as is */
int	gl_load(void)
{
	GLFWglproc	proc;
	size_t		i;
	int			ok;

	ok = 1;
	for (i = 0; i < sizeof(g_procs) / sizeof(g_procs[0]); i++)
	{
		proc = glfwGetProcAddress(g_procs[i].name);
		if (!proc)
		{
			fprintf(stderr, "Error: OpenGL function %s not found\n", g_procs[i].name);
			ok = 0;
		}
		memcpy((char *)gl() + g_procs[i].offset, &proc, sizeof(proc));
	}
	return (ok);
}
