/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ravazque <ravazque@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 16:51:47 by ravazque          #+#    #+#             */
/*   Updated: 2026/10/07 18:36:17 by ravazque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/* Images on the GPU: mipmapped and repeating, bound to texture unit 0 */

unsigned int	texture_upload(const t_image *image)
{
	const t_gl		*g = gl();
	unsigned int	id;

	g->gen_textures(1, &id);
	g->active_texture(GL_TEXTURE0);
	g->bind_texture(GL_TEXTURE_2D, id);
	g->tex_image_2d(GL_TEXTURE_2D, 0, GL_RGBA8, image->width, image->height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	g->generate_mipmap(GL_TEXTURE_2D);
	g->tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	g->tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	g->tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	g->tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return (id);
}
