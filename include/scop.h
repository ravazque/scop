#ifndef SCOP_H
# define SCOP_H

# define _POSIX_C_SOURCE	200809L	/* POSIX.1-2008 (nanosleep, sigaction); must precede every system header */
# define GLFW_INCLUDE_NONE			/* no GL header from GLFW: gl_loader.h declares OpenGL */

# include <GLFW/glfw3.h>	/* window, OpenGL context, input and timer */
# include <math.h>			/* fminf, fmodf, lroundf, tanf, sinf, cosf, sqrtf */
# include <signal.h>		/* sigaction, to close cleanly on Ctrl+C */
# include <stdio.h>			/* fprintf, snprintf and the shader file reads */
# include <stdlib.h>		/* malloc, free */
# include <string.h>		/* memset, strlen, strcmp */
# include <time.h>			/* nanosleep, to hold FPS_CAP */

# include "gl_loader.h"		/* OpenGL types, constants and loaded entry points */
# include "vecmath.h"		/* vectors, matrices and projections */
# include "render.h"		/* shaders and GPU meshes */

/* ---- Window: size in screen coordinates; MIN/MAX bound both the arguments and resizing ---- */
# define WIN_TITLE			"scop"
# define WIN_WIDTH			1280
# define WIN_HEIGHT			720
# define WIN_WIDTH_MIN		1280
# define WIN_HEIGHT_MIN		720
# define WIN_WIDTH_MAX		3840
# define WIN_HEIGHT_MAX		2160

/* ---- Shader sources (relative to the run directory) ---- */
# define MESH_VERT			"shaders/mesh.vert"
# define MESH_FRAG			"shaders/mesh.frag"

/* ---- Scene ---- */
# define CLEAR_R			0.10f
# define CLEAR_G			0.11f
# define CLEAR_B			0.13f
# define FOV_DEGREES		45.0f
# define NEAR_PLANE			0.1f
# define FAR_PLANE			100.0f
# define CAMERA_HEIGHT		1.0f
# define CAMERA_DISTANCE	2.5f
# define SPIN_SPEED			DEG2RAD(45.0f)	/* rad/s of the automatic rotation */

/* ---- Frame timing and title readout ---- */
# define FPS_CAP			60		/* frames per second the main loop is limited to */
# define MAX_FRAME_TIME		0.25f	/* s: longer frames (a dragged window) are clamped */
# define HUD_REFRESH_PERIOD	0.25f	/* s between two FPS measurements */

typedef struct s_hud
{
	int		frames;
	float	elapsed;
	int		fps;
	int		visible;
}	t_hud;

typedef struct s_app
{
	GLFWwindow	*window;
	const char	*obj_path;
	int			width;
	int			height;
	GLuint		program;
	t_mesh		mesh;
	t_hud		hud;
	float		angle;
}	t_app;

/* ---- src/app/app.c ---- */
int		app_init(t_app *app, int argc, char **argv);
void	app_run(t_app *app);
void	app_destroy(t_app *app);

/* ---- src/app/args.c ---- */
int		args_parse(t_app *app, int argc, char **argv);

/* ---- src/app/window.c ---- */
int		window_init(t_app *app);
void	window_destroy(t_app *app);

/* ---- src/app/input.c ---- */
void	input_catch_interrupt(void);
void	input_init(t_app *app);
int		input_interrupted(void);

/* ---- src/app/hud.c ---- */
void	hud_update(t_app *app, float frame_time);
void	hud_toggle(t_app *app);

/* ---- src/app/draw.c ---- */
void	draw_frame(t_app *app);

#endif
