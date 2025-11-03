/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_utils.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* 파싱에 필요한 유틸리티 함수들입니다. */
/* Updated: 2025/11/02 21:50:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

/*
 * 2차원 배열을 해제합니다.
 */
void	free_split(char **split_arr)
{
	int	i;

	if (!split_arr)
		return ;
	i = 0;
	while (split_arr[i])
	{
		free(split_arr[i]);
		i++;
	}
	free(split_arr);
}

/*
 * 해당 라인이 맵의 일부인지 (요소가 아닌지) 확인합니다.
 * '1', '0', ' ' (공백) 문자를 포함하면 맵 라인으로 간주합니다.
 */
int	is_map_line(char *line)
{
	while (*line)
	{
		if (*line == '1' || *line == '0' || *line == ' ')
			return (1);
		// 맵 라인에는 탭이 허용되지 않는다고 가정 (PDF 명세 기준)
		// 만약 탭도 맵의 일부로 간주하려면 ' ' 대신 ft_isspace() 사용
		line++;
	}
	return (0);
}
