/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_loader.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* OpenGL entry points, resolved by name through GLFW */

t_gl	*gl(void)
{
	static t_gl	table;

	return (&table);
}

/* Function pointers share one representation, so the address is copied as is */
void	gl_load_proc(void *slot, const char *name, int *ok)
{
	GLFWglproc	proc;

	proc = glfwGetProcAddress(name);
	if (!proc)
	{
		fprintf(stderr, "Error: OpenGL function %s not found\n", name);
		*ok = 0;
	}
	memcpy(slot, &proc, sizeof(proc));
}

static void	load_state(t_gl *g, int *ok)
{
	gl_load_proc(&g->viewport, "glViewport", ok);
	gl_load_proc(&g->clear_color, "glClearColor", ok);
	gl_load_proc(&g->clear, "glClear", ok);
	gl_load_proc(&g->enable, "glEnable", ok);
	gl_load_proc(&g->polygon_mode, "glPolygonMode", ok);
	gl_load_proc(&g->point_size, "glPointSize", ok);
	gl_load_proc(&g->draw_arrays, "glDrawArrays", ok);
}

static void	load_buffers(t_gl *g, int *ok)
{
	gl_load_proc(&g->gen_vertex_arrays, "glGenVertexArrays", ok);
	gl_load_proc(&g->bind_vertex_array, "glBindVertexArray", ok);
	gl_load_proc(&g->delete_vertex_arrays, "glDeleteVertexArrays", ok);
	gl_load_proc(&g->gen_buffers, "glGenBuffers", ok);
	gl_load_proc(&g->bind_buffer, "glBindBuffer", ok);
	gl_load_proc(&g->buffer_data, "glBufferData", ok);
	gl_load_proc(&g->delete_buffers, "glDeleteBuffers", ok);
	gl_load_proc(&g->enable_attrib, "glEnableVertexAttribArray", ok);
	gl_load_proc(&g->attrib_pointer, "glVertexAttribPointer", ok);
}

/* Needs a current context; reports every missing function */
int	gl_load(void)
{
	int	ok;

	ok = 1;
	load_state(gl(), &ok);
	load_buffers(gl(), &ok);
	gl_load_programs(gl(), &ok);
	gl_load_textures(gl(), &ok);
	return (ok);
}
