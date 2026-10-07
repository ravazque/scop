#ifndef SCOP_H
# define SCOP_H

# define _POSIX_C_SOURCE	200809L	/* POSIX.1-2008 (nanosleep, sigaction, fileno); must precede every system header */
# define GLFW_INCLUDE_NONE			/* no GL header from GLFW: gl_loader.h declares OpenGL */

# include <GLFW/glfw3.h>	/* window, OpenGL context, input and timer */
# include <errno.h>			/* errno, ERANGE from strtol */
# include <limits.h>		/* UINT_MAX, INT_MAX: index and draw-count limits */
# include <math.h>			/* fminf, fmodf, lroundf, isfinite, tanf, sinf, cosf, sqrtf */
# include <signal.h>		/* sigaction, to close cleanly on Ctrl+C */
# include <stdint.h>		/* SIZE_MAX */
# include <stdio.h>			/* fprintf, snprintf, fopen, fread */
# include <stdlib.h>		/* malloc, realloc, free, strtof, strtol */
# include <string.h>		/* memset, memmove, strlen, strcmp, strchr, strerror */
# include <sys/stat.h>		/* fstat, S_ISREG: only regular files are read */
# include <time.h>			/* nanosleep, to hold FPS_CAP */

# include "gl_loader.h"		/* OpenGL types, constants and loaded entry points */
# include "vecmath.h"		/* vectors, matrices and projections */
# include "util.h"			/* whole-file reads and growable arrays */
# include "obj.h"			/* .obj parsing and triangulation */
# include "image.h"			/* BMP images */
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

/* ---- Texture ---- */
# define TEXTURE_DEFAULT	"resources/kittens.bmp"
# define FADE_SECONDS		0.8f		/* length of every toggle transition */

/* ---- Scene ---- */
# define CLEAR_R			0.10f
# define CLEAR_G			0.11f
# define CLEAR_B			0.13f
# define FOV_DEGREES		45.0f
# define DRAW_MODES			3		/* filled faces, wireframe, points (M cycles through them) */
# define POINT_SIZE			3.0f	/* pixels per vertex in the points mode */
# define NEAR_PLANE			0.1f
# define FAR_PLANE			100.0f
# define CAMERA_HEIGHT		0.6f
# define CAMERA_DISTANCE	2.6f
# define SPIN_SPEED			DEG2RAD(45.0f)	/* rad/s of the automatic rotation */

/* ---- Model controls (keys held down; the change is speed times frame time) ---- */
# define ROTATE_SPEED		DEG2RAD(90.0f)	/* rad/s around each axis */
# define MOVE_SPEED			1.5f			/* units/s along each axis; the model fits in a unit sphere */
# define MOVE_LIMIT_XY		2.0f			/* farthest the center may go sideways or up and down */
# define MOVE_LIMIT_NEAR	1.2f			/* closest z: the model stays in front of the camera */
# define MOVE_LIMIT_FAR		-12.0f			/* farthest z */

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

/* A toggle that eases between 0 and 1 instead of switching, so the picture never cuts. */
typedef struct s_fade
{
	float	value;
	int		on;
}	t_fade;

/* Where the user put the model: rotations around its own axes, then a translation along the world axes. */
typedef struct s_view
{
	float	rotation[3];
	float	position[3];
	float	spin;
	int		paused;
}	t_view;

typedef struct s_app
{
	GLFWwindow	*window;
	const char	*obj_path;
	const char	*texture_path;
	int			width;
	int			height;
	GLuint		program;
	t_mesh		mesh;
	GLuint		texture;
	float		texture_scale[2];
	t_fade		textured;
	t_fade		triplanar;
	t_fade		lit;
	int			draw_mode;
	t_hud		hud;
	t_view		view;
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

/* ---- src/app/view.c ---- */
void	view_update(t_view *view, GLFWwindow *window, float frame_time);
void	view_reset(t_view *view);
t_mat4	view_model_matrix(const t_view *view);

/* ---- src/app/fade.c ---- */
void	fade_update(t_fade *fade, float frame_time);
float	fade_eased(const t_fade *fade);

/* ---- src/app/hud.c ---- */
void	hud_update(t_app *app, float frame_time);
void	hud_toggle(t_app *app);

/* ---- src/app/draw.c ---- */
void	draw_frame(t_app *app);

#endif
