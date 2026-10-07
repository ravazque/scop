#include "scop.h"

/* Toggles that ease over FADE_SECONDS instead of switching at once. */

void	fade_update(t_fade *fade, float frame_time)
{
	const float	step = frame_time / FADE_SECONDS;

	if (fade->on)
		fade->value = fminf(fade->value + step, 1.0f);
	else
		fade->value = fmaxf(fade->value - step, 0.0f);
}

/* Smoothstep of the linear value: it starts and ends gently. */
float	fade_eased(const t_fade *fade)
{
	return (fade->value * fade->value * (3.0f - 2.0f * fade->value));
}
