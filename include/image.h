#ifndef IMAGE_H
# define IMAGE_H

# include <stddef.h>		/* size_t */
# include <stdint.h>		/* uint8_t, uint16_t, uint32_t */

# define BMP_FILE_HEADER	14		/* "BM", file size, two reserved words, offset of the pixels */
# define BMP_INFO_HEADER	40		/* BITMAPINFOHEADER: the smallest header accepted (V4 and V5 extend it) */
# define BMP_MASKS_END		66		/* the three BI_BITFIELDS masks end here, after or inside the header */
# define BMP_BI_RGB			0
# define BMP_BI_BITFIELDS	3
# define BMP_MAX_SIDE		16384	/* OpenGL 4.1 guarantees 2D textures at least this wide */

/* RGBA pixels, rows from bottom to top as OpenGL reads them. */
typedef struct s_image
{
	int		width;
	int		height;
	uint8_t	*pixels;
}	t_image;

/* Where the pixels of a BMP file are and how to read one. */
typedef struct s_bmp
{
	const uint8_t	*data;
	size_t			size;
	size_t			offset;
	size_t			stride;
	int				bytes;
	int				channel[3];
	int				top_down;
}	t_bmp;

/* ---- src/image/bmp.c ---- */
int		bmp_load(const char *path, t_image *image);
void	image_free(t_image *image);

#endif
