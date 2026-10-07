/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Keyboard set-up and Ctrl+C in the terminal, which also closes cleanly */

static volatile sig_atomic_t	g_interrupted = 0;

static void	on_interrupt(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

/* Installed first, so a Ctrl+C during start-up also ends cleanly */
void	input_catch_interrupt(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_interrupt;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

/* Sticky keys: a press shorter than a frame is still seen once */
void	input_init(t_app *app)
{
	glfwSetInputMode(app->window, GLFW_STICKY_KEYS, GLFW_TRUE);
}

int	input_interrupted(void)
{
	return (g_interrupted != 0);
}
