#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

# define ARRAY_MIN_CAPACITY	16

char	*file_read(const char *path, size_t *size);
void	*array_reserve(void *data, size_t *cap, size_t need, size_t elem_size);

#endif
