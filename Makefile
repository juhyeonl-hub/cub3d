NAME        = cub3d
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
RM          = rm -rf

SRCS_DIR    = srcs
OBJS_DIR    = objs
INCS_DIR    = includes

LIBFT_DIR   = libft
LIBFT_URL   = git@github.com:Hive-juhyeonl/libft.git
LIBFT       = $(LIBFT_DIR)/libft.a

MLX_DIR     = MLX42
MLX_URL     = https://github.com/codam-coding-college/MLX42.git
MLX_BUILD   = $(MLX_DIR)/build
MLX         = $(MLX_BUILD)/libmlx42.a

SRC_FILES   =	main.c \
				utils.c \
				parse/parse.c \
				parse/parse_elements.c \
				parse/parse_map.c \
				parse/parse_utils.c \
				parse/validate_config.c \
				raycast/draw.c \
				raycast/draw_utils.c \
				raycast/game_loop.c \
				raycast/movement.c \
				raycast/ray_utils.c \
				raycast/raycasting.c \
				raycast/setup_ray.c

SRCS        = $(addprefix $(SRCS_DIR)/, $(SRC_FILES))
OBJS        = $(addprefix $(OBJS_DIR)/, $(SRC_FILES:.c=.o))

INCS        = -I$(INCS_DIR) -I$(LIBFT_DIR)/includes -I$(MLX_DIR)/include
LIBS        = $(LIBFT) $(MLX) -ldl -lglfw -lpthread -lm

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(INCS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then git clone $(LIBFT_URL) $(LIBFT_DIR); fi
	@make -C $(LIBFT_DIR)

$(MLX):
	@if [ ! -d "$(MLX_DIR)" ]; then git clone $(MLX_URL) $(MLX_DIR); fi
	@cmake -S $(MLX_DIR) -B $(MLX_BUILD)
	@make -C $(MLX_BUILD) -j4

clean:
	$(RM) $(OBJS_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then make -C $(LIBFT_DIR) clean; fi
	@rm -rf $(MLX_BUILD)

fclean: clean
	$(RM) $(NAME)
	@rm -rf $(LIBFT_DIR)
	@rm -rf $(MLX_DIR)

re: fclean all

.PHONY: all clean fclean re