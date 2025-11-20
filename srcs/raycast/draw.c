/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
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

static mlx_texture_t	*select_wall_texture(t_game *game)
{
	if (game->ray.side == 0)
	{
		if (game->ray.dir.x > 0)
			return (game->textures[EAST]);
		else
			return (game->textures[WEST]);
	}
	else
	{
		if (game->ray.dir.y > 0)
			return (game->textures[SOUTH]);
		else
			return (game->textures[NORTH]);
	}
}

static void	draw_ceiling(t_game *game, int x, int draw_start)
{
	int	y;

	y = 0;
	while (y < draw_start)
	{
		mlx_put_pixel(game->screen_buffer, x, y, game->config.ceiling_color);
		y++;
	}
}

static void	draw_floor(t_game *game, int x, int draw_end)
{
	int	y;

	y = draw_end;
	while (y < WIN_HEIGHT)
	{
		mlx_put_pixel(game->screen_buffer, x, y, game->config.floor_color);
		y++;
	}
}

void	draw_textured_walls(t_game *game, int x, int draw_start, int draw_end)
{
	int				y;
	int				color;
	int				line_height;
	double			step;
	double			tex_pos;
	mlx_texture_t	*texture;

	texture = select_wall_texture(game);
	line_height = draw_end - draw_start;
	step = (double)texture->height / (double)line_height;
	tex_pos = (draw_start - WIN_HEIGHT / 2 + line_height / 2) * step;
	draw_ceiling(game, x, draw_start);
	y = draw_start;
	while (y < draw_end)
	{
		game->ray.tex_y = (int)tex_pos % texture->height;
		tex_pos += step;
		color = get_texture_color(texture, game->ray.tex_x, game->ray.tex_y);
		mlx_put_pixel(game->screen_buffer, x, y, color);
		y++;
	}
	draw_floor(game, x, draw_end);
}
