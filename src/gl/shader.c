#include "scop.h"

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
	const char		*source;
	unsigned int	shader;

	source = file_read(path, NULL);
	if (!source)
		return (0);
	shader = g->create_shader(type);
	g->shader_source(shader, 1, &source, NULL);
	g->compile_shader(shader);
	free((char *)source);
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

/* The linked program keeps its own copy of the stages, so they are deleted either way */
unsigned int	shader_load(const char *vert_path, const char *frag_path)
{
	const unsigned int	vs = compile_stage(GL_VERTEX_SHADER, vert_path);
	const unsigned int	fs = compile_stage(GL_FRAGMENT_SHADER, frag_path);
	unsigned int		program;

	program = 0;
	if (vs && fs)
		program = link_program(vs, fs);
	gl()->delete_shader(vs);
	gl()->delete_shader(fs);
	return (program);
}

void	shader_set_mat4(unsigned int program, const char *name, t_mat4 value)
{
	gl()->uniform_matrix4fv(gl()->uniform_location(program, name), 1, GL_FALSE, value.m);
}

void	shader_set_float(unsigned int program, const char *name, float value)
{
	gl()->uniform1f(gl()->uniform_location(program, name), value);
}

void	shader_set_vec2(unsigned int program, const char *name, float x, float y)
{
	gl()->uniform2f(gl()->uniform_location(program, name), x, y);
}

void	shader_set_int(unsigned int program, const char *name, int value)
{
	gl()->uniform1i(gl()->uniform_location(program, name), value);
}
