#ifndef GL_LOADER_H
# define GL_LOADER_H

# include <stddef.h>		/* ptrdiff_t, for GLsizeiptr */

/* OpenGL 4.1 core subset used by scop, loaded with glfwGetProcAddress; add a function to GL_FUNCTIONS and its alias. */

typedef unsigned int	GLenum;
typedef unsigned int	GLbitfield;
typedef unsigned int	GLuint;
typedef int				GLint;
typedef int				GLsizei;
typedef unsigned char	GLboolean;
typedef float			GLfloat;
typedef char			GLchar;
typedef ptrdiff_t		GLsizeiptr;

# define GL_FALSE					0
# define GL_TRUE					1
# define GL_TRIANGLES				0x0004
# define GL_DEPTH_BUFFER_BIT		0x00000100
# define GL_COLOR_BUFFER_BIT		0x00004000
# define GL_DEPTH_TEST				0x0B71
# define GL_UNSIGNED_INT			0x1405
# define GL_FLOAT					0x1406
# define GL_ARRAY_BUFFER			0x8892
# define GL_ELEMENT_ARRAY_BUFFER	0x8893
# define GL_STATIC_DRAW				0x88E4
# define GL_FRAGMENT_SHADER			0x8B30
# define GL_VERTEX_SHADER			0x8B31
# define GL_COMPILE_STATUS			0x8B81
# define GL_LINK_STATUS				0x8B82

/* X(return type, name without the gl prefix, parameter list) */
# define GL_FUNCTIONS(X) \
	X(void, Viewport, (GLint x, GLint y, GLsizei width, GLsizei height)) \
	X(void, ClearColor, (GLfloat r, GLfloat g, GLfloat b, GLfloat a)) \
	X(void, Clear, (GLbitfield mask)) \
	X(void, Enable, (GLenum cap)) \
	X(void, DrawElements, (GLenum mode, GLsizei count, GLenum type, const void *indices)) \
	X(void, GenVertexArrays, (GLsizei n, GLuint *arrays)) \
	X(void, BindVertexArray, (GLuint array)) \
	X(void, DeleteVertexArrays, (GLsizei n, const GLuint *arrays)) \
	X(void, GenBuffers, (GLsizei n, GLuint *buffers)) \
	X(void, BindBuffer, (GLenum target, GLuint buffer)) \
	X(void, BufferData, (GLenum target, GLsizeiptr size, const void *data, GLenum usage)) \
	X(void, DeleteBuffers, (GLsizei n, const GLuint *buffers)) \
	X(void, EnableVertexAttribArray, (GLuint index)) \
	X(void, VertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)) \
	X(GLuint, CreateShader, (GLenum type)) \
	X(void, ShaderSource, (GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length)) \
	X(void, CompileShader, (GLuint shader)) \
	X(void, GetShaderiv, (GLuint shader, GLenum pname, GLint *params)) \
	X(void, GetShaderInfoLog, (GLuint shader, GLsizei size, GLsizei *length, GLchar *log)) \
	X(void, DeleteShader, (GLuint shader)) \
	X(GLuint, CreateProgram, (void)) \
	X(void, AttachShader, (GLuint program, GLuint shader)) \
	X(void, LinkProgram, (GLuint program)) \
	X(void, GetProgramiv, (GLuint program, GLenum pname, GLint *params)) \
	X(void, GetProgramInfoLog, (GLuint program, GLsizei size, GLsizei *length, GLchar *log)) \
	X(void, DeleteProgram, (GLuint program)) \
	X(void, UseProgram, (GLuint program)) \
	X(GLint, GetUniformLocation, (GLuint program, const GLchar *name)) \
	X(void, UniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)) \
	X(void, Uniform1f, (GLint location, GLfloat v0)) \
	X(void, Uniform1i, (GLint location, GLint v0))

# define GL_DECLARE(ret, name, params) \
	typedef ret	(*t_gl_##name)params; \
	extern t_gl_##name	g_gl_##name;

GL_FUNCTIONS(GL_DECLARE)

# undef GL_DECLARE

/* Used only by gl_loader.c: defines each pointer, and loads it inside gl_load_functions (a miss sets its ok to 0). */
# define GL_DEFINE(ret, name, params)	t_gl_##name	g_gl_##name;
# define GL_LOAD(ret, name, params) \
	g_gl_##name = (t_gl_##name)glfwGetProcAddress("gl" #name); \
	if (!g_gl_##name) \
	{ \
		fprintf(stderr, "Error: OpenGL function gl" #name " not found\n"); \
		ok = 0; \
	}

/* Prefixed pointers, so they never clash with the symbols the GL library exports. */
# define glViewport					g_gl_Viewport
# define glClearColor				g_gl_ClearColor
# define glClear					g_gl_Clear
# define glEnable					g_gl_Enable
# define glDrawElements				g_gl_DrawElements
# define glGenVertexArrays			g_gl_GenVertexArrays
# define glBindVertexArray			g_gl_BindVertexArray
# define glDeleteVertexArrays		g_gl_DeleteVertexArrays
# define glGenBuffers				g_gl_GenBuffers
# define glBindBuffer				g_gl_BindBuffer
# define glBufferData				g_gl_BufferData
# define glDeleteBuffers			g_gl_DeleteBuffers
# define glEnableVertexAttribArray	g_gl_EnableVertexAttribArray
# define glVertexAttribPointer		g_gl_VertexAttribPointer
# define glCreateShader				g_gl_CreateShader
# define glShaderSource				g_gl_ShaderSource
# define glCompileShader			g_gl_CompileShader
# define glGetShaderiv				g_gl_GetShaderiv
# define glGetShaderInfoLog			g_gl_GetShaderInfoLog
# define glDeleteShader				g_gl_DeleteShader
# define glCreateProgram			g_gl_CreateProgram
# define glAttachShader				g_gl_AttachShader
# define glLinkProgram				g_gl_LinkProgram
# define glGetProgramiv				g_gl_GetProgramiv
# define glGetProgramInfoLog		g_gl_GetProgramInfoLog
# define glDeleteProgram			g_gl_DeleteProgram
# define glUseProgram				g_gl_UseProgram
# define glGetUniformLocation		g_gl_GetUniformLocation
# define glUniformMatrix4fv			g_gl_UniformMatrix4fv
# define glUniform1f				g_gl_Uniform1f
# define glUniform1i				g_gl_Uniform1i

int	gl_load_functions(void);

#endif
