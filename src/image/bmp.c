#include "scop.h"

static int	bmp_error(const char *path, const char *message)
{
	fprintf(stderr, "Error: %s: %s\n", path, message);
	return (0);
}

static uint16_t	read_u16(const uint8_t *p)
{
	return ((uint16_t)(p[0] | p[1] << 8));
}

static uint32_t	read_u32(const uint8_t *p)
{
	return ((uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24);
}

/* A BI_BITFIELDS mask must select one whole byte, and that byte is the channel */
static int	mask_channel(uint32_t mask, int *channel)
{
	int	k;

	for (k = 0; k < 4; k++)
	{
		if (mask == (uint32_t)0xFF << (8 * k))
		{
			*channel = k;
			return (1);
		}
	}
	return (0);
}

static int	read_format(t_bmp *bmp, const char *path, int bits)
{
	const uint32_t	compression = read_u32(bmp->data + 30);
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
	for (i = 0; i < 3; i++)
	{
		if (!mask_channel(read_u32(bmp->data + BMP_MASKS + 4 * i), &bmp->channel[i]))
			return (bmp_error(path, "unsupported BMP: masks are not bytes"));
	}
	return (1);
}

static int	read_size(t_bmp *bmp, t_image *image, const char *path)
{
	const int32_t	height = (int32_t)read_u32(bmp->data + 22);

	image->width = (int32_t)read_u32(bmp->data + 18);
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

static int	read_header(t_bmp *bmp, t_image *image, const char *path)
{
	size_t	pixels;

	if (bmp->size < BMP_FILE_HEADER + BMP_INFO_HEADER || bmp->data[0] != 'B' || bmp->data[1] != 'M')
		return (bmp_error(path, "not a BMP file"));
	if (read_u32(bmp->data + BMP_FILE_HEADER) < BMP_INFO_HEADER)
		return (bmp_error(path, "unsupported BMP: header too old"));
	if (!read_size(bmp, image, path))
		return (0);
	if (read_u16(bmp->data + 26) != 1)
		return (bmp_error(path, "invalid BMP: one plane expected"));
	if (!read_format(bmp, path, read_u16(bmp->data + 28)))
		return (0);
	bmp->offset = read_u32(bmp->data + 10);
	bmp->stride = ((size_t)image->width * bmp->bytes + 3) / 4 * 4;
	pixels = bmp->stride * (size_t)image->height;
	if (bmp->offset > bmp->size || pixels > bmp->size - bmp->offset)
		return (bmp_error(path, "truncated BMP: pixel data missing"));
	return (1);
}

/* Rows come out bottom first, so a top-down file is read from its last row */
static void	decode(const t_bmp *bmp, t_image *image)
{
	const uint8_t	*src;
	uint8_t			*dst;
	int				row;
	int				from;
	int				x;

	dst = image->pixels;
	for (row = 0; row < image->height; row++)
	{
		from = row;
		if (bmp->top_down)
			from = image->height - 1 - row;
		src = bmp->data + bmp->offset + bmp->stride * (size_t)from;
		for (x = 0; x < image->width; x++)
		{
			*dst++ = src[bmp->channel[0]];
			*dst++ = src[bmp->channel[1]];
			*dst++ = src[bmp->channel[2]];
			*dst++ = 255;
			src += bmp->bytes;
		}
	}
}

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
	ok = read_header(&bmp, image, path);
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
