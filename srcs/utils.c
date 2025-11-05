/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:59:38 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/05 09:40:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

int	ft_perror(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (1);
}

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
		free_split(config->map_grid);
}

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
