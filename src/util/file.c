#include "scop.h"

/* Whole files in memory. */

/* NUL-terminated copy of a regular file, NULL with a message on failure; size (optional) excludes the terminator. */
char	*file_read(const char *path, size_t *size)
{
	FILE		*f;
	struct stat	st;
	char		*buf;

	f = fopen(path, "rb");
	if (!f)
		return (fprintf(stderr, "Error: cannot open %s: %s\n", path, strerror(errno)), NULL);
	if (fstat(fileno(f), &st) != 0 || !S_ISREG(st.st_mode))
		return (fprintf(stderr, "Error: %s is not a regular file\n", path), fclose(f), NULL);
	buf = malloc((size_t)st.st_size + 1);
	if (!buf)
		return (fprintf(stderr, "Error: out of memory\n"), fclose(f), NULL);
	if (fread(buf, 1, (size_t)st.st_size, f) != (size_t)st.st_size)
		return (fprintf(stderr, "Error: cannot read %s\n", path), free(buf), fclose(f), NULL);
	buf[st.st_size] = '\0';
	fclose(f);
	if (size)
		*size = (size_t)st.st_size;
	return (buf);
}
