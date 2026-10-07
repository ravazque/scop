/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_ear.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Ear clipping on the projected outline of a face */

/* Twice the signed area of a, b, c: positive when they turn to the left */
static float	turn(const float *a, const float *b, const float *c)
{
	return ((b[0] - a[0]) * (c[1] - a[1]) - (b[1] - a[1]) * (c[0] - a[0]));
}

static int	same_point(const float *a, const float *b)
{
	return (a[0] == b[0] && a[1] == b[1]);
}

/* Corner i is an ear: convex, with no other corner inside or on it */
static int	is_ear(const t_obj_parser *p, size_t count, size_t i)
{
	const float	*a = p->flat + 2 * p->ring[(i + count - 1) % count];
	const float	*b = p->flat + 2 * p->ring[i];
	const float	*c = p->flat + 2 * p->ring[(i + 1) % count];
	const float	*q;
	size_t		k;

	if (turn(a, b, c) <= 0.0f)
		return (0);
	k = 0;
	while (k < count)
	{
		q = p->flat + 2 * p->ring[k++];
		if (same_point(q, a) || same_point(q, b) || same_point(q, c))
			continue ;
		if (turn(a, b, q) >= 0.0f && turn(b, c, q) >= 0.0f
			&& turn(c, a, q) >= 0.0f)
			return (0);
	}
	return (1);
}

/* Clips ears until a triangle is left; a ring with no ear is fanned */
int	obj_ear_clip(t_obj *obj, t_obj_parser *p, size_t count, unsigned int id)
{
	unsigned int	abc[3];
	size_t			i;
	size_t			misses;

	i = 0;
	misses = 0;
	while (count > 3 && misses < count)
	{
		misses++;
		if (is_ear(p, count, i))
		{
			abc[0] = p->polygon[p->ring[(i + count - 1) % count]];
			abc[1] = p->polygon[p->ring[i]];
			abc[2] = p->polygon[p->ring[(i + 1) % count]];
			if (!obj_emit(obj, abc, id))
				return (0);
			memmove(p->ring + i, p->ring + i + 1,
				(count - i - 1) * sizeof(size_t));
			count--;
			misses = 0;
		}
		i = (i + 1) % count;
	}
	return (obj_fan(obj, p, count, id));
}
