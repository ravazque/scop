/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_header.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:36:17 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* BMP headers: size, pixel format and where the pixel rows are */

static uint16_t	read_u16(const uint8_t *p)
{
	return ((uint16_t)(p[0] | p[1] << 8));
}

/* A BI_BITFIELDS mask must select one whole byte, which is the channel */
static int	mask_channel(uint32_t mask, int *channel)
{
	int	k;

	k = 0;
	while (k < 4)
	{
		if (mask == (uint32_t)0xFF << (8 * k))
		{
			*channel = k;
			return (1);
		}
		k++;
	}
	return (0);
}

static int	read_format(t_bmp *bmp, const char *path, int bits)
{
	const uint32_t	compression = bmp_u32(bmp->data + 30);
	int				i;

	if (bits != 24 && bits != 32)
		return (bmp_error(path, "unsupported BMP: 24 or 32 bits expected"));
	bmp->bytes = bits / 8;
	bmp->channel[0] = 2;
	bmp->channel[1] = 1;
	bmp->channel[2] = 0;
	if (compression == BMP_BI_RGB)
		return (1);
	if (compression != BMP_BI_BITFIELDS || bits != 32)
		return (bmp_error(path, "unsupported BMP: compressed pixels"));
	if (bmp->size < BMP_MASKS_END)
		return (bmp_error(path, "truncated BMP: missing color masks"));
	i = -1;
	while (++i < 3)
	{
		if (!mask_channel(bmp_u32(bmp->data + BMP_MASKS + 4 * i),
				&bmp->channel[i]))
			return (bmp_error(path, "unsupported BMP: masks are not bytes"));
	}
	return (1);
}

static int	read_size(t_bmp *bmp, t_image *image, const char *path)
{
	int32_t	height;

	image->width = (int32_t)bmp_u32(bmp->data + 18);
	height = (int32_t)bmp_u32(bmp->data + 22);
	bmp->top_down = height < 0;
	if (image->width <= 0 || height == 0 || height == INT32_MIN)
		return (bmp_error(path, "invalid BMP size"));
	image->height = height;
	if (height < 0)
		image->height = -height;
	if (image->width > BMP_MAX_SIDE || image->height > BMP_MAX_SIDE)
		return (bmp_error(path, "BMP larger than 16384 pixels per side"));
	return (1);
}

/* Accepts BITMAPINFOHEADER to V5, uncompressed or with byte masks */
int	bmp_read_header(t_bmp *bmp, t_image *image, const char *path)
{
	size_t	pixels;

	if (bmp->size < BMP_FILE_HEADER + BMP_INFO_HEADER
		|| bmp->data[0] != 'B' || bmp->data[1] != 'M')
		return (bmp_error(path, "not a BMP file"));
	if (bmp_u32(bmp->data + BMP_FILE_HEADER) < BMP_INFO_HEADER)
		return (bmp_error(path, "unsupported BMP: header too old"));
	if (!read_size(bmp, image, path))
		return (0);
	if (read_u16(bmp->data + 26) != 1)
		return (bmp_error(path, "invalid BMP: one plane expected"));
	if (!read_format(bmp, path, read_u16(bmp->data + 28)))
		return (0);
	bmp->offset = bmp_u32(bmp->data + 10);
	bmp->stride = ((size_t)image->width * bmp->bytes + 3) / 4 * 4;
	pixels = bmp->stride * (size_t)image->height;
	if (bmp->offset > bmp->size || pixels > bmp->size - bmp->offset)
		return (bmp_error(path, "truncated BMP: pixel data missing"));
	return (1);
}
