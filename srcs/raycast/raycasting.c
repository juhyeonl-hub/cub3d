/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunjkim <hyunjkim@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:28:44 by hyunjkim          #+#    #+#             */
/*   Updated: 2025/11/19 15:28:45 by hyunjkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	run_dda(t_ray *ray, t_map_config *config)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= config->map_height || ray->map_x < 0
			|| ray->map_x >= (int)ft_strlen(config->map_grid[ray->map_y]))
		{
			ray->hit = 1;
			break ;
		}
		if (config->map_grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

static void	get_wall_dist(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->map_x - player->pos.x + (1 - ray->step_x)
				/ 2) / ray->dir.x;
	}
	else
	{
		ray->perp_wall_dist = (ray->map_y - player->pos.y + (1 - ray->step_y)
				/ 2) / ray->dir.y;
	}
}

static void	get_texture_x(t_game *game, t_ray *ray, t_player *player)
{
	double	x_wall;
	int		tex_width;

	tex_width = game->textures[NORTH]->width;
	if (ray->side == 0)
		x_wall = player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		x_wall = player->pos.x + ray->perp_wall_dist * ray->dir.x;
	x_wall -= floor(x_wall);
	ray->tex_x = (int)(x_wall * (double)tex_width);
	if (ray->side == 0 && ray->dir.x > 0)
		ray->tex_x = tex_width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir.y < 0)
		ray->tex_x = tex_width - ray->tex_x - 1;
}

void	raycasting(t_game *game)
{
	int	x;
	int	line_height;
	int	draw_start;
	int	draw_end;

	x = 0;
	while (x < WIN_WIDTH)
	{
		setup_ray(&game->ray, &game->player, x);
		run_dda(&game->ray, &game->config);
		get_wall_dist(&game->ray, &game->player);
		line_height = (int)(WIN_HEIGHT / game->ray.perp_wall_dist);
		draw_start = -line_height / 2 + WIN_HEIGHT / 2;
		draw_end = line_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= WIN_HEIGHT)
			draw_end = WIN_HEIGHT - 1;
		get_texture_x(game, &game->ray, &game->player);
		draw_textured_walls(game, x, draw_start, draw_end);
		x++;
	}
	mlx_image_to_window(game->mlx_ptr, game->screen_buffer, 0, 0);
}
