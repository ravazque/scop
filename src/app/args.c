#include "scop.h"

/* Command line: <model.obj> [width height]. A bad window size falls back to the default. */

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

int	args_parse(t_app *app, int argc, char **argv)
{
	size_t	len;

	if (argc != 2 && argc != 4)
	{
		fprintf(stderr, "Usage: %s <model.obj> [width height]\n", argv[0]);
		return (0);
	}
	len = strlen(argv[1]);
	if (len <= 4 || strcmp(argv[1] + len - 4, ".obj") != 0)
	{
		fprintf(stderr, "Error: '%s' is not a .obj file\n", argv[1]);
		return (0);
	}
	app->obj_path = argv[1];
	app->width = WIN_WIDTH;
	app->height = WIN_HEIGHT;
	if (argc == 4)
	{
		app->width = resolve_dim(argv[2], WIN_WIDTH_MIN, WIN_WIDTH_MAX, WIN_WIDTH, "Width");
		app->height = resolve_dim(argv[3], WIN_HEIGHT_MIN, WIN_HEIGHT_MAX, WIN_HEIGHT, "Height");
	}
	return (1);
}
