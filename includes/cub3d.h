/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 12:09:28 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/10/22 14:45:03 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

typedef struct	s_map_config {
	char	*n_tex_path;
	char	*s_tex_path;
	char	*w_tex_path;
	char	*e_tex_path;
	int		f_color;
	int		c_color;
	int		map_w;
	int		map_h;
	char	**map_grid;
}	t_map_config;

typedef struct	s_player {
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct	s_image {
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_len;
	int		endian;
	int		i_width;
	int		i_height;
}	t_image;

typedef struct	s_ray {
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}	t_ray;

typedef	struct	s_game {
	void	*mlx_ptr;
	void	*win_ptr;
	t_map_config	*map_config;
	t_player		*player;
	t_image			screen_buffer;
	t_image			textures[4];
	t_ray			ray;
}	t_game;


# include <stdio.h>
# include "MLX42/MLX42.h"
# include "../libft/includes/libft.h"

/* ------------parse------------ */
int	parse_map(char **av);


/* -----------raycast----------- */


/* ------------common----------- */
int		ft_perror(char *msg);

#endif