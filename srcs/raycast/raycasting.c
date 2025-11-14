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

static void	setup_ray(t_ray *ray, t_player *player, int x)
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
		ray->delta_dist.y = fabs(1 / ray->dir.y);
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - player->pos.x) * ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - player->pos.y) * ray->delta_dist.y;
	}
}

static void	run_dda(t_ray *ray, t_map_config *config)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= config->map_height ||
            	ray->map_x < 0 || ray->map_x >= (int)ft_strlen(config->map_grid[ray->map_y]))
        	{
          		ray->hit = 1;
            		break ;
       		}
		if (config->map_grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

static void	get_wall_dist(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->map_x - player->pos.x + (1 - ray->step_x) / 2) / ray->dir.x;
	}
	else
	{
		ray->perp_wall_dist = (ray->map_y - player->pos.y + (1 - ray->step_y) / 2) / ray->dir.y;
	}
}

static void	get_texture_x(t_game *game, t_ray *ray, t_player *player)
{
	double	x_wall;
	int	tex_width;

	tex_width = game->textures[NORTH]->width;
	if (ray->side == 0)
		x_wall = player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		x_wall = player->pos.x + ray->perp_wall_dist * ray->dir.x;
	x_wall -= floor(x_wall);
	ray->tex_x = (int)(x_wall * (double)tex_width);
	if (ray->side == 0 && ray->dir.x > 0)
		ray->tex_x = tex_width - ray->tex_x - 1;
	if (ray->side == 1 && ray->dir.y < 0)
		ray->tex_x = tex_width - ray->tex_x -1;
}
static int	get_texture_color(mlx_texture_t *texture, int x, int y)
{
	int index;
	int r;
	int g;
	int b;
	int a;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
        	return (0xFF000000);
	index = (y * texture->width + x) * texture->bytes_per_pixel;
	r = texture->pixels[index];
	g = texture->pixels[index + 1];
	b = texture->pixels[index + 2];
	a = texture->pixels[index + 3];
	return (r << 24 | g << 16 | b << 8 | a);
}

static void	draw_textured_walls(t_game *game, int x, int draw_start, int draw_end)
{
	int	y;
	int	color;
	int	line_height;
	double	step;
	double	tex_pos;
	mlx_texture_t	*texture;

	if (game->ray.side == 0)
	{
		if (game->ray.dir.x > 0)
			texture = game->textures[EAST];
		else
			texture = game->textures[WEST];
	}
	else
	{
		if (game->ray.dir.y > 0)
			texture = game->textures[SOUTH];
		else
			texture = game->textures[NORTH];
	}
	line_height = draw_end - draw_start;
	step = (double)texture->height / (double)line_height;
	tex_pos = (draw_start - WIN_HEIGHT / 2 + line_height / 2) * step;
	y = 0;
	while (y < draw_start)
	{
		mlx_put_pixel(game->screen_buffer, x, y, game->config.ceiling_color);
		y++;
	}
	while (y < draw_end)
	{
		game->ray.tex_y = (int)tex_pos % texture->height;
		tex_pos += step;
		color = get_texture_color(texture, game->ray.tex_x, game->ray.tex_y);
		mlx_put_pixel(game->screen_buffer, x, y, color);
		y++;
	}
	while (y < WIN_HEIGHT)
	{
		mlx_put_pixel(game->screen_buffer, x, y, game->config.floor_color);
		y++;
	}
}

void	raycasting(t_game *game)
{
	int	x;
	int	line_height;
	int	draw_start;
	int	draw_end;

	x = 0;
	while (x < WIN_WIDTH)
	{
		setup_ray(&game->ray, &game->player, x);
		run_dda(&game->ray, &game->config);
		get_wall_dist(&game->ray, &game->player);
		line_height = (int)(WIN_HEIGHT / game->ray.perp_wall_dist);
		draw_start = -line_height / 2 + WIN_HEIGHT / 2;
		draw_end = line_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= WIN_HEIGHT)
			draw_end = WIN_HEIGHT - 1;
		get_texture_x(game, &game->ray, &game->player);
		draw_textured_walls(game, x, draw_start, draw_end);
		x++;
	}
	mlx_image_to_window(game->mlx_ptr, game->screen_buffer, 0, 0);
}

