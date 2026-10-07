/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Keys that act once per press: ESC, H, T, U, L, M, Space and Backspace */

/* 1 on the frame the key goes down; held[] keeps the previous frame */
static int	pressed(t_app *app, int key)
{
	const int	down = glfwGetKey(app->window, key) == GLFW_PRESS;
	const int	was_down = app->held[key];

	app->held[key] = down;
	return (down && !was_down);
}

void	input_poll(t_app *app)
{
	if (pressed(app, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(app->window, GLFW_TRUE);
	if (pressed(app, GLFW_KEY_H))
		hud_toggle(app);
	if (pressed(app, GLFW_KEY_T))
		app->textured.on = !app->textured.on;
	if (pressed(app, GLFW_KEY_U))
		app->triplanar.on = !app->triplanar.on;
	if (pressed(app, GLFW_KEY_L))
		app->lit.on = !app->lit.on;
	if (pressed(app, GLFW_KEY_M))
		app->draw_mode = (app->draw_mode + 1) % DRAW_MODES;
	if (pressed(app, GLFW_KEY_SPACE))
		app->view.paused = !app->view.paused;
	if (pressed(app, GLFW_KEY_BACKSPACE))
		view_reset(&app->view);
}
