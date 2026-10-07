/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:51:47 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* BMP files of 24 or 32 bits per pixel, into RGBA rows from the bottom up */

int	bmp_error(const char *path, const char *message)
{
	fprintf(stderr, "Error: %s: %s\n", path, message);
	return (0);
}

uint32_t	bmp_u32(const uint8_t *p)
{
	return ((uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16
		| (uint32_t)p[3] << 24);
}

/* Rows come out bottom first; a top-down file is read from its last row */
static void	decode(const t_bmp *bmp, t_image *image)
{
	const uint8_t	*src;
	uint8_t			*dst;
	int				row;
	int				from;
	int				x;

	dst = image->pixels;
	row = -1;
	while (++row < image->height)
	{
		from = row;
		if (bmp->top_down)
			from = image->height - 1 - row;
		src = bmp->data + bmp->offset + bmp->stride * (size_t)from;
		x = -1;
		while (++x < image->width)
		{
			*dst++ = src[bmp->channel[0]];
			*dst++ = src[bmp->channel[1]];
			*dst++ = src[bmp->channel[2]];
			*dst++ = 255;
			src += bmp->bytes;
		}
	}
}

/* 1 with the pixels in image; 0 after an error message naming the file */
int	bmp_load(const char *path, t_image *image)
{
	t_bmp	bmp;
	char	*data;
	int		ok;

	memset(image, 0, sizeof(*image));
	memset(&bmp, 0, sizeof(bmp));
	data = file_read(path, &bmp.size);
	if (!data)
		return (0);
	bmp.data = (const uint8_t *)data;
	ok = bmp_read_header(&bmp, image, path);
	if (ok)
		image->pixels = malloc((size_t)image->width * image->height * 4);
	if (ok && !image->pixels)
		ok = bmp_error(path, "out of memory");
	if (ok)
		decode(&bmp, image);
	free(data);
	if (!ok)
		image_free(image);
	return (ok);
}

void	image_free(t_image *image)
{
	free(image->pixels);
	memset(image, 0, sizeof(*image));
}
