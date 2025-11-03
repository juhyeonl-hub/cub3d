/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* validate_config.c                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* 3단계(Pass 3): 파싱된 모든 config 데이터를 최종 검증합니다. */
/* Updated: 2025/11/02 21:55:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

// 헬퍼 함수 프로토타입
static int	validate_elements(t_map_config *config);
static int	find_player(t_map_config *config);
static int	validate_map_chars(t_map_config *config);
static int	validate_map_walls(t_game *game);

/*
 * 파싱이 완료된 t_game 구조체를 받아 최종 유효성을 검사합니다.
 */
int	validate_config(t_game *game)
{
	if (validate_elements(&game->config) != 0)
		return (1);
	
	if (find_player(&game->config) != 0)
		return (1);

	if (validate_map_chars(&game->config) != 0)
		return (1);
	
	if (validate_map_walls(game) != 0)
		return (1);

	return (0); // 모든 검증 통과
}

/*
 * 1. 텍스처와 색상 요소가 모두 파싱되었는지 확인합니다.
 */
static int	validate_elements(t_map_config *config)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (config->tex_paths[i] == NULL)
			return (ft_perror("Error: Missing texture definition\n"));
		i++;
	}
	if (config->floor_color == -1)
		return (ft_perror("Error: Missing floor color definition\n"));
	if (config->ceiling_color == -1)
		return (ft_perror("Error: Missing ceiling color definition\n"));
	return (0);
}

/*
 * 2. 맵을 순회하며 플레이어(N,S,E,W)를 찾습니다.
 * - 정확히 1명이어야 합니다.
 * - 찾은 위치와 방향을 config에 저장합니다.
 * - 맵 그리드의 해당 위치를 '0'으로 변경합니다.
 */
static int	find_player(t_map_config *config)
{
	int	y;
	int	x;

	y = 0;
	while (y < config->map_height)
	{
		x = 0;
		while (config->map_grid[y][x])
		{
			if (ft_strchr("NSEW", config->map_grid[y][x]))
			{
				if (config->start_dir != '\0') // 플레이어가 2명 이상
					return (ft_perror("Error: Multiple players found in map\n"));
				
				config->start_dir = config->map_grid[y][x];
				config->start_pos.x = (double)x + 0.5; // 타일 중앙
				config->start_pos.y = (double)y + 0.5;
				config->map_grid[y][x] = '0'; // 맵을 빈 공간으로 변경
			}
			x++;
		}
		y++;
	}
	if (config->start_dir == '\0') // 플레이어가 없음
		return (ft_perror("Error: No player found in map\n"));
	return (0);
}

/*
 * 3. 맵에 유효하지 않은 문자가 있는지 확인합니다.
 * (플레이어를 찾은 후 호출되므로 '0', '1', ' '만 유효)
 */
static int	validate_map_chars(t_map_config *config)
{
	int	y;
	int	x;

	y = 0;
	while (y < config->map_height)
	{
		x = 0;
		while (config->map_grid[y][x])
		{
			if (!ft_strchr("01 ", config->map_grid[y][x]))
				return (ft_perror("Error: Invalid character in map\n"));
			x++;
		}
		// 맵 중간의 빈 줄(길이가 0인 라인)은 오류
		if (x == 0 && y != config->map_height - 1)
			return (ft_perror("Error: Empty line inside map\n"));
		y++;
	}
	return (0);
}

/*
 * 4. 맵이 벽으로 둘러싸여 있는지 검사합니다. (Flood Fill 알고리즘)
 */
// DFS(Flood Fill)를 위한 헬퍼 구조체
typedef struct s_flood_fill {
	char	**map_copy;
	int		height;
	int		width;
	int		is_open;
}	t_flood_fill;

// DFS 재귀 함수
static void	dfs_check(t_flood_fill *ff, int y, int x)
{
	// 1. 경계 검사 (맵 밖으로 나감)
	if (y < 0 || y >= ff->height || x < 0 || x >= (int)ft_strlen(ff->map_copy[y]))
	{
		ff->is_open = 1; // 맵이 열려있음
		return ;
	}
	
	// 2. 방문했거나 벽('1')이면 중단
	if (ff->map_copy[y][x] == '1' || ff->map_copy[y][x] == 'V')
		return ;

	// 3. 공백(' ')을 만나면 열려있음
	if (ff->map_copy[y][x] == ' ')
	{
		ff->is_open = 1;
		return ;
	}
	
	// 4. 방문 처리 ('V')
	ff->map_copy[y][x] = 'V';

	// 5. 4방향 탐색
	dfs_check(ff, y + 1, x);
	dfs_check(ff, y - 1, x);
	dfs_check(ff, y, x + 1);
	dfs_check(ff, y, x - 1);
}

// 맵 검증 메인 함수
static int	validate_map_walls(t_game *game)
{
	t_flood_fill	ff;
	int				i;

	ff.height = game->config.map_height;
	ff.width = game->config.map_width;
	ff.is_open = 0;
	
	// 맵 복사 (원본 맵을 수정하지 않기 위해)
	ff.map_copy = (char **)malloc(sizeof(char *) * (ff.height + 1));
	if (!ff.map_copy)
		return (ft_perror("Error: Malloc failed for wall validation\n"));
	i = 0;
	while (i < ff.height)
	{
		ff.map_copy[i] = ft_strdup(game->config.map_grid[i]);
		i++;
	}
	ff.map_copy[i] = NULL;
	
	// 플레이어 시작 위치에서 DFS 시작
	dfs_check(&ff, (int)game->config.start_pos.y, (int)game->config.start_pos.x);
	
	// 복사본 맵 해제
	free_split(ff.map_copy);

	if (ff.is_open)
		return (ft_perror("Error: Map is not surrounded by walls\n"));
	return (0);
}
