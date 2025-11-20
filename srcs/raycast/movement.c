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

static void	move_forward(t_game *game, double move_speed)
{
	int	new_x;
	int	new_y;
	int	cur_x;
	int	cur_y;

	new_x = (int)(game->player.pos.x + game->player.dir.x * move_speed);
	cur_y = (int)(game->player.pos.y);
	if (game->config.map_grid[cur_y][new_x] == '0')
		game->player.pos.x += game->player.dir.x * move_speed;
	cur_x = (int)(game->player.pos.x);
	new_y = (int)(game->player.pos.y + game->player.dir.y * move_speed);
	if (game->config.map_grid[new_y][cur_x] == '0')
		game->player.pos.y += game->player.dir.y * move_speed;
}

static void	move_backward(t_game *game, double move_speed)
{
	int	new_x;
	int	new_y;
	int	cur_x;
	int	cur_y;

	new_x = (int)(game->player.pos.x - game->player.dir.x * move_speed);
	cur_y = (int)(game->player.pos.y);
	if (game->config.map_grid[cur_y][new_x] == '0')
		game->player.pos.x -= game->player.dir.x * move_speed;
	cur_x = (int)(game->player.pos.x);
	new_y = (int)(game->player.pos.y - game->player.dir.y * move_speed);
	if (game->config.map_grid[new_y][cur_x] == '0')
		game->player.pos.y -= game->player.dir.y * move_speed;
}

static void	move_left(t_game *game, double move_speed)
{
	int	new_x;
	int	new_y;
	int	cur_x;
	int	cur_y;

	new_x = (int)(game->player.pos.x - game->player.plane.x * move_speed);
	cur_y = (int)(game->player.pos.y);
	if (game->config.map_grid[cur_y][new_x] == '0')
		game->player.pos.x -= game->player.plane.x * move_speed;
	cur_x = (int)(game->player.pos.x);
	new_y = (int)(game->player.pos.y - game->player.plane.y * move_speed);
	if (game->config.map_grid[new_y][cur_x] == '0')
		game->player.pos.y -= game->player.plane.y * move_speed;
}

static void	move_right(t_game *game, double move_speed)
{
	int	new_x;
	int	new_y;
	int	cur_x;
	int	cur_y;

	new_x = (int)(game->player.pos.x + game->player.plane.x * move_speed);
	cur_y = (int)(game->player.pos.y);
	if (game->config.map_grid[cur_y][new_x] == '0')
		game->player.pos.x += game->player.plane.x * move_speed;
	cur_x = (int)(game->player.pos.x);
	new_y = (int)(game->player.pos.y + game->player.plane.y * move_speed);
	if (game->config.map_grid[new_y][cur_x] == '0')
		game->player.pos.y += game->player.plane.y * move_speed;
}

void	player_movement(t_game *game, double frame_time)
{
	double	move_speed;
	double	rotation_speed;

	move_speed = frame_time * 5.0;
	rotation_speed = frame_time * 3.0;
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_A))
		move_left(game, move_speed);
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_D))
		move_right(game, move_speed);
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_W))
		move_forward(game, move_speed);
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_S))
		move_backward(game, move_speed);
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_LEFT))
		rotate_view(game, rotation_speed, -1);
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_RIGHT))
		rotate_view(game, rotation_speed, 1);
}
