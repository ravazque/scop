/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_loader.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GL_LOADER_H
# define GL_LOADER_H

# include <stddef.h>		/* ptrdiff_t, the size type of glBufferData */

# define GL_FALSE					0
# define GL_TRUE					1
# define GL_TRIANGLES				0x0004
# define GL_DEPTH_BUFFER_BIT		0x00000100
# define GL_COLOR_BUFFER_BIT		0x00004000
# define GL_FRONT_AND_BACK			0x0408
# define GL_DEPTH_TEST				0x0B71
# define GL_TEXTURE_2D				0x0DE1
# define GL_UNSIGNED_BYTE			0x1401
# define GL_FLOAT					0x1406
# define GL_RGBA					0x1908
# define GL_POINT					0x1B00
# define GL_LINE					0x1B01
# define GL_FILL					0x1B02
# define GL_LINEAR					0x2601
# define GL_LINEAR_MIPMAP_LINEAR	0x2703
# define GL_TEXTURE_MAG_FILTER		0x2800
# define GL_TEXTURE_MIN_FILTER		0x2801
# define GL_TEXTURE_WRAP_S			0x2802
# define GL_TEXTURE_WRAP_T			0x2803
# define GL_REPEAT					0x2901
# define GL_RGBA8					0x8058
# define GL_TEXTURE0				0x84C0
# define GL_ARRAY_BUFFER			0x8892
# define GL_STATIC_DRAW				0x88E4
# define GL_FRAGMENT_SHADER			0x8B30
# define GL_VERTEX_SHADER			0x8B31
# define GL_COMPILE_STATUS			0x8B81
# define GL_LINK_STATUS				0x8B82

/* Entry points, with the GL types written as plain C types */
typedef void			(*t_gl_viewport)(int x, int y, int width, int height);
typedef void			(*t_gl_clear_color)(float r, float g, float b, float a);
typedef void			(*t_gl_clear)(unsigned int mask);
typedef void			(*t_gl_enable)(unsigned int cap);
typedef void			(*t_gl_polygon_mode)(unsigned int face,
	unsigned int mode);
typedef void			(*t_gl_point_size)(float size);
typedef void			(*t_gl_draw_arrays)(unsigned int mode, int first,
	int count);
typedef void			(*t_gl_gen_vertex_arrays)(int n, unsigned int *arrays);
typedef void			(*t_gl_bind_vertex_array)(unsigned int array);
typedef void			(*t_gl_delete_vertex_arrays)(int n,
	const unsigned int *arrays);
typedef void			(*t_gl_gen_buffers)(int n, unsigned int *buffers);
typedef void			(*t_gl_bind_buffer)(unsigned int target,
	unsigned int buffer);
typedef void			(*t_gl_buffer_data)(unsigned int target, ptrdiff_t size,
	const void *data, unsigned int usage);
typedef void			(*t_gl_delete_buffers)(int n,
	const unsigned int *buffers);
typedef void			(*t_gl_enable_attrib)(unsigned int index);
typedef void			(*t_gl_attrib_pointer)(unsigned int index, int size,
	unsigned int type, unsigned char normalized, int stride,
	const void *pointer);
typedef unsigned int	(*t_gl_create_shader)(unsigned int type);
typedef void			(*t_gl_shader_source)(unsigned int shader, int count,
	const char *const *string, const int *length);
typedef void			(*t_gl_compile_shader)(unsigned int shader);
typedef void			(*t_gl_get_shader_iv)(unsigned int shader,
	unsigned int pname, int *params);
typedef void			(*t_gl_get_shader_log)(unsigned int shader, int size,
	int *length, char *log);
typedef void			(*t_gl_delete_shader)(unsigned int shader);
typedef unsigned int	(*t_gl_create_program)(void);
typedef void			(*t_gl_attach_shader)(unsigned int program,
	unsigned int shader);
typedef void			(*t_gl_link_program)(unsigned int program);
typedef void			(*t_gl_get_program_iv)(unsigned int program,
	unsigned int pname, int *params);
typedef void			(*t_gl_get_program_log)(unsigned int program, int size,
	int *length, char *log);
typedef void			(*t_gl_delete_program)(unsigned int program);
typedef void			(*t_gl_use_program)(unsigned int program);
typedef int				(*t_gl_uniform_location)(unsigned int program,
	const char *name);
typedef void			(*t_gl_uniform_matrix4fv)(int location, int count,
	unsigned char transpose, const float *value);
typedef void			(*t_gl_uniform1f)(int location, float v0);
typedef void			(*t_gl_uniform2f)(int location, float v0, float v1);
typedef void			(*t_gl_uniform1i)(int location, int v0);
typedef void			(*t_gl_gen_textures)(int n, unsigned int *textures);
typedef void			(*t_gl_bind_texture)(unsigned int target,
	unsigned int texture);
typedef void			(*t_gl_active_texture)(unsigned int texture);
typedef void			(*t_gl_tex_image_2d)(unsigned int target, int level,
	int internalformat, int width, int height, int border, unsigned int format,
	unsigned int type, const void *pixels);
typedef void			(*t_gl_tex_parameteri)(unsigned int target,
	unsigned int pname, int param);
typedef void			(*t_gl_generate_mipmap)(unsigned int target);
typedef void			(*t_gl_delete_textures)(int n,
	const unsigned int *textures);

/* Every OpenGL function scop calls, filled once with glfwGetProcAddress */
typedef struct s_gl
{
	t_gl_viewport				viewport;
	t_gl_clear_color			clear_color;
	t_gl_clear					clear;
	t_gl_enable					enable;
	t_gl_polygon_mode			polygon_mode;
	t_gl_point_size				point_size;
	t_gl_draw_arrays			draw_arrays;
	t_gl_gen_vertex_arrays		gen_vertex_arrays;
	t_gl_bind_vertex_array		bind_vertex_array;
	t_gl_delete_vertex_arrays	delete_vertex_arrays;
	t_gl_gen_buffers			gen_buffers;
	t_gl_bind_buffer			bind_buffer;
	t_gl_buffer_data			buffer_data;
	t_gl_delete_buffers			delete_buffers;
	t_gl_enable_attrib			enable_attrib;
	t_gl_attrib_pointer			attrib_pointer;
	t_gl_create_shader			create_shader;
	t_gl_shader_source			shader_source;
	t_gl_compile_shader			compile_shader;
	t_gl_get_shader_iv			get_shader_iv;
	t_gl_get_shader_log			get_shader_log;
	t_gl_delete_shader			delete_shader;
	t_gl_create_program			create_program;
	t_gl_attach_shader			attach_shader;
	t_gl_link_program			link_program;
	t_gl_get_program_iv			get_program_iv;
	t_gl_get_program_log		get_program_log;
	t_gl_delete_program			delete_program;
	t_gl_use_program			use_program;
	t_gl_uniform_location		uniform_location;
	t_gl_uniform_matrix4fv		uniform_matrix4fv;
	t_gl_uniform1f				uniform1f;
	t_gl_uniform2f				uniform2f;
	t_gl_uniform1i				uniform1i;
	t_gl_gen_textures			gen_textures;
	t_gl_bind_texture			bind_texture;
	t_gl_active_texture			active_texture;
	t_gl_tex_image_2d			tex_image_2d;
	t_gl_tex_parameteri			tex_parameteri;
	t_gl_generate_mipmap		generate_mipmap;
	t_gl_delete_textures		delete_textures;
}	t_gl;

/* ---- src/gl/gl_loader.c ---- */
t_gl	*gl(void);
void	gl_load_proc(void *slot, const char *name, int *ok);
int		gl_load(void);

/* ---- src/gl/gl_load_programs.c ---- */
void	gl_load_programs(t_gl *g, int *ok);
void	gl_load_textures(t_gl *g, int *ok);

#endif
