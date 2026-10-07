#include "scop.h"

/* Uncompressed BMP files of 24 or 32 bits per pixel, bottom-up or top-down, with any header from BITMAPINFOHEADER to V5. */

static uint32_t	read_u32(const uint8_t *p)
{
	return ((uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24);
}

static uint16_t	read_u16(const uint8_t *p)
{
	return ((uint16_t)(p[0] | p[1] << 8));
}

static int	bmp_error(const char *path, const char *message)
{
	fprintf(stderr, "Error: %s: %s\n", path, message);
	return (0);
}

/* A BI_BITFIELDS mask must select one whole byte of the pixel; that byte is the channel. */
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
	const uint32_t	compression = read_u32(bmp->data + 30);
	int				i;

	if (bits != 24 && bits != 32)
		return (bmp_error(path, "unsupported BMP: 24 or 32 bits per pixel expected"));
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
	i = 0;
	while (i < 3)
	{
		if (!mask_channel(read_u32(bmp->data + BMP_FILE_HEADER + BMP_INFO_HEADER + 4 * i), &bmp->channel[i]))
			return (bmp_error(path, "unsupported BMP: color masks are not whole bytes"));
		i++;
	}
	return (1);
}

static int	read_header(t_bmp *bmp, t_image *image, const char *path)
{
	int32_t	height;

	if (bmp->size < BMP_FILE_HEADER + BMP_INFO_HEADER || bmp->data[0] != 'B' || bmp->data[1] != 'M')
		return (bmp_error(path, "not a BMP file"));
	if (read_u32(bmp->data + BMP_FILE_HEADER) < BMP_INFO_HEADER)
		return (bmp_error(path, "unsupported BMP: header older than BITMAPINFOHEADER"));
	image->width = (int32_t)read_u32(bmp->data + 18);
	height = (int32_t)read_u32(bmp->data + 22);
	bmp->top_down = height < 0;
	if (image->width <= 0 || height == 0 || height == INT32_MIN)
		return (bmp_error(path, "invalid BMP size"));
	image->height = height < 0 ? -height : height;
	if (image->width > BMP_MAX_SIDE || image->height > BMP_MAX_SIDE)
		return (bmp_error(path, "BMP larger than 16384 pixels per side"));
	if (read_u16(bmp->data + 26) != 1)
		return (bmp_error(path, "invalid BMP: one plane expected"));
	if (!read_format(bmp, path, read_u16(bmp->data + 28)))
		return (0);
	bmp->offset = read_u32(bmp->data + 10);
	bmp->stride = ((size_t)image->width * (size_t)bmp->bytes + 3) / 4 * 4;
	if (bmp->offset > bmp->size || bmp->stride * (size_t)image->height > bmp->size - bmp->offset)
		return (bmp_error(path, "truncated BMP: pixel data missing"));
	return (1);
}

/* Rows come out bottom first; a top-down file is read from its last row. */
static void	decode(const t_bmp *bmp, t_image *image)
{
	const uint8_t	*src;
	uint8_t			*dst;
	int				row;
	int				x;

	dst = image->pixels;
	row = 0;
	while (row < image->height)
	{
		src = bmp->data + bmp->offset + bmp->stride * (size_t)(bmp->top_down ? image->height - 1 - row : row);
		x = 0;
		while (x++ < image->width)
		{
			*dst++ = src[bmp->channel[0]];
			*dst++ = src[bmp->channel[1]];
			*dst++ = src[bmp->channel[2]];
			*dst++ = 255;
			src += bmp->bytes;
		}
		row++;
	}
}

/* 1 with the pixels in image; 0 after an error message naming the file. */
int	bmp_load(const char *path, t_image *image)
{
	t_bmp	bmp;
	uint8_t	*data;
	int		ok;

	memset(image, 0, sizeof(*image));
	memset(&bmp, 0, sizeof(bmp));
	data = (uint8_t *)file_read(path, &bmp.size);
	if (!data)
		return (0);
	bmp.data = data;
	ok = read_header(&bmp, image, path);
	if (ok)
	{
		image->pixels = malloc((size_t)image->width * (size_t)image->height * 4);
		ok = image->pixels != NULL;
		if (!ok)
			fprintf(stderr, "Error: out of memory\n");
	}
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
