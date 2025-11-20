/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 23:06:44 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/19 16:11:54 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// Window Size
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768
# define MINIMAP_SCALE 0.2
# define ERRMSG_FINDPLAYER "Error: Multiple players found in map\n"
# define ERRMSG_NOT_ENOUGH_ARG "Error: Invalid arguments\nUsage: ./cub3d <map.cub>\n"

// game->textures[NORTH] == game->textures[0]
typedef enum e_texture_index
{
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
}					t_texture_index;

typedef struct s_vector
{
	double			x;
	double			y;
}					t_vector;

typedef struct s_map_config
{
	char *tex_paths[4]; // 텍스처 경로 (NO, SO, WE, EA)
	int floor_color;    // 0xRRGGBBAA
	int ceiling_color;  // 0xRRGGBBAA
	char			**map_grid;
	int				map_width;
	int				map_height;
	char start_dir;     // player start direction
	t_vector start_pos; // player start position (예: {x: 5.5, y: 3.5})
}					t_map_config;

typedef struct s_player
{
	t_vector pos;   // 현재 위치 벡터 (pos.x, pos.y)
	t_vector dir;   // 방향 벡터 (dir.x, dir.y)
	t_vector plane; // 카메라 평면 벡터 (plane.x, plane.y)
}					t_player;

typedef struct s_ray
{
	t_vector dir;        // 레이의 방향 벡터 (ray.dir.x, ray.dir.y)
	t_vector side_dist;  // 다음 면까지의 거리
	t_vector delta_dist; // x, y면 사이의 거리
	int step_x;          // x, y 방향 스텝 (+1 또는 -1)
	int				step_y;
	int map_x;             // 레이의 현재 맵 x 인덱스
	int map_y;             // 레이의 현재 맵 y 인덱스
	double perp_wall_dist; // 수직 거리
	int hit;               // 벽 충돌 여부
	int side;              // NS(0) 또는 EW(1) 충돌
	int tex_x;             // 텍스처 x 좌표
	int tex_y;             // 텍스처 y 좌표
}					t_ray;

typedef struct s_game
{
	mlx_t			*mlx_ptr;
	t_map_config	config;
	t_player		player;
	t_ray			ray;
	mlx_texture_t	*textures[4];
	mlx_image_t		*screen_buffer;
	int				key_w;
	int				key_a;
	int				key_s;
	int				key_d;
	int				key_left;
	int				key_right;
	mlx_image_t		*minimap_buffer;
	int				minimap_enabled;
	double			prev_mouse_x;
}					t_game;

typedef struct s_flood_fill
{
	char			**map_copy;
	int				height;
	int				width;
	int				is_open;
}					t_flood_fill;

/* ------------parse------------ */
int					parse(char *filename, t_game *game);
void				init_config(t_map_config *config);
int					validate_config(t_game *game);
char				*parse_elements(int fd, t_game *game);
int					parse_texture(char **parts, t_game *game);
int					parse_color(char **parts, t_game *game);
int					parse_map_grid(int fd, char *first_map_line, t_game *game);
int					is_map_line(char *line);
int					find_player(t_map_config *config);
int					is_empty_line(char *line);

/* -----------raycast----------- */
void				init_ray_data(t_ray *ray);
void				init_player_state(t_player *player, t_map_config *config);
void				draw_textured_walls(t_game *game, int x, int draw_start,
						int draw_end);
void				raycasting(t_game *game);
void				player_movement(t_game *game, double frame_time);
void				game_loop(void *param);
void				rotate_view(t_game *game, double rotation_speed, int dir);
void				setup_ray(t_ray *ray, t_player *player, int x);		

/* ------------common----------- */
int					ft_perror(char *msg);
void				free_game_config(t_map_config *config);
void				free_split(char **split_arr);

/* ------------debug----------- */
void				debug_print_game_struct(t_game *game);

#endif