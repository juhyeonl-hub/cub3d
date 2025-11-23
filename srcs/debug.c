/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:36:06 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/19 16:10:17 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	debug_print_vector(const char *title, t_vector v)
{
	printf("      %-20s: (x=%.3f, y=%.3f)\n", title, v.x, v.y);
}

static void	debug_print_map_config(t_map_config *config)
{
	int	y;

	printf("\n  [t_map_config]\n");
	printf("    --- 텍스처 경로 ---\n");
	printf("      NO: %s\n", config->tex_paths[NORTH]);
	printf("      SO: %s\n", config->tex_paths[SOUTH]);
	printf("      WE: %s\n", config->tex_paths[WEST]);
	printf("      EA: %s\n", config->tex_paths[EAST]);
	printf("    --- 색상 (int / hex) ---\n");
	printf("      Floor (F)  : %-10d (0x%X)\n", config->floor_color,
		config->floor_color);
	printf("      Ceiling (C): %-10d (0x%X)\n", config->ceiling_color,
		config->ceiling_color);
	printf("    --- 맵 정보 ---\n");
	printf("      맵 크기   : (width=%d, height=%d)\n", config->map_width,
		config->map_height);
	printf("      시작 방향 : %c\n", config->start_dir);
	debug_print_vector("시작 위치 (start_pos)", config->start_pos);
	printf("    --- 맵 그리드 (map_grid: %p) ---\n", (void *)config->map_grid);
	if (config->map_grid)
	{
		y = 0;
		while (y < config->map_height)
		{
			printf("      [%2d] \"%s\"\n", y, config->map_grid[y]);
			y++;
		}
	}
	else
		printf("      (map_grid is NULL)\n");
}

static void	debug_print_player(t_player *player)
{
	printf("\n  [t_player]\n");
	debug_print_vector("플레이어 위치 (pos)", player->pos);
	debug_print_vector("방향 벡터 (dir)", player->dir);
	debug_print_vector("카메라 평면 (plane)", player->plane);
}

static void	debug_print_ray(t_ray *ray)
{
	printf("\n  [t_ray] (파싱 직후에는 대부분 0)\n");
	debug_print_vector("방향 (dir)", ray->dir);
	debug_print_vector("Side Dist", ray->side_dist);
	debug_print_vector("Delta Dist", ray->delta_dist);
	printf("      Step      : (step_x=%d, step_y=%d)\n", ray->step_x,
		ray->step_y);
	printf("      Map Coords      : (map_x=%d, map_y=%d)\n", ray->map_x,
		ray->map_y);
	printf("      Perp Wall Dist  : %.3f\n", ray->perp_wall_dist);
	printf("      Hit / Side      : (hit=%d, side=%d)\n", ray->hit, ray->side);
	printf("      Texture Coords  : (tex_x=%d, tex_y=%d)\n", ray->tex_x,
		ray->tex_y);
}

void	debug_print_game_struct(t_game *game)
{
	if (!game)
	{
		printf("--- DEBUG: t_game 구조체가 NULL입니다 ---\n");
		return ;
	}
	printf("\n\n--- DEBUG: t_game 구조체 전체 덤프 (주소: %p) ---\n", (void *)game);
	printf("  [t_game 루트 멤버]\n");
	printf("    MLX 포인터     : %p\n", (void *)game->mlx_ptr);
	printf("    스크린 버퍼    : %p\n", (void *)game->screen_buffer);
	printf("    미니맵 버퍼    : %p\n", (void *)game->minimap_buffer);
	printf("    텍스처[NO]     : %p\n", (void *)game->textures[NORTH]);
	printf("    텍스처[SO]     : %p\n", (void *)game->textures[SOUTH]);
	printf("    텍스처[WE]     : %p\n", (void *)game->textures[WEST]);
	printf("    텍스처[EA]     : %p\n", (void *)game->textures[EAST]);
	printf("    --- 키 상태 ---\n");
	// printf("      W: %d, A: %d, S: %d, D: %d\n", game->key_w, game->key_a,
	// 	game->key_s, game->key_d);
	// printf("      L: %d, R: %d\n", game->key_left, game->key_right);
	printf("    --- 기타 ---\n");
	// printf("      미니맵 활성화  : %d\n", game->minimap_enabled);
	printf("      이전 마우스 X  : %.3f\n", game->prev_mouse_x);
	debug_print_map_config(&game->config);
	debug_print_player(&game->player);
	debug_print_ray(&game->ray);
	printf("\n--- END DEBUG: t_game 구조체 덤프 ---\n\n");
}
