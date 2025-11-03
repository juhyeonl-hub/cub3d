/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_elements.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* 1단계(Pass 1): 텍스처, 색상 등 요소(Element)를 파싱합니다. */
/* Updated: 2025/11/02 21:50:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

// 헬퍼 함수 프로토타입 (parse_utils.c 에 위치)
int		parse_texture(char **parts, t_game *game);
int		parse_color(char **parts, t_game *game);
int		is_map_line(char *line);
void	free_split(char **split_arr);

/*
 * GNL로 파일(fd)을 읽어 텍스처/색상 요소를 파싱합니다.
 * 맵의 첫 번째 줄을 만나면 해당 라인을 반환합니다.
 */
char	*parse_elements(int fd, t_game *game)
{
	char	*line;
	char	**parts;
	int		parse_result;

	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL) // 맵 없이 파일이 끝남
			return (NULL);
		if (*line == '\n' || *line == '\0') // 빈 줄 건너뛰기
		{
			free(line);
			continue ;
		}
		if (is_map_line(line)) // 맵 라인을 만남
			return (line); // 1단계 종료, 맵의 첫 줄 반환

		parts = ft_split(line, ' ');
		free(line); // 라인은 더이상 필요 없음
		if (!parts)
			return (NULL); // Malloc Error
		if (ft_strncmp(parts[0], "NO", 3) == 0 || ft_strncmp(parts[0], "SO", 3) == 0 || \
			ft_strncmp(parts[0], "WE", 3) == 0 || ft_strncmp(parts[0], "EA", 3) == 0)
			parse_result = parse_texture(parts, game);
		else if (ft_strncmp(parts[0], "F", 2) == 0 || ft_strncmp(parts[0], "C", 2) == 0)
			parse_result = parse_color(parts, game);
		else
			parse_result = 1; // 알 수 없는 식별자
		
		free_split(parts);
		if (parse_result != 0) // 파싱 중 오류 발생 (중복, 형식 오류 등)
			return (NULL);
	}
}

/*
 * 텍스처 경로를 파싱하여 config에 저장합니다.
 */
int	parse_texture(char **parts, t_game *game)
{
	t_texture_index	idx;

	// 형식 검사 (예: "NO" "path/to/texture.xpm") -> 2개여야 함
	if (!parts[1] || parts[2])
		return (ft_perror("Error: Invalid texture format\n"));

	if (ft_strncmp(parts[0], "NO", 3) == 0)
		idx = NORTH;
	else if (ft_strncmp(parts[0], "SO", 3) == 0)
		idx = SOUTH;
	else if (ft_strncmp(parts[0], "WE", 3) == 0)
		idx = WEST;
	else // "EA"
		idx = EAST;

	// 중복 검사
	if (game->config.tex_paths[idx] != NULL)
		return (ft_perror("Error: Duplicate texture definition\n"));

	// 경로 저장
	game->config.tex_paths[idx] = ft_strdup(parts[1]);
	if (!game->config.tex_paths[idx])
		return (ft_perror("Error: Malloc failed\n"));
	return (0);
}

/*
 * R,G,B 색상 문자열을 파싱하여 int로 변환 후 config에 저장합니다.
 */
int	parse_color(char **parts, t_game *game)
{
	char	**rgb_values;
	int		rgb[3];
	int		color_int;

	// 형식 검사 (예: "F" "220,100,0") -> 2개여야 함
	if (!parts[1] || parts[2])
		return (ft_perror("Error: Invalid color format\n"));

	rgb_values = ft_split(parts[1], ',');
	if (!rgb_values || !rgb_values[0] || !rgb_values[1] || !rgb_values[2] || rgb_values[3])
	{
		free_split(rgb_values);
		return (ft_perror("Error: Invalid RGB format\n"));
	}
	
	// (추가 검증 필요: ft_atoi_safe, is_digit 등)
	rgb[0] = ft_atoi(rgb_values[0]); // R
	rgb[1] = ft_atoi(rgb_values[1]); // G
	rgb[2] = ft_atoi(rgb_values[2]); // B
	free_split(rgb_values);

	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255 || \
		rgb[2] < 0 || rgb[2] > 255)
		return (ft_perror("Error: RGB values out of range (0-255)\n"));

	// 0xRRGGBBAA (MLX42는 RGBA, A=255는 불투명)
	color_int = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255;

	if (ft_strncmp(parts[0], "F", 2) == 0)
	{
		if (game->config.floor_color != -1) // 중복 검사
			return (ft_perror("Error: Duplicate floor color definition\n"));
		game->config.floor_color = color_int;
	}
	else // "C"
	{
		if (game->config.ceiling_color != -1) // 중복 검사
			return (ft_perror("Error: Duplicate ceiling color definition\n"));
		game->config.ceiling_color = color_int;
	}
	return (0);
}
