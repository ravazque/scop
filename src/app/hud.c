#include "scop.h"

/* Window title: the bare name by default, H adds the FPS and the model path. */

static void	set_title(t_app *app)
{
	char	title[512];

	if (!app->hud.visible)
	{
		glfwSetWindowTitle(app->window, WIN_TITLE);
		return ;
	}
	snprintf(title, sizeof(title), WIN_TITLE "  /  FPS:%d | '%s' |", app->hud.fps, app->obj_path);
	glfwSetWindowTitle(app->window, title);
}

void	hud_toggle(t_app *app)
{
	app->hud.visible = !app->hud.visible;
	set_title(app);
}

/* Frames counted over each refresh period: an exact rate, and the title changes only a few times per second. */
void	hud_update(t_app *app, float frame_time)
{
	t_hud	*h;

	h = &app->hud;
	h->frames++;
	h->elapsed += frame_time;
	if (h->elapsed < HUD_REFRESH_PERIOD)
		return ;
	h->fps = (int)lroundf((float)h->frames / h->elapsed);
	h->frames = 0;
	h->elapsed = 0.0f;
	if (h->visible)
		set_title(app);
}
