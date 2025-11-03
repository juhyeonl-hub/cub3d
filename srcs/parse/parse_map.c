/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_map.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* 2단계(Pass 2): 맵 그리드를 t_list를 거쳐 char**로 파싱합니다. */
/* Updated: 2025/11/02 21:55:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

// 헬퍼 함수 프로토타입
static char	**list_to_map_grid(t_list *head, t_map_config *config);
static char	*ft_strdup_no_newline(const char *s);
void		free_split(char **split_arr); // parse_utils.c에서 가져옴

/*
 * 맵의 첫 줄(first_map_line)과 GNL(fd)을 사용하여
 * 맵 전체를 읽고 t_list에 저장한 뒤, char**로 변환합니다.
 */
int	parse_map_grid(int fd, char *first_map_line, t_game *game)
{
	t_list	*head;
	char	*line;

	head = ft_lstnew(ft_strdup_no_newline(first_map_line));
	free(first_map_line); // GNL에서 받은 원본 라인 해제
	if (!head)
		return (ft_perror("Error: Malloc failed\n"));

	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL) // 파일 끝
			break ;
		
		// PDF 규칙: 맵은 파일의 마지막이어야 함.
		// 맵 중간에 빈 줄이 있는 경우(error2.cub)도 맵의 일부로 간주
		// (추후 validate_config에서 빈 줄을 오류 처리할 수 있음)
		ft_lstadd_back(&head, ft_lstnew(ft_strdup_no_newline(line)));
		free(line);
	}

	// t_list를 char**로 변환하여 config에 저장
	game->config.map_grid = list_to_map_grid(head, &game->config);
	ft_lstclear(&head, free); // 리스트와 내부 문자열 모두 해제

	if (!game->config.map_grid)
		return (ft_perror("Error: Failed to allocate map grid\n"));
	
	return (0);
}

/*
 * GNL에서 읽을 때 \n이 포함될 수 있으므로, \n을 제외하고 복제합니다.
 */
static char	*ft_strdup_no_newline(const char *s)
{
	char	*new_str;
	size_t	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		len--;
	
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, s, len);
	new_str[len] = '\0';
	return (new_str);
}

/*
 * t_list를 char** 2차원 배열로 변환합니다.
 * 이 과정에서 map_height와 map_width를 계산하여 config에 저장합니다.
 */
static char	**list_to_map_grid(t_list *head, t_map_config *config)
{
	char	**map;
	t_list	*current;
	int		i;
	size_t	max_width;

	config->map_height = ft_lstsize(head);
	if (config->map_height == 0)
		return (NULL);
	
	map = (char **)malloc(sizeof(char *) * (config->map_height + 1));
	if (!map)
		return (NULL);

	i = 0;
	max_width = 0;
	current = head;
	while (current)
	{
		map[i] = (char *)current->content; // content의 소유권을 map으로 이전
		current->content = NULL; // 리스트 해제 시 content가 free되는 것을 방지
		
		if (ft_strlen(map[i]) > max_width)
			max_width = ft_strlen(map[i]);
		
		current = current->next;
		i++;
	}
	map[i] = NULL; // 2차원 배열의 끝
	config->map_width = (int)max_width;
	return (map);
}
