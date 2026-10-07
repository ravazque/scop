/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_load_programs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Shader, program, uniform and texture entry points */

void	gl_load_programs(t_gl *g, int *ok)
{
	gl_load_proc(&g->create_shader, "glCreateShader", ok);
	gl_load_proc(&g->shader_source, "glShaderSource", ok);
	gl_load_proc(&g->compile_shader, "glCompileShader", ok);
	gl_load_proc(&g->get_shader_iv, "glGetShaderiv", ok);
	gl_load_proc(&g->get_shader_log, "glGetShaderInfoLog", ok);
	gl_load_proc(&g->delete_shader, "glDeleteShader", ok);
	gl_load_proc(&g->create_program, "glCreateProgram", ok);
	gl_load_proc(&g->attach_shader, "glAttachShader", ok);
	gl_load_proc(&g->link_program, "glLinkProgram", ok);
	gl_load_proc(&g->get_program_iv, "glGetProgramiv", ok);
	gl_load_proc(&g->get_program_log, "glGetProgramInfoLog", ok);
	gl_load_proc(&g->delete_program, "glDeleteProgram", ok);
	gl_load_proc(&g->use_program, "glUseProgram", ok);
	gl_load_proc(&g->uniform_location, "glGetUniformLocation", ok);
}

void	gl_load_textures(t_gl *g, int *ok)
{
	gl_load_proc(&g->uniform_matrix4fv, "glUniformMatrix4fv", ok);
	gl_load_proc(&g->uniform1f, "glUniform1f", ok);
	gl_load_proc(&g->uniform2f, "glUniform2f", ok);
	gl_load_proc(&g->uniform1i, "glUniform1i", ok);
	gl_load_proc(&g->gen_textures, "glGenTextures", ok);
	gl_load_proc(&g->bind_texture, "glBindTexture", ok);
	gl_load_proc(&g->active_texture, "glActiveTexture", ok);
	gl_load_proc(&g->tex_image_2d, "glTexImage2D", ok);
	gl_load_proc(&g->tex_parameteri, "glTexParameteri", ok);
	gl_load_proc(&g->generate_mipmap, "glGenerateMipmap", ok);
	gl_load_proc(&g->delete_textures, "glDeleteTextures", ok);
}
