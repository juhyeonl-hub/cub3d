/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:56:46 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/03 13:57:45 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

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
		map[i] = (char *)current->content;
		current->content = NULL;
		
		if (ft_strlen(map[i]) > max_width)
			max_width = ft_strlen(map[i]);
		
		current = current->next;
		i++;
	}
	map[i] = NULL;
	config->map_width = (int)max_width;
	return (map);
}

int	parse_map_grid(int fd, char *first_map_line, t_game *game)
{
	t_list	*head;
	char	*line;

	head = ft_lstnew(ft_strdup_no_newline(first_map_line));
	free(first_map_line);
	if (!head)
		return (ft_perror("Error: Malloc failed\n"));

	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		ft_lstadd_back(&head, ft_lstnew(ft_strdup_no_newline(line)));
		free(line);
	}
	game->config.map_grid = list_to_map_grid(head, &game->config);
	ft_lstclear(&head, free);
	if (!game->config.map_grid)
		return (ft_perror("Error: Failed to allocate map grid\n"));
	return (0);
}