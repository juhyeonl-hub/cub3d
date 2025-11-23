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

static int	check_and_resize(t_game *game)
{
	int	cur_width;
	int	cur_height;

	cur_width = game->mlx_ptr->width;
	cur_height = game->mlx_ptr->height;
	if (game->win_width != cur_width || game->win_height != cur_height)
	{
		game->win_width = cur_width;
		game->win_height = cur_height;
		mlx_delete_image(game->mlx_ptr, game->screen_buffer);
		game->screen_buffer = mlx_new_image(game->mlx_ptr, cur_width, cur_height);
		if (!game->screen_buffer)
			return (1);
		if (mlx_image_to_window(game->mlx_ptr, game->screen_buffer, 0, 0) < 0)
			return (1);
	}
	return (0);
}

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
	if (check_and_resize(game) != 0)
		return ;
	raycasting(game);
}
