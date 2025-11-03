/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* parse() 메인 함수가 위치하며, 파싱의 전체 흐름을 제어합니다. */
/* Updated: 2025/11/02 21:50:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

// 헬퍼 함수 프로토타입 (parse_elements.c, parse_map.c 등에 위치)
char	*parse_elements(int fd, t_game *game);
int		parse_map_grid(int fd, char *first_map_line, t_game *game);
int		validate_config(t_game *game);
void	init_config(t_map_config *config);

/*
 * 파싱 메인 함수
 * t_game 포인터를 받아 config 필드를 채웁니다.
 */
int	parse(char *filename, t_game *game)
{
	int		fd;
	char	*first_map_line;

	// 1. .cub 확장자 검사
	if (ft_strnrcmp(filename, ".cub", 4) != 0 || \
		(filename[ft_strlen(filename) - 5] == '/'))
		return (ft_perror("Error: Invalid file extension (must be .cub)\n"));

	// 2. 파일 열기
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_perror("Error: Cannot open file\n"));

	// 3. config 구조체 초기화
	init_config(&game->config);

	// 4. (Pass 1) 요소 파싱 (텍스처, 색상)
	// 맵의 첫 번째 줄을 반환받습니다.
	first_map_line = parse_elements(fd, game);
	if (!first_map_line)
	{
		close(fd);
		return (ft_perror("Error: Map data not found or invalid element\n"));
	}

	// 5. (Pass 2) 맵 그리드 파싱
	if (parse_map_grid(fd, first_map_line, game) != 0)
	{
		close(fd);
		return (1); // 오류 메시지는 내부 함수가 출력
	}
	close(fd);

	// 6. (Pass 3) 최종 유효성 검사
	if (validate_config(game) != 0)
		return (1); // 오류 메시지는 내부 함수가 출력

	return (0); // 파싱 성공
}

/*
 * game->config 구조체를 안전한 초기값으로 설정합니다.
 */
void	init_config(t_map_config *config)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		config->tex_paths[i] = NULL;
		i++;
	}
	config->floor_color = -1; // -1을 "설정되지 않음"으로 사용
	config->ceiling_color = -1;
	config->map_grid = NULL;
	config->map_width = 0;
	config->map_height = 0;
	config->start_dir = '\0';
	config->start_pos.x = 0;
	config->start_pos.y = 0;
}
