#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>
#include <stdint.h>

#define BMP_FILE_HEADER		14
#define BMP_INFO_HEADER		40
#define BMP_MASKS			54
#define BMP_MASKS_END		66
#define BMP_BI_RGB			0
#define BMP_BI_BITFIELDS	3
#define BMP_MAX_SIDE		16384

/* RGBA pixels, rows from bottom to top as OpenGL reads them */
typedef struct s_image
{
	int		width;
	int		height;
	uint8_t	*pixels;
}	t_image;

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

int		bmp_load(const char *path, t_image *image);
void	image_free(t_image *image);

#endif
