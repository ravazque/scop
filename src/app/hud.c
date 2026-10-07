/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Window title: the bare name by default, H adds the FPS and the model path */

static void	set_title(t_app *app)
{
	char	title[HUD_TITLE_SIZE];

	if (!app->hud.visible)
	{
		glfwSetWindowTitle(app->window, WIN_TITLE);
		return ;
	}
	snprintf(title, sizeof(title), HUD_FORMAT, app->hud.fps, app->obj_path);
	glfwSetWindowTitle(app->window, title);
}

void	hud_toggle(t_app *app)
{
	app->hud.visible = !app->hud.visible;
	set_title(app);
}

/* Frames counted over each period: an exact rate, few title rewrites */
void	hud_update(t_app *app, float dt)
{
	t_hud	*h;

	h = &app->hud;
	h->frames++;
	h->elapsed += dt;
	if (h->elapsed < HUD_REFRESH_PERIOD)
		return ;
	h->fps = (int)lroundf((float)h->frames / h->elapsed);
	h->frames = 0;
	h->elapsed = 0.0f;
	if (h->visible)
		set_title(app);
}
