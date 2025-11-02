/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 23:06:44 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/11/03 00:23:47 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include "../libft/includes/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

// Window Size
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768
# define MINIMAP_SCALE 0.2

// game->textures[NORTH] == game->textures[0]
typedef enum e_texture_index
{
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
}	t_texture_index;

/*
 * 2D 벡터 구조체
 * (x, y) 좌표를 하나의 단위로 묶어 관리합니다.
 */
typedef struct	s_vector
{
	double	x;
	double	y;
}			t_vector;

/*
 * 1. 맵 설정 정보 (t_map_config)
 * .cub 파일의 정적 데이터를 저장합니다.
 */
typedef struct	s_map_config {

	char		*tex_paths[4];	// 텍스처 경로 (NO, SO, WE, EA)
	int			floor_color;	// 0xRRGGBBAA
	int			ceiling_color;	// 0xRRGGBBAA
	char		**map_grid;
	int			map_width;
	int			map_height;
	char		start_dir;		// player start direction
	t_vector	start_pos;		// player start position (예: {x: 5.5, y: 3.5})
}	t_map_config;

/*
 * 2. 플레이어 정보 (t_player)
 * 게임 중 실시간으로 변하는 동적 데이터를 저장합니다.
 */
typedef struct	s_player {
	t_vector	pos;		// 현재 위치 벡터 (pos.x, pos.y)
	t_vector	dir;		// 방향 벡터 (dir.x, dir.y)
	t_vector	plane;		// 카메라 평면 벡터 (plane.x, plane.y)
}	t_player;

/*
 * 3. 레이캐스팅 정보 (t_ray)
 * 단일 수직선을 그리기 위한 계산값 저장용
 */
typedef struct	s_ray {
	t_vector	dir;		// 레이의 방향 벡터 (ray.dir.x, ray.dir.y)
	t_vector	side_dist;	// 다음 면까지의 거리
	t_vector	delta_dist;	// x, y면 사이의 거리
	t_vector	step;		// x, y 방향 스텝 (+1 또는 -1)
	int			map_x;		// 레이의 현재 맵 x 인덱스
	int			map_y;		// 레이의 현재 맵 y 인덱스
	double		perp_wall_dist; // 수직 거리
	int			hit;		// 벽 충돌 여부
	int			side;		// NS(0) 또는 EW(1) 충돌
	int			tex_x;		// 텍스처 x 좌표
	int			tex_y;		// 텍스처 y 좌표
}	t_ray;

/*
 * 4. 게임 메인 구조체 (t_game)
 * 모든 핵심 요소를 포함하는 "컨트롤 타워"입니다.
 */
typedef	struct	s_game {
	// MLX 및 창 포인터
	mlx_t			*mlx_ptr;

	// 핵심 데이터 구조체 (값으로 직접 포함)
	t_map_config	config;
	t_player		player;
	t_ray			ray;
	
	// MLX42 텍스처 및 이미지
	mlx_texture_t	*textures[4]; // [NORTH], [SOUTH], [WEST], [EAST]
	mlx_image_t		*screen_buffer; // 렌더링할 메인 이미지 (버퍼)
	
	// 키 입력 상태
	int				key_w;
	int				key_a;
	int				key_s;
	int				key_d;
	int				key_left;
	int				key_right;

	/* ---------- 보너스 파트 ---------- */
	mlx_image_t		*minimap_buffer; // 미니맵 이미지
	int				minimap_enabled;
	double			prev_mouse_x;    // 이전 마우스 x좌표
}	t_game;


/* ------------parse------------ */
int	parse(char *filename, t_game *game);


/* -----------raycast----------- */
void	game_loop_hook(void *param);
void	key_hook(mlx_key_data_t keydata, void *param);


/* ------------common----------- */
int		ft_perror(char *msg);
void	free_game_config(t_map_config *config);

/* ------------bonus------------ */
void	mouse_hook(double xpos, double ypos, void *param);
void	draw_minimap(t_game *game);

#endif