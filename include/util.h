/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:07:38 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>		/* size_t */

# define ARRAY_MIN_CAPACITY	16	/* elements on the first allocation */

/* ---- src/util/file.c ---- */
char	*file_read(const char *path, size_t *size);

/* ---- src/util/array.c ---- */
void	*array_reserve(void *data, size_t *cap, size_t need, size_t elem_size);

#endif
