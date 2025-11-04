# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/22 12:15:10 by juhyeonl          #+#    #+#              #
#    Updated: 2025/11/04 21:18:24 by juhyeonl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

# --- 디렉토리 경로 ---
SRCS_DIR = srcs
OBJS_DIR = objs
INCS_DIR = includes

# --- 라이브러리: libft ---
LIBFT_DIR = libft
LIBFT_URL = git@github.com:Hive-juhyeonl/libft.git
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# --- 라이브러리: MLX42 ---
MLX_DIR = MLX42
MLX_URL = https://github.com/codam-coding-college/MLX42.git
MLX_BUILD_DIR = $(MLX_DIR)/build
MLX_LIB = $(MLX_BUILD_DIR)/libmlx42.a

# --- 인클루드 및 라이브러리 플래그 ---
INCLUDES = -I$(INCS_DIR) -I$(LIBFT_DIR)/includes -I$(MLX_DIR)/include
LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_BUILD_DIR) -lmlx42 -ldl -lglfw -lpthread -lm

# --- 소스 파일 ---
SRCS_FILES = $(shell find $(SRCS_DIR) -name "*.c")

OBJS = $(addprefix $(OBJS_DIR)/, $(notdir $(SRCS_FILES:.c=.o)))
VPATH = $(sort $(dir $(SRCS_FILES))) # VPATH에 모든 소스 디렉토리 추가

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_LIB) $(MLX_LIB)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(INCLUDES)
	@echo "✅ cub3d compiled successfully!"

# --- 라이브러리 빌드 규칙 ---
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
	@cmake -S $(MLX_DIR) -B $(MLX_BUILD_DIR)
	@$(MAKE) --no-print-directory --quiet -C $(MLX_BUILD_DIR) -j4

# --- 오브젝트 파일 빌드 규칙 ---
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR) $(LIBFT_LIB) $(MLX_LIB)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --- 정리 규칙 (수정됨) ---
clean:
	@$(RM) -r $(OBJS_DIR)
	@if [ -f "$(LIBFT_DIR)/Makefile" ]; then \
		$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean; \
	fi
	@if [ -d "$(MLX_BUILD_DIR)" ]; then $(RM) -r $(MLX_BUILD_DIR); fi
	@echo "✅ Objects removed."

fclean:
	@$(RM) -r $(OBJS_DIR)
	@$(RM) $(NAME)
	@echo "🧹 Removing cloned repositories..."
	@if [ -d "$(LIBFT_DIR)" ]; then $(RM) -r $(LIBFT_DIR); fi
	@if [ -d "$(MLX_DIR)" ]; then $(RM) -r $(MLX_DIR); fi
	@echo "✅ Executable and cloned libraries removed."

re: fclean all