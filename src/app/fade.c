/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fade.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:51:47 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Toggles that ease over FADE_SECONDS instead of switching at once */

void	fade_update(t_fade *fade, float dt)
{
	const float	step = dt / FADE_SECONDS;

	if (fade->on)
		fade->value = fminf(fade->value + step, 1.0f);
	else
		fade->value = fmaxf(fade->value - step, 0.0f);
}

/* Smoothstep of the linear value: it starts and ends gently */
float	fade_eased(const t_fade *fade)
{
	return (fade->value * fade->value * (3.0f - 2.0f * fade->value));
}
