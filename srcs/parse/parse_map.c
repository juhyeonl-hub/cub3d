/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_map.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/22 14:16:10 by juhyeonl          #+#    #+#             */
/* Updated: 2025/10/22 15:00:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>
#include <unistd.h>

// --- 보조 함수들 ---

// config 구조체를 0으로 초기화하는 함수
static void init_config(t_config *config)
{
    ft_memset(config, 0, sizeof(t_config));
}

// 색상 문자열(R,G,B)을 파싱하여 int로 변환하는 함수
static int parse_color(char *line, int *color_out)
{
    char    **rgb_str;
    int     rgb[3];
    
    // ft_split 같은 함수로 쉼표(,)를 기준으로 나눔
    rgb_str = ft_split(line, ',');
    if (!rgb_str || !rgb_str[0] || !rgb_str[1] || !rgb_str[2] || rgb_str[3])
    {
        // ft_free_split(rgb_str);
        return (1);
    }
    rgb[0] = ft_atoi(rgb_str[0]); // R
    rgb[1] = ft_atoi(rgb_str[1]); // G
    rgb[2] = ft_atoi(rgb_str[2]); // B
    // ft_free_split(rgb_str);

    if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255 || \
        rgb[2] < 0 || rgb[2] > 255)
        return (1);
    
    // R, G, B 값을 하나의 int로 합침 (비트 연산)
    *color_out = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
    return (0);
}

// 텍스처와 색상 정보를 파싱하는 함수
static int parse_element(char *line, t_config *config)
{
    char **parts;

    parts = ft_split(line, ' '); // 공백을 기준으로 식별자와 값 분리
    if (!parts || !parts[0] || !parts[1] || parts[2])
    {
        // ft_free_split(parts);
        return (ft_perror("Error: Invalid element format\n"));
    }
    if (ft_strncmp(parts[0], "NO", 3) == 0)
        config->north_path = ft_strdup(parts[1]);
    else if (ft_strncmp(parts[0], "SO", 3) == 0)
        config->south_path = ft_strdup(parts[1]);
    else if (ft_strncmp(parts[0], "WE", 3) == 0)
        config->west_path = ft_strdup(parts[1]);
    else if (ft_strncmp(parts[0], "EA", 3) == 0)
        config->east_path = ft_strdup(parts[1]);
    else if (ft_strncmp(parts[0], "F", 2) == 0)
    {
        if (parse_color(parts[1], &config->floor_color))
            return (ft_perror("Error: Invalid floor color\n"));
    }
    else if (ft_strncmp(parts[0], "C", 2) == 0)
    {
        if (parse_color(parts[1], &config->ceiling_color))
            return (ft_perror("Error: Invalid ceiling color\n"));
    }
    // ft_free_split(parts);
    return (0);
}

// --- 메인 파싱 함수 ---

int parse_map(char **av, t_game *game)
{
    int     fd;
    char    *line;

    // 1. 맵 유효성 검사 (확장자, 문자, 벽 등)
    //    성공하면 game->config.map에 맵 데이터가 할당됨
    if (map_val(av, &game->config.map))
        return (1);
    
    init_config(&game->config); // game->config 구조체 초기화

    // 2. 텍스처와 색상 정보 파싱
    fd = open(av[1], O_RDONLY);
    if (fd < 0)
        return (ft_perror("Error: Cannot open map file\n"));
    
    line = get_next_line(fd);
    while (line)
    {
        // 빈 줄은 건너뜀
        if (*line == '\n')
        {
            free(line);
            line = get_next_line(fd);
            continue;
        }
        // 맵 라인이 시작되면 파싱 중단 (맵은 항상 마지막에 와야 함)
        if (ft_strchr(line, '1') || ft_strchr(line, '0'))
        {
            free(line);
            break;
        }
        if (parse_element(line, &game->config))
        {
            free(line);
            close(fd);
            return (1); // 파싱 중 오류 발생
        }
        free(line);
        line = get_next_line(fd);
    }
    close(fd);

    // 3. 모든 필수 요소가 파싱되었는지 확인 (구현 필요)
    // if (!config->north_path || ... || !config->floor_color)
    //     return (ft_perror("Error: Missing map elements\n"));

    // 4. 플레이어 위치 찾고 맵에서 '0'으로 변경 (구현 필요)
    // find_player_position(&game->config);

    return (0); // 파싱 성공
}