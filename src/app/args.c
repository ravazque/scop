/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 12:50:14 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* <model.obj> [texture.bmp] [width height]; a bad size uses the default */

/* The file name is a name, not empty and not ending in '.', plus extension */
static int	has_extension(const char *path, const char *extension)
{
	const char		*name = strrchr(path, '/');
	const size_t	ext_len = strlen(extension);
	size_t			len;

	if (name)
		name++;
	else
		name = path;
	len = strlen(name);
	if (len <= ext_len || strcmp(name + len - ext_len, extension) != 0)
		return (0);
	return (name[len - ext_len - 1] != '.');
}

/* -2 when s is not a number, -1 when it leaves [min, max]; cannot overflow */
static int	parse_dim(const char *s, int min, int max)
{
	int	v;
	int	i;

	if (s[0] == '\0')
		return (-2);
	v = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] < '0' || s[i] > '9')
			return (-2);
		if (v <= max)
			v = v * 10 + (s[i] - '0');
		i++;
	}
	if (v < min || v > max)
		return (-1);
	return (v);
}

/* range: minimum, maximum and default */
static int	resolve_dim(const char *arg, const int *range, const char *label)
{
	const int	value = parse_dim(arg, range[0], range[1]);

	if (value == -2)
		fprintf(stderr, "%s '%s' is not a valid number; using default %d\n",
			label, arg, range[2]);
	if (value == -1)
		fprintf(stderr, "%s '%s' is out of range [%d..%d]; using default %d\n",
			label, arg, range[0], range[1], range[2]);
	if (value < 0)
		return (range[2]);
	return (value);
}

/* With 3 or 5 arguments the second one is the texture: index of the size */
static int	texture_arg(t_app *app, int argc, char **argv)
{
	app->texture_path = TEXTURE_DEFAULT;
	if (argc != 3 && argc != 5)
		return (2);
	if (!has_extension(argv[2], ".bmp"))
	{
		fprintf(stderr, "Error: '%s' is not a .bmp file\n", argv[2]);
		return (-1);
	}
	app->texture_path = argv[2];
	return (3);
}

int	args_parse(t_app *app, int argc, char **argv)
{
	const int	width[3] = {WIN_WIDTH_MIN, WIN_WIDTH_MAX, WIN_WIDTH};
	const int	height[3] = {WIN_HEIGHT_MIN, WIN_HEIGHT_MAX, WIN_HEIGHT};
	int			size_at;

	if (argc < 2 || argc > 5)
		return (fprintf(stderr, ARGS_USAGE, argv[0]), 0);
	if (!has_extension(argv[1], ".obj"))
	{
		fprintf(stderr, "Error: '%s' is not a .obj file\n", argv[1]);
		return (0);
	}
	app->obj_path = argv[1];
	size_at = texture_arg(app, argc, argv);
	if (size_at < 0)
		return (0);
	app->width = WIN_WIDTH;
	app->height = WIN_HEIGHT;
	if (argc - size_at == 2)
	{
		app->width = resolve_dim(argv[size_at], width, "Width");
		app->height = resolve_dim(argv[size_at + 1], height, "Height");
	}
	return (1);
}
