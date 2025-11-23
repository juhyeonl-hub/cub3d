/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:58:34 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/19 15:26:28 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	process_config_line(char *line, t_game *game)
{
	char	**parts;
	int		res;

	parts = ft_split(line, ' ');
	if (!parts)
		return (1);
		
	if (!parts[0])
		res = 0;
	else if (!ft_strncmp(parts[0], "NO", 3) || !ft_strncmp(parts[0], "SO", 3)
		|| !ft_strncmp(parts[0], "WE", 3) || !ft_strncmp(parts[0], "EA", 3))
		res = parse_texture(parts, game);
	else if (!ft_strncmp(parts[0], "F", 2) || !ft_strncmp(parts[0], "C", 2))
		res = parse_color(parts, game);
	else
		res = 1;
	free_split(parts);
	return (res);
}

char	*parse_elements(int fd, t_game *game)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			return (NULL);
		if (is_empty_line(line))
		{
			free(line);
			continue ;
		}
		if (is_map_line(line))
			return (line);
		if (process_config_line(line, game) != 0)
		{
			free(line);
			return (NULL);
		}
		free(line);
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
	game->config.tex_paths[idx] = ft_strtrim(parts[1], " \t\n");
	if (!game->config.tex_paths[idx])
		return (ft_perror("Error: Malloc failed\n"));
	return (0);
}

static int	get_rgb_value(char *str, int *color_out)
{
	char	**rgb_v;
	int		rgb[3];

	rgb_v = ft_split(str, ',');
	if (!rgb_v || !rgb_v[0] || !rgb_v[1] || !rgb_v[2] || rgb_v[3])
	{
		free_split(rgb_v);
		return (ft_perror("Error: Invalid RGB format\n"));
	}
	rgb[0] = ft_atoi(rgb_v[0]);
	rgb[1] = ft_atoi(rgb_v[1]);
	rgb[2] = ft_atoi(rgb_v[2]);
	free_split(rgb_v);
	if (rgb[0] < 0 || rgb[0] > 255 || rgb[1] < 0 || rgb[1] > 255
		|| rgb[2] < 0 || rgb[2] > 255)
		return (ft_perror("Error: RGB values out of range (0-255)\n"));
	*color_out = (rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255;
	return (0);
}

int	parse_color(char **parts, t_game *game)
{
	int	color;

	if (!parts[1] || parts[2])
		return (ft_perror("Error: Invalid color format\n"));
	if (get_rgb_value(parts[1], &color) != 0)
		return (1);
	if (ft_strncmp(parts[0], "F", 2) == 0)
	{
		if (game->config.floor_color != -1)
			return (ft_perror("Error: Duplicate floor color definition\n"));
		game->config.floor_color = color;
	}
	else
	{
		if (game->config.ceiling_color != -1)
			return (ft_perror("Error: Duplicate ceiling color definition\n"));
		game->config.ceiling_color = color;
	}
	return (0);
}
