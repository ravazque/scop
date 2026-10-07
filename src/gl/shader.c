#include "scop.h"

/* Loads, compiles and links a GLSL program from two files, and sets its uniforms by name. */

static GLuint	compile_stage(GLenum type, const char *path)
{
	char		*src;
	const char	*sources[1];
	GLuint		shader;
	GLint		ok;
	char		log[SHADER_LOG_SIZE];

	src = file_read(path, NULL);
	if (!src)
		return (0);
	sources[0] = src;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, sources, NULL);
	glCompileShader(shader);
	free(src);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (ok)
		return (shader);
	glGetShaderInfoLog(shader, SHADER_LOG_SIZE, NULL, log);
	fprintf(stderr, "Error: cannot compile %s:\n%s\n", path, log);
	glDeleteShader(shader);
	return (0);
}

static GLuint	link_program(GLuint vs, GLuint fs)
{
	GLuint	program;
	GLint	ok;
	char	log[SHADER_LOG_SIZE];

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	if (ok)
		return (program);
	glGetProgramInfoLog(program, SHADER_LOG_SIZE, NULL, log);
	fprintf(stderr, "Error: cannot link shader program:\n%s\n", log);
	glDeleteProgram(program);
	return (0);
}

/* 0 on failure, with the driver's log printed. The stages are freed once linked. */
GLuint	shader_load(const char *vert_path, const char *frag_path)
{
	GLuint	vs;
	GLuint	fs;
	GLuint	program;

	vs = compile_stage(GL_VERTEX_SHADER, vert_path);
	fs = compile_stage(GL_FRAGMENT_SHADER, frag_path);
	program = 0;
	if (vs && fs)
		program = link_program(vs, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return (program);
}

void	shader_set_mat4(GLuint program, const char *name, t_mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value.m);
}

void	shader_set_float(GLuint program, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void	shader_set_vec2(GLuint program, const char *name, float x, float y)
{
	glUniform2f(glGetUniformLocation(program, name), x, y);
}

void	shader_set_int(GLuint program, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
