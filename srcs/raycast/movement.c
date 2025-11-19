/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunjkim <hyunjkim@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:28:36 by hyunjkim          #+#    #+#             */
/*   Updated: 2025/11/19 15:28:37 by hyunjkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	player_movement(t_game *game, double frame_time)
{
	double	move_speed;
	double	rotation_speed;
	double	old_dir_x;
	double	old_plane_x;
	int		new_x;
	int		new_y;
	int		cur_x;
	int		cur_y;

	move_speed = frame_time * 5.0;
	rotation_speed = frame_time * 3.0;
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_A))
	{
		old_dir_x = game->player.dir.x;
		game->player.dir.x = game->player.dir.x * cos(-rotation_speed)
			- game->player.dir.y * sin(-rotation_speed);
		game->player.dir.y = old_dir_x * sin(-rotation_speed)
			+ game->player.dir.y * cos(-rotation_speed);
		old_plane_x = game->player.plane.x;
		game->player.plane.x = game->player.plane.x * cos(-rotation_speed)
			- game->player.plane.y * sin(-rotation_speed);
		game->player.plane.y = old_plane_x * sin(-rotation_speed)
			+ game->player.plane.y * cos(-rotation_speed);
	}
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_D))
	{
		old_dir_x = game->player.dir.x;
		game->player.dir.x = game->player.dir.x * cos(rotation_speed)
			- game->player.dir.y * sin(rotation_speed);
		game->player.dir.y = old_dir_x * sin(rotation_speed)
			+ game->player.dir.y * cos(rotation_speed);
		old_plane_x = game->player.plane.x;
		game->player.plane.x = game->player.plane.x * cos(rotation_speed)
			- game->player.plane.y * sin(rotation_speed);
		game->player.plane.y = old_plane_x * sin(rotation_speed)
			+ game->player.plane.y * cos(rotation_speed);
	}
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_W))
	{
		new_x = (int)(game->player.pos.x + game->player.dir.x * move_speed);
		cur_y = (int)(game->player.pos.y);
		if (game->config.map_grid[cur_y][new_x] == '0')
			game->player.pos.x += game->player.dir.x * move_speed;
		cur_x = (int)(game->player.pos.x);
		new_y = (int)(game->player.pos.y + game->player.dir.y * move_speed);
		if (game->config.map_grid[new_y][cur_x] == '0')
			game->player.pos.y += game->player.dir.y * move_speed;
	}
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_S))
	{
		new_x = (int)(game->player.pos.x - game->player.dir.x * move_speed);
		cur_y = (int)(game->player.pos.y);
		if (game->config.map_grid[cur_y][new_x] == '0')
			game->player.pos.x -= game->player.dir.x * move_speed;
		cur_x = (int)(game->player.pos.x);
		new_y = (int)(game->player.pos.y - game->player.dir.y * move_speed);
		if (game->config.map_grid[new_y][cur_x] == '0')
			game->player.pos.y -= game->player.dir.y * move_speed;
	}
}
