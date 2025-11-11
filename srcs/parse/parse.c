/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:23:54 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/05 09:39:53 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_config(t_map_config *config)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		config->tex_paths[i] = NULL;
		i++;
	}
	config->floor_color = -1;
	config->ceiling_color = -1;
	config->map_grid = NULL;
	config->map_width = 0;
	config->map_height = 0;
	config->start_dir = '\0';
	config->start_pos.x = 0;
	config->start_pos.y = 0;
}

int	parse(char *filename, t_game *game)
{
	int		fd;
	char	*first_map_line;

	if (ft_strncmp(ft_strrchr(filename, '.'), ".cub", 4))
		return (ft_perror("Error: Invalid file extension (must be .cub)\n"));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_perror("Error: Cannot open file\n"));
	init_config(&game->config);
	first_map_line = parse_elements(fd, game);
	if (!first_map_line)
	{
		close(fd);
		return (ft_perror("Error: Map data not found or invalid element\n"));
	}
	if (parse_map_grid(fd, first_map_line, game) != 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	if (validate_config(game) != 0)
		return (1);
	return (0);
}