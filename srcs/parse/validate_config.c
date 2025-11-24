/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_config.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:58:55 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/24 14:01:43 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_elements(t_map_config *config)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (config->tex_paths[i] == NULL)
			return (ft_perror("Error: Missing texture definition\n"));
		i++;
	}
	if (config->floor_color == -1)
		return (ft_perror("Error: Missing floor color definition\n"));
	if (config->ceiling_color == -1)
		return (ft_perror("Error: Missing ceiling color definition\n"));
	return (0);
}

static int	validate_map_chars(t_map_config *config)
{
	int	y;
	int	x;

	y = 0;
	while (y < config->map_height)
	{
		x = 0;
		while (config->map_grid[y][x])
		{
			if (!ft_strchr("01 ", config->map_grid[y][x]))
				return (ft_perror("Error: Invalid map\n"));
			x++;
		}
		if (x == 0 && y != config->map_height - 1)
			return (ft_perror("Error: Empty line inside map\n"));
		y++;
	}
	return (0);
}

static void	dfs_check(t_flood_fill *ff, int y, int x)
{
	if (y < 0 || y >= ff->height || x < 0
		|| x >= (int)ft_strlen(ff->map_copy[y]))
	{
		ff->is_open = 1;
		return ;
	}
	if (ff->map_copy[y][x] == '1' || ff->map_copy[y][x] == 'V')
		return ;
	if (ff->map_copy[y][x] == ' ')
	{
		ff->is_open = 1;
		return ;
	}
	ff->map_copy[y][x] = 'V';
	dfs_check(ff, y + 1, x);
	dfs_check(ff, y - 1, x);
	dfs_check(ff, y, x + 1);
	dfs_check(ff, y, x - 1);
}

static int	validate_map_walls(t_game *game)
{
	t_flood_fill	ff;
	int				i;

	ff.height = game->config.map_height;
	ff.width = game->config.map_width;
	ff.is_open = 0;
	ff.map_copy = (char **)malloc(sizeof(char *) * (ff.height + 1));
	if (!ff.map_copy)
		return (ft_perror("Error: Malloc failed for wall validation\n"));
	i = 0;
	while (i < ff.height)
	{
		ff.map_copy[i] = ft_strdup(game->config.map_grid[i]);
		i++;
	}
	ff.map_copy[i] = NULL;
	dfs_check(&ff, (int)game->config.start_pos.y,
		(int)game->config.start_pos.x);
	free_split(ff.map_copy);
	if (ff.is_open)
		return (ft_perror("Error: Map is not surrounded by walls\n"));
	return (0);
}

int	validate_config(t_game *game)
{
	if (validate_elements(&game->config) != 0)
		return (1);
	if (find_player(&game->config) != 0)
		return (1);
	if (validate_map_chars(&game->config) != 0)
		return (1);
	if (validate_map_walls(game) != 0)
		return (1);
	return (0);
}
