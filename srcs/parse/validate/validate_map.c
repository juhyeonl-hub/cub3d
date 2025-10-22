/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* validate_map.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/22 11:59:17 by juhyeonl          #+#    #+#             */
/* Updated: 2025/10/22 14:48:11 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h> // open 함수
#include <unistd.h> // close 함수
#include <stdlib.h> // malloc, free 함수

// --- 이 파일에서만 사용할 보조 함수들 ---

// 파일의 라인 수를 세는 함수 (메모리 할당을 위해 필요)
static int count_lines(char *filename)
{
    int fd;
    int count;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    count = 0;
    line = get_next_line(fd);
    while (line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (count);
}

// 파일을 읽어 char** 2차원 배열로 만드는 함수
static char **read_map_lines(char *filename)
{
    int line_count;
    char **map;
    int fd;
    int i;

    line_count = count_lines(filename);
    if (line_count <= 0)
        return (NULL);
    map = (char **)malloc(sizeof(char *) * (line_count + 1));
    if (!map)
        return (NULL);
    fd = open(filename, O_RDONLY);
    i = 0;
    while (i < line_count)
    {
        map[i] = get_next_line(fd);
        // get_next_line이 반환한 개행 문자를 제거할 수 있습니다.
        // 예: ft_strtrim(map[i], "\n");
        i++;
    }
    map[i] = NULL;
    close(fd);
    return (map);
}

// --- 유효성 검사 함수들 ---

// 1. 파일 확장자가 .cub 인지 확인
int is_cub(char *str)
{
    char *dot_ptr;
    size_t required_len;

    dot_ptr = strrchr(str, '.');
    if (dot_ptr == NULL || dot_ptr == str)
        return (1);
    required_len = strlen("cub");
    if (ft_strncmp(dot_ptr + 1, "cub", required_len + 1) == 0)
        return (0);
    return (1);
}

// 2. 맵에 유효한 문자(0, 1, N, S, E, W, ' ')만 있는지, 플레이어는 1명인지 확인
int is_include_valid_chars(char **map)
{
    int i;
    int j;
    int player_count;

    player_count = 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (ft_strchr("01NSEW ", map[i][j]) == NULL)
                return (ft_perror("Error: Invalid character in map\n"));
            if (ft_strchr("NSEW", map[i][j]))
                player_count++;
            j++;
        }
        i++;
    }
    if (player_count != 1)
        return (ft_perror("Error: Map must have exactly one player\n"));
    return (0);
}

// 3. 맵이 벽('1')으로 둘러싸여 있는지 확인
int is_surrounded_by_walls(char **map)
{
    int i;
    int j;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            // '0' 또는 플레이어 위치가 지도의 가장자리에 있거나,
            // 주변에 빈 공간(' ')이 있으면 오류입니다.
            if (ft_strchr("0NSEW", map[i][j]))
            {
                if (i == 0 || !map[i + 1] || j == 0 || !map[i][j + 1] || \
                    (int)strlen(map[i - 1]) <= j || map[i - 1][j] == ' ' || \
                    (int)strlen(map[i + 1]) <= j || map[i + 1][j] == ' ' || \
                    map[i][j - 1] == ' ' || map[i][j + 1] == ' ')
                    return (ft_perror("Error: Map is not surrounded by walls\n"));
            }
            j++;
        }
        i++;
    }
    return (0);
}

// --- 메인 유효성 검사 함수 ---

// char** av를 받아 모든 유효성 검사를 순차적으로 실행
int map_val(char **av, char ***map_out)
{
    char    **map;

    if (is_cub(av[1]))
        return (ft_perror("Error: File extension must be .cub\n"));
    
    // 파일을 읽어 map 변수에 저장
    map = read_map_lines(av[1]);
    if (!map)
        return (ft_perror("Error: Failed to read map file or file is empty\n"));

    // 맵 내용 검사
    if (is_include_valid_chars(map) || is_surrounded_by_walls(map))
    {
        // ft_free_split(map); // 2차원 배열 해제 함수
        return (1); // 오류 발생
    }

    // 모든 검사를 통과하면 파싱 단계에서 사용할 수 있도록 맵 주소를 넘겨줌
    *map_out = map;
    return (0);
}
