/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:59:38 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/03 12:59:40 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

int	ft_perror(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (1);
}

/*
 * config 구조체에 할당된 메모리를 해제합니다.
 */
void	free_game_config(t_map_config *config)
{
	int	i;

	if (!config)
		return ;
	i = 0;
	while (i < 4)
	{
		if (config->tex_paths[i])
			free(config->tex_paths[i]);
		i++;
	}
	if (config->map_grid)
		free_split(config->map_grid); // 2차원 배열 해제 유틸 필요
}