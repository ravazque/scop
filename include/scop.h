/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

/* POSIX.1-2008 (nanosleep, sigaction, fileno), before any system header */
# define _POSIX_C_SOURCE	200809L
/* No GL header from GLFW: gl_loader.h declares OpenGL */
# define GLFW_INCLUDE_NONE

# include <GLFW/glfw3.h>	/* window, OpenGL context, keyboard and timer */
# include <errno.h>			/* errno, ERANGE from strtol */
# include <limits.h>		/* UINT_MAX, INT_MAX: index and draw limits */
# include <math.h>			/* fminf, fmodf, lroundf, isfinite, sqrtf... */
# include <signal.h>		/* sigaction, to close cleanly on Ctrl+C */
# include <stdint.h>		/* SIZE_MAX, INT32_MIN, uintptr_t */
# include <stdio.h>			/* fprintf, snprintf, fopen, fread */
# include <stdlib.h>		/* malloc, realloc, free, strtof, strtol */
# include <string.h>		/* memset, memcpy, memmove, strlen, strcmp... */
# include <sys/stat.h>		/* fstat, S_ISREG: only regular files are read */
# include <time.h>			/* nanosleep, to hold FPS_CAP */

# include "gl_loader.h"		/* OpenGL constants and loaded entry points */
# include "vecmath.h"		/* vectors, matrices and projections */
# include "util.h"			/* whole-file reads and growable arrays */
# include "obj.h"			/* .obj parsing and triangulation */
# include "image.h"			/* BMP images */
# include "render.h"		/* shaders, meshes and textures on the GPU */

/* ---- Window: screen coordinates; MIN/MAX bound arguments and resizing ---- */
# define WIN_TITLE			"scop"
# define WIN_WIDTH			1280
# define WIN_HEIGHT			720
# define WIN_WIDTH_MIN		1280
# define WIN_HEIGHT_MIN		720
# define WIN_WIDTH_MAX		3840
# define WIN_HEIGHT_MAX		2160
# define ARGS_USAGE	"Usage: %s <model.obj> [texture.bmp] [width height]\n"

/* ---- Shader sources and default texture (from the run directory) ---- */
# define MESH_VERT			"shaders/mesh.vert"
# define MESH_FRAG			"shaders/mesh.frag"
# define TEXTURE_DEFAULT	"resources/kittens.bmp"

/* ---- Scene ---- */
# define CLEAR_R			0.10f
# define CLEAR_G			0.11f
# define CLEAR_B			0.13f
# define FOV				0.785398163f	/* vertical field of view, 45 deg */
# define NEAR_PLANE			0.1f
# define FAR_PLANE			100.0f
# define CAMERA_HEIGHT		0.6f
# define CAMERA_DISTANCE	2.6f
# define DRAW_MODES			3				/* filled, wireframe, points */
# define POINT_SIZE			3.0f			/* pixels per point */
# define FADE_SECONDS		0.8f			/* length of every transition */

/* ---- Motion: keys act while held, at speed times frame time ---- */
# define SPIN_SPEED			0.785398163f	/* rad/s of the spin, 45 deg/s */
# define ROTATE_SPEED		1.570796327f	/* rad/s per axis, 90 deg/s */
# define MOVE_SPEED			1.5f			/* units/s; the model is 2 wide */
# define MOVE_LIMIT_XY		2.0f			/* farthest sideways or up */
# define MOVE_LIMIT_NEAR	1.2f			/* closest z, before the camera */
# define MOVE_LIMIT_FAR		-12.0f			/* farthest z */

/* ---- Frame timing and title readout ---- */
# define FPS_CAP			60				/* frames per second at most */
# define MAX_FRAME_TIME		0.25f			/* s: longer frames are clamped */
# define HUD_REFRESH_PERIOD	0.25f			/* s between FPS measurements */
# define HUD_TITLE_SIZE		512
# define HUD_FORMAT			"scop  /  FPS:%d | '%s' |"

typedef struct s_hud
{
	int		frames;
	float	elapsed;
	int		fps;
	int		visible;
}	t_hud;

/* A toggle that eases from 0 to 1 and back, so the picture never cuts */
typedef struct s_fade
{
	float	value;
	int		on;
}	t_fade;

/* Rotations around the model's own axes, then a move along the world axes */
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
	unsigned char	held[GLFW_KEY_LAST + 1];
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

/* ---- src/app/keys.c ---- */
void	input_poll(t_app *app);

/* ---- src/app/view.c ---- */
void	view_update(t_view *v, GLFWwindow *w, float dt);
void	view_reset(t_view *v);
t_mat4	view_model_matrix(const t_view *v);

/* ---- src/app/fade.c ---- */
void	fade_update(t_fade *fade, float dt);
float	fade_eased(const t_fade *fade);

/* ---- src/app/hud.c ---- */
void	hud_update(t_app *app, float dt);
void	hud_toggle(t_app *app);

/* ---- src/app/draw.c ---- */
void	draw_frame(t_app *app);

#endif
