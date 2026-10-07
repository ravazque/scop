/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:51:47 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

# include <stddef.h>		/* size_t */
# include <stdint.h>		/* uint8_t, uint16_t, uint32_t */

# define BMP_FILE_HEADER	14		/* "BM", size, reserved, pixel offset */
# define BMP_INFO_HEADER	40		/* BITMAPINFOHEADER, the smallest taken */
# define BMP_MASKS			54		/* BI_BITFIELDS masks, after or in it */
# define BMP_MASKS_END		66
# define BMP_BI_RGB			0
# define BMP_BI_BITFIELDS	3
# define BMP_MAX_SIDE		16384	/* least max size of a GL 4.1 texture */

/* RGBA pixels, rows from bottom to top as OpenGL reads them */
typedef struct s_image
{
	int		width;
	int		height;
	uint8_t	*pixels;
}	t_image;

/* Where the pixels of a BMP file are and how to read one */
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
int			bmp_load(const char *path, t_image *image);
void		image_free(t_image *image);
int			bmp_error(const char *path, const char *message);
uint32_t	bmp_u32(const uint8_t *p);

/* ---- src/image/bmp_header.c ---- */
int			bmp_read_header(t_bmp *bmp, t_image *image, const char *path);

#endif
