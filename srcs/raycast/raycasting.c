#include "cub3d.h"

void	init_ray_data(t_ray *ray)
{
	ray->dir.x = 0;
	ray->dir.y = 0;
	ray->side_dist.x = 0;
	ray->side_dist.y = 0;
	ray->delta_dist.x = 0;
	ray->delta_dist.y = 0;
	ray->step.x = 0;
	ray->step.y = 0;
	ray->map_x = 0;
	ray->map_y = 0;
	ray->perp_wall_dist = 0;
	ray->hit = 0;
	ray->side = 0;
	ray->tex_x = 0;
	ray->tex_y = 0;
}

void	init_player_state(t_player *player, t_map_config *config)
{
	player->pos.x = config->start_pos.x;
	player->pos.y = config->start_pos.y;
	if (config->start_dir == 'E')
	{
		player->dir.x = 1;
		player->dir.y = 0;
		player->plane.x = 0;
		player->plane.y = 0.66;
	}
	else if (config->start_dir == 'W')
	{
		player->dir.x = -1;
        	player->dir.y = 0;
       		player->plane.x = 0;
        	player->plane.y = -0.66;
	}
	else if (config->start_dir == 'S')
	{
		player->dir.x = 0;
		player->dir.y = 1;
		player->plane.x = -0.66;
		player->plane.y = 0;
	}
	else if (config->start_dir == 'N')
	{
		player->dir.x = 0;
		player->dir.y = -1;
		player->plane.x = 0.66;
		player->plane.y = 0;
	}
}

void	setup_ray(t_ray *ray, t_player *player, int x)
{
	double	camera_x;

	camera_x = 2 * (double)x / (double)WIN_WIDTH - 1;
	ray->dir.x = player->dir.x + player->plane.x * camera_x;
	ray->dir.y = player->dir.y + player->plane.y * camera_x;
	ray->map_x = (int)player->pos.x;
	ray->map_y = (int)player->pos.y;
	ray->hit = 0;
	if (ray->dir.x == 0)
		ray->delta_dist.x = 1e30;
	else
		ray->delta_dist.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta_dist.y = 1e30;
	else
		ray->delta_dist.y = fabs(1 / data->dir.y);
	if (ray->dir.x < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - player->pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - player->pos.y) * ray->delta_dist.y;
	}
}

void	run_dda(t_ray *ray, t_map_config *config)
{
	while (ray->hit = 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += (int)ray->step.x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += (int)ray->step.y;
			ray->side = 1;
		}
		if (config->map_grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}
// add calculation for texture x coordinate
void	get_wall_dist(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->map_x - player->pos.x + (1 - ray->step.x) / 2) / ray->dir.x;
	}
	else
	{
		ray->perp_wall_dist = (ray->map_y - player->pos.y + (1 - ray->step.y) / 2) / ray->dir.y;
	}
}
// add mlx window functions
void	raycasting(t_game *game)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		setup_ray(&gmae->ray, &game->player, x);
		run_dda(&game->ray, &game->config);
		//calculate distance and texture_x
		//calculate draw start/end
		//init & draw texture (column)
		x++;
	}

}