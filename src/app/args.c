#include "scop.h"

/* Command line: <model.obj> [texture.bmp] [width height]. A bad window size falls back to the default. */

static int	has_extension(const char *path, const char *extension)
{
	const size_t	len = strlen(path);
	const size_t	ext_len = strlen(extension);

	return (len > ext_len && strcmp(path + len - ext_len, extension) == 0);
}

static int	is_number(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '\0')
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* -1 when the value leaves [min, max]; checked digit by digit, so it cannot overflow. */
static int	parse_dim(const char *s, int min, int max)
{
	int	v;
	int	i;

	v = 0;
	i = 0;
	while (s[i] != '\0')
	{
		v = v * 10 + (s[i] - '0');
		if (v > max)
			return (-1);
		i++;
	}
	if (v < min)
		return (-1);
	return (v);
}

static int	resolve_dim(const char *arg, int min, int max, int def, const char *label)
{
	int	value;

	if (!is_number(arg))
	{
		fprintf(stderr, "%s '%s' is not a valid number; using default %d\n", label, arg, def);
		return (def);
	}
	value = parse_dim(arg, min, max);
	if (value < 0)
	{
		fprintf(stderr, "%s '%s' is out of range [%d..%d]; using default %d\n", label, arg, min, max, def);
		return (def);
	}
	return (value);
}

/* With 3 or 5 arguments the second one is the texture; the size comes last when present. */
int	args_parse(t_app *app, int argc, char **argv)
{
	int	size_at;

	if (argc < 2 || argc > 5)
		return (fprintf(stderr, "Usage: %s <model.obj> [texture.bmp] [width height]\n", argv[0]), 0);
	if (!has_extension(argv[1], ".obj"))
		return (fprintf(stderr, "Error: '%s' is not a .obj file\n", argv[1]), 0);
	app->obj_path = argv[1];
	app->texture_path = TEXTURE_DEFAULT;
	size_at = 2;
	if (argc == 3 || argc == 5)
	{
		if (!has_extension(argv[2], ".bmp"))
			return (fprintf(stderr, "Error: '%s' is not a .bmp file\n", argv[2]), 0);
		app->texture_path = argv[2];
		size_at = 3;
	}
	app->width = WIN_WIDTH;
	app->height = WIN_HEIGHT;
	if (argc - size_at == 2)
	{
		app->width = resolve_dim(argv[size_at], WIN_WIDTH_MIN, WIN_WIDTH_MAX, WIN_WIDTH, "Width");
		app->height = resolve_dim(argv[size_at + 1], WIN_HEIGHT_MIN, WIN_HEIGHT_MAX, WIN_HEIGHT, "Height");
	}
	return (1);
}
