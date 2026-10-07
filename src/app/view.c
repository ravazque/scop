#include "scop.h"

/* Keyboard-driven model transform: W/S, A/D, Q/E rotate around X, Y, Z; arrows and R/F move along X, Y, Z. */

static const int	g_rotate_keys[][3] = {
	{GLFW_KEY_W, 0, -1}, {GLFW_KEY_S, 0, 1},
	{GLFW_KEY_A, 1, -1}, {GLFW_KEY_D, 1, 1},
	{GLFW_KEY_Q, 2, 1}, {GLFW_KEY_E, 2, -1},
};

static const int	g_move_keys[][3] = {
	{GLFW_KEY_LEFT, 0, -1}, {GLFW_KEY_RIGHT, 0, 1},
	{GLFW_KEY_DOWN, 1, -1}, {GLFW_KEY_UP, 1, 1},
	{GLFW_KEY_F, 2, -1}, {GLFW_KEY_R, 2, 1},
};

/* Each row is {key, axis, direction}: a held key adds direction * step to values[axis]. */
static void	apply_held(GLFWwindow *window, const int (*keys)[3], float *values, float step)
{
	int	i;

	i = 0;
	while (i < 6)
	{
		if (glfwGetKey(window, keys[i][0]) == GLFW_PRESS)
			values[keys[i][1]] += (float)keys[i][2] * step;
		i++;
	}
}

void	view_update(t_view *view, GLFWwindow *window, float frame_time)
{
	int	i;

	apply_held(window, g_rotate_keys, view->rotation, ROTATE_SPEED * frame_time);
	apply_held(window, g_move_keys, view->position, MOVE_SPEED * frame_time);
	i = 0;
	while (i < 3)
	{
		view->rotation[i] = fmodf(view->rotation[i], 2.0f * SCOP_PI);
		i++;
	}
	view->position[0] = fminf(fmaxf(view->position[0], -MOVE_LIMIT_XY), MOVE_LIMIT_XY);
	view->position[1] = fminf(fmaxf(view->position[1], -MOVE_LIMIT_XY), MOVE_LIMIT_XY);
	view->position[2] = fminf(fmaxf(view->position[2], MOVE_LIMIT_FAR), MOVE_LIMIT_NEAR);
	if (!view->paused)
		view->spin = fmodf(view->spin + SPIN_SPEED * frame_time, 2.0f * SCOP_PI);
}

/* Back to the start: centered, unrotated; a paused spin stays paused. */
void	view_reset(t_view *view)
{
	const int	paused = view->paused;

	memset(view, 0, sizeof(*view));
	view->paused = paused;
}

/* Spin and Y rotation turn the model around its own vertical axis, inside the X and Z rotations. */
t_mat4	view_model_matrix(const t_view *view)
{
	t_mat4	m;

	m = mat4_translation(vec3(view->position[0], view->position[1], view->position[2]));
	m = mat4_mul(m, mat4_rotation_z(view->rotation[2]));
	m = mat4_mul(m, mat4_rotation_x(view->rotation[0]));
	return (mat4_mul(m, mat4_rotation_y(view->rotation[1] + view->spin)));
}
