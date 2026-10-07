#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>		/* size_t */

# define ARRAY_MIN_CAPACITY	16	/* elements of a growable array on its first allocation */

/* ---- src/util/file.c ---- */
char	*file_read(const char *path, size_t *size);

/* ---- src/util/array.c ---- */
void	*array_reserve(void *data, size_t *cap, size_t need, size_t elem_size);

#endif
