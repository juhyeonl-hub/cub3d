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

static t_vector	init_texture_param(t_game *game, mlx_texture_t *texture, int draw_start, int draw_end)
{
	t_vector	param;
	int			line_height;
	
	line_height = draw_end - draw_start;
	param.x = (double)texture->height / (double)line_height;
	param.y = (draw_start - game->win_height / 2 + line_height / 2) * param.x;
	return (param);
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
	while (y < game->win_height)
	{
		mlx_put_pixel(game->screen_buffer, x, y, game->config.floor_color);
		y++;
	}
}

void	draw_all(t_game *game, int x, int draw_start, int draw_end)
{
	mlx_texture_t	*texture;
	t_vector	param;

	texture = select_wall_texture(game);
	param = init_texture_param(game, texture, draw_start, draw_end);
	draw_ceiling(game, x, draw_start);
	game->ray.map_y = draw_start;
	game->ray.map_x = x;
	draw_textured_column(game, &param, texture, draw_end);
	draw_floor(game, x, draw_end);
}
