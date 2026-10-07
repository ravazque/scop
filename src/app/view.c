/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:29:27 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Keyboard-driven model transform: W/S, A/D, Q/E rotate; arrows, R/F move */

/* 1, -1 or 0 from a pair of held keys */
static float	axis(GLFWwindow *w, int negative, int positive)
{
	return ((float)(glfwGetKey(w, positive) == GLFW_PRESS)
		- (float)(glfwGetKey(w, negative) == GLFW_PRESS));
}

static float	clamp(float value, float low, float high)
{
	return (fminf(fmaxf(value, low), high));
}

void	view_update(t_view *v, GLFWwindow *w, float dt)
{
	const float	turn = ROTATE_SPEED * dt;
	const float	move = MOVE_SPEED * dt;
	int			i;

	v->rotation[0] += turn * axis(w, GLFW_KEY_W, GLFW_KEY_S);
	v->rotation[1] += turn * axis(w, GLFW_KEY_A, GLFW_KEY_D);
	v->rotation[2] += turn * axis(w, GLFW_KEY_E, GLFW_KEY_Q);
	v->position[0] += move * axis(w, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
	v->position[1] += move * axis(w, GLFW_KEY_DOWN, GLFW_KEY_UP);
	v->position[2] += move * axis(w, GLFW_KEY_F, GLFW_KEY_R);
	i = 0;
	while (i < 3)
	{
		v->rotation[i] = fmodf(v->rotation[i], 2.0f * SCOP_PI);
		i++;
	}
	v->position[0] = clamp(v->position[0], -MOVE_LIMIT_XY, MOVE_LIMIT_XY);
	v->position[1] = clamp(v->position[1], -MOVE_LIMIT_XY, MOVE_LIMIT_XY);
	v->position[2] = clamp(v->position[2], MOVE_LIMIT_FAR, MOVE_LIMIT_NEAR);
	if (!v->paused)
		v->spin = fmodf(v->spin + SPIN_SPEED * dt, 2.0f * SCOP_PI);
}

/* Back to the start: centered and unrotated; a paused spin stays paused */
void	view_reset(t_view *v)
{
	const int	paused = v->paused;

	memset(v, 0, sizeof(*v));
	v->paused = paused;
}

/* The spin turns the model around its own vertical axis, inside X and Z */
t_mat4	view_model_matrix(const t_view *v)
{
	t_mat4	m;

	m = mat4_translation(vec3(v->position[0], v->position[1], v->position[2]));
	m = mat4_mul(m, mat4_rotation_z(v->rotation[2]));
	m = mat4_mul(m, mat4_rotation_x(v->rotation[0]));
	return (mat4_mul(m, mat4_rotation_y(v->rotation[1] + v->spin)));
}
