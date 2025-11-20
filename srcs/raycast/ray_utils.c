/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunjkim <hyunjkim@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:28:51 by hyunjkim          #+#    #+#             */
/*   Updated: 2025/11/19 15:28:54 by hyunjkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray_data(t_ray *ray)
{
	ray->dir.x = 0;
	ray->dir.y = 0;
	ray->side_dist.x = 0;
	ray->side_dist.y = 0;
	ray->delta_dist.x = 0;
	ray->delta_dist.y = 0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->map_x = 0;
	ray->map_y = 0;
	ray->perp_wall_dist = 0;
	ray->hit = 0;
	ray->side = 0;
	ray->tex_x = 0;
	ray->tex_y = 0;
}
static void	set_ew_dir(t_player *player, int sign)
{
	player->dir.x = sign;
	player->dir.y = 0;
	player->plane.x = 0;
	player->plane.y = 0.66 * sign;
}

static void	set_ns_dir(t_player *player, int sign)
{
	player->dir.x = 0;
	player->dir.y = sign;
	player->plane.x = -0.66 * sign;
	player->plane.y = 0;
}

void	init_player_state(t_player *player, t_map_config *config)
{
	player->pos.x = config->start_pos.x;
	player->pos.y = config->start_pos.y;
	if (config->start_dir == 'E')
		set_ew_dir(player, 1);
	else if (config->start_dir == 'W')
		set_ew_dir(player, -1);
	else if (config->start_dir == 'S')
		set_ns_dir(player, 1);
	else if (config->start_dir == 'N')
		set_ns_dir(player, -1);
}

void	rotate_view(t_game *game, double rotation_speed, int dir)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	angle = rotation_speed * dir;
	old_dir_x = game->player.dir.x;
	game->player.dir.x = game->player.dir.x * cos(angle)
		- game->player.dir.y * sin(angle);
	game->player.dir.y = old_dir_x * sin(angle)
		+  game->player.dir.y * cos(angle);
	old_plane_x = game->player.plane.x;
	game->player.plane.x = game->player.plane.x * cos(angle)
		- game->player.plane.y * sin(angle);
	game->player.plane.y = old_plane_x * sin(angle)
		+ game->player.plane.y * cos(angle);
}
