#include "scop.h"

/* Doubling keeps n appends at O(n) in total; on failure data and cap are left untouched */
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
