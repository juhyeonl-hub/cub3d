/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 12:24:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/05 09:40:40 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

static int exit_game(t_game *game)
{
	free_game_config(&game->config);
	return (1);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (ft_perror("Error: Invalid arguments\nUsage: ./cub3d <map.cub>\n"));
	ft_memset(&game, 0, sizeof(t_game));
	if (parse(av[1], &game) != 0)
		return (exit_game(&game));
	debug_print_game_struct(&game);	// for DEBUG

	return (exit_game(&game));
}
