/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunjkim <hyunjkim@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:28:21 by hyunjkim          #+#    #+#             */
/*   Updated: 2025/11/19 15:28:23 by hyunjkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_texture_color(mlx_texture_t *texture, int x, int y)
{
	int	index;
	int	r;
	int	g;
	int	b;
	int	a;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0xFF000000);
	index = (y * texture->width + x) * texture->bytes_per_pixel;
	r = texture->pixels[index];
	g = texture->pixels[index + 1];
	b = texture->pixels[index + 2];
	a = texture->pixels[index + 3];
	return (r << 24 | g << 16 | b << 8 | a);
}

void	draw_textured_column(t_game *game, t_vector *param, mlx_texture_t *texture, int draw_end)
{
	int	y;
	int	color;

	y = game->ray.map_y;
	while (y < draw_end)
	{
		game->ray.tex_y = (int)param->y % texture->height;
		param->y += param->x;
		color = get_texture_color(texture, game->ray.tex_x, game->ray.tex_y);
		mlx_put_pixel(game->screen_buffer, game->ray.map_x, y, color);
		y++;
	}
}

