#include "scop.h"

static char	*fail(FILE *f, char *buf, const char *path, const char *why)
{
	fprintf(stderr, "Error: %s: %s\n", path, why);
	free(buf);
	if (f)
		fclose(f);
	return (NULL);
}

/* NUL-terminated copy of a regular file; *size, when asked for, does not count the NUL */
char	*file_read(const char *path, size_t *size)
{
	FILE		*f;
	struct stat	st;
	char		*buf;

	f = fopen(path, "rb");
	if (!f)
		return (fail(NULL, NULL, path, strerror(errno)));
	if (fstat(fileno(f), &st) != 0 || !S_ISREG(st.st_mode))
		return (fail(f, NULL, path, "not a regular file"));
	buf = malloc((size_t)st.st_size + 1);
	if (!buf)
		return (fail(f, NULL, path, "out of memory"));
	if (fread(buf, 1, (size_t)st.st_size, f) != (size_t)st.st_size)
		return (fail(f, buf, path, "cannot read the file"));
	buf[st.st_size] = '\0';
	fclose(f);
	if (size)
		*size = (size_t)st.st_size;
	return (buf);
}
