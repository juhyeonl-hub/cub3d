/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 12:24:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/24 13:45:08 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	exit_game(t_game *game)
{
	free_game_config(&game->config);
	if (game->textures[NORTH])
		mlx_delete_texture(game->textures[NORTH]);
	if (game->textures[SOUTH])
		mlx_delete_texture(game->textures[SOUTH]);
	if (game->textures[EAST])
		mlx_delete_texture(game->textures[EAST]);
	if (game->textures[WEST])
		mlx_delete_texture(game->textures[WEST]);
	if (game->screen_buffer)
		mlx_delete_image(game->mlx_ptr, game->screen_buffer);
	if (game->mlx_ptr)
		mlx_terminate(game->mlx_ptr);
	return (1);
}

static int	load_textures(t_game *game)
{
	game->textures[NORTH] = mlx_load_png(game->config.tex_paths[NORTH]);
	if (!game->textures[NORTH])
		return (ft_perror("Error: Failed to load north texture\n"));
	game->textures[SOUTH] = mlx_load_png(game->config.tex_paths[SOUTH]);
	if (!game->textures[SOUTH])
		return (ft_perror("Error: Failed to load south texture\n"));
	game->textures[EAST] = mlx_load_png(game->config.tex_paths[EAST]);
	if (!game->textures[EAST])
		return (ft_perror("Error: Failed to load east texture\n"));
	game->textures[WEST] = mlx_load_png(game->config.tex_paths[WEST]);
	if (!game->textures[WEST])
		return (ft_perror("Error: Failed to load west texture\n"));
	return (0);
}

static int	validate_texture_sizes(t_game *game)
{
	uint32_t	width;
	uint32_t	height;

	width = game->textures[NORTH]->width;
	height = game->textures[NORTH]->height;
	if (game->textures[SOUTH]->width != width
		|| game->textures[SOUTH]->height != height
		|| game->textures[EAST]->width != width
		|| game->textures[EAST]->height != height
		|| game->textures[WEST]->width != width
		|| game->textures[WEST]->height != height)
		return (ft_perror("Error: All textures must be the same size\n"));
	return (0);
}

static int	init_graphics(t_game *game)
{
	game->mlx_ptr = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", true);
	if (!game->mlx_ptr)
		return (ft_perror("Error: MLX initialization failed\n"));
	game->win_width = WIN_WIDTH;
	game->win_height = WIN_HEIGHT;
	game->screen_buffer = mlx_new_image(game->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	if (!game->screen_buffer)
		return (ft_perror("Error: Failed to create screen buffer\n"));
	if (mlx_image_to_window(game->mlx_ptr, game->screen_buffer, 0, 0) < 0)
		return (ft_perror("Error: Failed to add image to window\n"));
	if (load_textures(game) != 0)
		return (1);
	if (validate_texture_sizes(game) != 0)
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (ft_perror(ERRMSG_NOT_ENOUGH_ARG));
	ft_memset(&game, 0, sizeof(t_game));
	if (parse(av[1], &game) != 0)
		return (exit_game(&game));
	if (init_graphics(&game) != 0)
		return (exit_game(&game));
	init_player_state(&game.player, &game.config);
	init_ray_data(&game.ray);
	mlx_loop_hook(game.mlx_ptr, &game_loop, &game);
	mlx_loop(game.mlx_ptr);
	return (exit_game(&game));
}
