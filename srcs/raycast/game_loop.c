/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunjkim <hyunjkim@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:28:29 by hyunjkim          #+#    #+#             */
/*   Updated: 2025/11/19 15:28:30 by hyunjkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	game_loop(void *param)
{
	t_game			*game;
	static double	old_time = 0;
	double			time;
	double			frame_time;

	game = (t_game *)param;
	time = mlx_get_time();
	if (old_time == 0)
		old_time = time;
	frame_time = time - old_time;
	old_time = time;
	if (mlx_is_key_down(game->mlx_ptr, MLX_KEY_ESCAPE))
	{
		mlx_close_window(game->mlx_ptr);
		return ;
	}
	player_movement(game, frame_time);
	raycasting(game);
}
