#ifndef SCOP_H
# define SCOP_H

/* POSIX.1-2008 for nanosleep, sigaction and fileno: it must come before any system header */
# define _POSIX_C_SOURCE		200809L
/* gl_loader.h declares OpenGL, so GLFW must not include a GL header of its own */
# define GLFW_INCLUDE_NONE

# include <GLFW/glfw3.h>
# include <errno.h>
# include <limits.h>
# include <math.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <time.h>

# include "gl_loader.h"
# include "vecmath.h"
# include "util.h"
# include "obj.h"
# include "image.h"
# include "render.h"

# define WIN_TITLE				"scop"
# define WIN_WIDTH				1280
# define WIN_HEIGHT				720
# define WIN_WIDTH_MIN			1280
# define WIN_HEIGHT_MIN			720
# define WIN_WIDTH_MAX			3840
# define WIN_HEIGHT_MAX			2160
# define ARGS_USAGE				"Usage: %s <model.obj> [texture.bmp] [width height]\n"

# define MESH_VERT				"shaders/mesh.vert"
# define MESH_FRAG				"shaders/mesh.frag"
# define TEXTURE_DEFAULT		"resources/kittens.bmp"

# define CLEAR_R				0.10f
# define CLEAR_G				0.11f
# define CLEAR_B				0.13f
# define FOV					(SCOP_PI / 4.0f)
# define NEAR_PLANE				0.1f
# define FAR_PLANE				100.0f
# define CAMERA_HEIGHT			0.6f
# define CAMERA_DISTANCE		2.6f
# define DRAW_MODES				3
# define POINT_SIZE				3.0f
# define FADE_SECONDS			0.8f

# define SPIN_SPEED				(SCOP_PI / 4.0f)
# define ROTATE_SPEED			(SCOP_PI / 2.0f)
# define MOVE_SPEED				1.5f
# define MOVE_LIMIT_XY			2.0f
# define MOVE_LIMIT_NEAR		1.2f
# define MOVE_LIMIT_FAR			-12.0f

# define FPS_CAP				60
# define MAX_FRAME_SECONDS		0.25f
# define HUD_REFRESH_SECONDS	0.25f
# define HUD_TITLE_SIZE			512
# define HUD_FORMAT				"scop  /  FPS:%d | '%s' |"

typedef struct s_hud
{
	int		frames;
	float	elapsed;
	int		fps;
	int		visible;
}	t_hud;

typedef struct s_fade
{
	float	value;
	int		on;
}	t_fade;

typedef struct s_view
{
	float	rotation[3];
	float	position[3];
	float	spin;
	int		paused;
}	t_view;

typedef struct s_app
{
	GLFWwindow		*window;
	const char		*obj_path;
	const char		*texture_path;
	int				width;
	int				height;
	unsigned int	program;
	t_mesh			mesh;
	unsigned int	texture;
	float			texture_scale[2];
	t_fade			textured;
	t_fade			triplanar;
	t_fade			lit;
	int				draw_mode;
	t_hud			hud;
	t_view			view;
}	t_app;

int		app_init(t_app *app, int argc, char **argv);
void	app_run(t_app *app);
void	app_destroy(t_app *app);

int		args_parse(t_app *app, int argc, char **argv);

int		window_init(t_app *app);
void	window_destroy(t_app *app);
void	hud_update(t_app *app, float dt);
void	hud_toggle(t_app *app);

void	input_catch_interrupt(void);
void	input_init(t_app *app);
int		input_interrupted(void);

void	view_update(t_view *v, GLFWwindow *w, float dt);
void	view_reset(t_view *v);
t_mat4	view_model_matrix(const t_view *v);

void	fade_update(t_fade *fade, float dt);
void	draw_frame(t_app *app);

#endif
