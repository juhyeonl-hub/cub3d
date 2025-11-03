/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 12:24:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/03 13:02:35 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

static void	init_game_struct(t_game *game)
{
	// ft_memset(game, 0, sizeof(t_game));
	// ft_calloc(1, sizeof(t_game)) 을 main에서 사용하는 것을 권장
}

static int exit_game(t_game *game)
{
	free_game_config(&game->config);
	// MLX Texture, image, window, memory need free
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
	// 2. MLX 초기화 (구현 필요)
	// game.mlx_ptr = mlx_init(...);

	// 3. 텍스처 로드 (구현 필요)
	// game.textures[NORTH] = mlx_load_xpm42(game.config.tex_paths[NORTH]);

	// 4. 렌더링 및 훅 설정 (구현 필요)
	// mlx_loop_hook(game.mlx_ptr, game_loop_hook, &game);
	// mlx_loop(game.mlx_ptr);

	// 5. 게임 종료 및 메모리 해제
	return (exit_game(&game));
}
