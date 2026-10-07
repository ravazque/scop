/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* GLSL programs from two files, with the driver's log when they fail */

/* 0 when the stage does not compile, after printing the driver's log */
static unsigned int	check_stage(unsigned int shader, const char *path)
{
	const t_gl	*g = gl();
	int			ok;
	char		log[SHADER_LOG_SIZE];

	g->get_shader_iv(shader, GL_COMPILE_STATUS, &ok);
	if (ok)
		return (shader);
	g->get_shader_log(shader, SHADER_LOG_SIZE, NULL, log);
	fprintf(stderr, "Error: cannot compile %s:\n%s\n", path, log);
	g->delete_shader(shader);
	return (0);
}

static unsigned int	compile_stage(unsigned int type, const char *path)
{
	const t_gl		*g = gl();
	const char		*sources[1];
	unsigned int	shader;

	sources[0] = file_read(path, NULL);
	if (!sources[0])
		return (0);
	shader = g->create_shader(type);
	g->shader_source(shader, 1, sources, NULL);
	g->compile_shader(shader);
	free((char *)sources[0]);
	return (check_stage(shader, path));
}

static unsigned int	link_program(unsigned int vs, unsigned int fs)
{
	const t_gl		*g = gl();
	unsigned int	program;
	int				ok;
	char			log[SHADER_LOG_SIZE];

	program = g->create_program();
	g->attach_shader(program, vs);
	g->attach_shader(program, fs);
	g->link_program(program);
	g->get_program_iv(program, GL_LINK_STATUS, &ok);
	if (ok)
		return (program);
	g->get_program_log(program, SHADER_LOG_SIZE, NULL, log);
	fprintf(stderr, "Error: cannot link shader program:\n%s\n", log);
	g->delete_program(program);
	return (0);
}

/* 0 on failure; the stages are deleted once linked */
unsigned int	shader_load(const char *vert_path, const char *frag_path)
{
	unsigned int	vs;
	unsigned int	fs;
	unsigned int	program;

	vs = compile_stage(GL_VERTEX_SHADER, vert_path);
	fs = compile_stage(GL_FRAGMENT_SHADER, frag_path);
	program = 0;
	if (vs && fs)
		program = link_program(vs, fs);
	gl()->delete_shader(vs);
	gl()->delete_shader(fs);
	return (program);
}
