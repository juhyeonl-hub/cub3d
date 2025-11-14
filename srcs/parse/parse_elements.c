/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:58:34 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/05 09:31:33 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*parse_elements(int fd, t_game *game)
{
	char	*line;
	char	**parts;
	int		parse_result;
	int		i;

	while (1)
	{
		i = 0;
		line = get_next_line(fd);
		if (line == NULL)
			return (NULL);
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (line[i] == '\n' || line[i] == '\0')
		{
			free(line);
			continue ;
		}
		if (is_map_line(line))
			return (line);
		parts = ft_split(line, ' ');
		free(line);
		if (!parts)
			return (NULL);
		if (ft_strncmp(parts[0], "NO", 3) == 0 || ft_strncmp(parts[0], "SO", 3) == 0 || \
			ft_strncmp(parts[0], "WE", 3) == 0 || ft_strncmp(parts[0], "EA", 3) == 0)
			parse_result = parse_texture(parts, game);
		else if (ft_strncmp(parts[0], "F", 2) == 0 || ft_strncmp(parts[0], "C", 2) == 0)
			parse_result = parse_color(parts, game);
		else
			parse_result = 1;
		free_split(parts);
		if (parse_result != 0)
			return (NULL);
	}
}

int	parse_texture(char **parts, t_game *game)
{
	t_texture_index	idx;

	if (!parts[1] || parts[2])
		return (ft_perror("Error: Invalid texture format\n"));
	if (ft_strncmp(parts[0], "NO", 3) == 0)
		idx = NORTH;
	else if (ft_strncmp(parts[0], "SO", 3) == 0)
		idx = SOUTH;
	else if (ft_strncmp(parts[0], "WE", 3) == 0)
		idx = WEST;
	else
		idx = EAST;
	if (game->config.tex_paths[idx] != NULL)
		return (ft_perror("Error: Duplicate texture definition\n"));
	// game->config.tex_paths[idx] = ft_strdup(parts[1]);
	game->config.tex_paths[idx] = ft_strtrim(parts[1], " \t\n");
	if (!game->config.tex_paths[idx])
		return (ft_perror("Error: Malloc failed\n"));
	return (0);
}

int	parse_color(char **parts, t_game *game)
{
	char	**rgb_values;
	int		rgb[3];
	int		color_int;

	if (!parts[1] || parts[2])
		return (ft_perror("Error: Invalid color format\n"));
	rgb_values = ft_split(parts[1], ',');
	if (!rgb_values || !rgb_values[0] || !rgb_values[1] || !rgb_values[2] || rgb_values[3])
	{
		free_split(rgb_values);
		return (ft_perror("Error: Invalid RGB format\n"));
	}
	rgb[0] = ft_atoi(rgb_values[0]);
	rgb[1] = ft_atoi(rgb_values[1]);
	rgb[2] = ft_atoi(rgb_values[2]);
	free_split(rgb_values);
	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255 || \
		rgb[2] < 0 || rgb[2] > 255)
		return (ft_perror("Error: RGB values out of range (0-255)\n"));
	color_int = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255;
	if (ft_strncmp(parts[0], "F", 2) == 0)
	{
		if (game->config.floor_color != -1)
			return (ft_perror("Error: Duplicate floor color definition\n"));
		game->config.floor_color = color_int;
	}
	else
	{
		if (game->config.ceiling_color != -1)
			return (ft_perror("Error: Duplicate ceiling color definition\n"));
		game->config.ceiling_color = color_int;
	}
	return (0);
}