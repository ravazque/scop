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

/* X(return type, name without the gl prefix, parameter list) */
# define GL_FUNCTIONS(X) \
	X(void, Viewport, (GLint x, GLint y, GLsizei width, GLsizei height)) \
	X(void, ClearColor, (GLfloat r, GLfloat g, GLfloat b, GLfloat a)) \
	X(void, Clear, (GLbitfield mask)) \
	X(void, Enable, (GLenum cap)) \
	X(void, PolygonMode, (GLenum face, GLenum mode)) \
	X(void, PointSize, (GLfloat size)) \
	X(void, DrawArrays, (GLenum mode, GLint first, GLsizei count)) \
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
	X(void, Uniform2f, (GLint location, GLfloat v0, GLfloat v1)) \
	X(void, Uniform1i, (GLint location, GLint v0)) \
	X(void, GenTextures, (GLsizei n, GLuint *textures)) \
	X(void, BindTexture, (GLenum target, GLuint texture)) \
	X(void, ActiveTexture, (GLenum texture)) \
	X(void, TexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)) \
	X(void, TexParameteri, (GLenum target, GLenum pname, GLint param)) \
	X(void, GenerateMipmap, (GLenum target)) \
	X(void, DeleteTextures, (GLsizei n, const GLuint *textures))

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
# define glPolygonMode				g_gl_PolygonMode
# define glPointSize				g_gl_PointSize
# define glDrawArrays				g_gl_DrawArrays
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
# define glUniform2f				g_gl_Uniform2f
# define glUniform1i				g_gl_Uniform1i
# define glGenTextures				g_gl_GenTextures
# define glBindTexture				g_gl_BindTexture
# define glActiveTexture			g_gl_ActiveTexture
# define glTexImage2D				g_gl_TexImage2D
# define glTexParameteri			g_gl_TexParameteri
# define glGenerateMipmap			g_gl_GenerateMipmap
# define glDeleteTextures			g_gl_DeleteTextures

int	gl_load_functions(void);

#endif
