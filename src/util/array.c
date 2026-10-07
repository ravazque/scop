/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Growable arrays: the capacity doubles, so n appends cost O(n) in total */

/* Room for need elements: the (maybe moved) array, or NULL with a message,
   leaving data and cap untouched */
void	*array_reserve(void *data, size_t *cap, size_t need, size_t elem_size)
{
	size_t	new_cap;
	void	*grown;

	if (need <= *cap)
		return (data);
	new_cap = *cap;
	if (new_cap < ARRAY_MIN_CAPACITY)
		new_cap = ARRAY_MIN_CAPACITY;
	while (new_cap < need && new_cap <= SIZE_MAX / 2)
		new_cap *= 2;
	if (new_cap < need || new_cap > SIZE_MAX / elem_size)
		return (fprintf(stderr, "Error: out of memory\n"), NULL);
	grown = realloc(data, new_cap * elem_size);
	if (!grown)
		return (fprintf(stderr, "Error: out of memory\n"), NULL);
	*cap = new_cap;
	return (grown);
}
