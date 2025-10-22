# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/22 12:15:10 by juhyeonl          #+#    #+#              #
#    Updated: 2025/10/22 13:38:01 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

SRCS_DIR = srcs
OBJS_DIR = objs
INCS_DIR = includes

LIBFT_DIR = libft
LIBFT_URL = git@github.com:Hive-juhyeonl/libft.git
LIBFT_LIB = $(LIBFT_DIR)/libft.a

MLX_DIR = MLX42
MLX_URL = git@github.com:codam-coding-college/MLX42.git
MLX_BUILD_DIR = $(MLX_DIR)/build
MLX_LIB = $(MLX_BUILD_DIR)/libmlx42.a

INCLUDES = -I$(INCS_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)/include
LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_BUILD_DIR) -lmlx42 -ldl -lglfw -lpthread -lm

SRCS = $(shell find $(SRCS_DIR) -name "*.c")
OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS:.c=.o)))
VPATH = $(sort $(dir $(SRCS)))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB) $(MLX_LIB)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)
	@echo "✅ cub3d compiled successfully!"

$(LIBFT_LIB):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "Downloading libft..."; \
		git clone --quiet $(LIBFT_URL) $(LIBFT_DIR); \
	fi
	@echo "Building libft..."
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Downloading MLX42..."; \
		git clone --quiet $(MLX_URL) $(MLX_DIR); \
	fi
	@echo "Building MLX42..."
	@cmake -S $(MLX_DIR) -B $(MLX_BUILD_DIR) > /dev/null 2>&1
	@$(MAKE) --no-print-directory --quiet -C $(MLX_BUILD_DIR) -j4

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR) $(LIBFT_LIB) $(MLX_LIB)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) -r $(OBJS_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) --no-print-directory -C $(LIBFT_DIR) clean; fi
	@if [ -d "$(MLX_BUILD_DIR)" ]; then $(RM) -r $(MLX_BUILD_DIR); fi
	@echo "✅ Objects removed."

fclean: clean
	@$(RM) $(NAME)
	@echo "✅ Executable and library objects removed."
	@echo "🧹 Removing cloned repositories..."
	@if [ -d "$(LIBFT_DIR)" ]; then $(RM) -r $(LIBFT_DIR); fi
	@if [ -d "$(MLX_DIR)" ]; then $(RM) -r $(MLX_DIR); fi

re: fclean all
