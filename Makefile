# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-der <fvon-der@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/28 04:26:53 by fvon-der          #+#    #+#              #
#    Updated: 2025/02/27 01:04:23 by fvon-der         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors for output
RESET_COLOR = \033[0m          # Reset to default color
YELLOW      = \033[1;33m       # Brighter yellow for compliling messages
BLUE        = \033[1;34m       # Bright blue for info
GREEN       = \033[1;32m       # Bright green for success
RED         = \033[1;31m       # Bright red for errors
CYAN        = \033[1;36m       # Bright cyan for debug info

# Project settings
INCLUDE_DIR = include
LIB42_DIR = lib/lib42
LIB42_REPO = https://github.com/F-vdHaar/42_lib.git
INCLUDE = -I$(INCLUDE_DIR) -I$(LIB42_DIR)/libft/include -I$(LIB42_DIR)/ft_printf/include -I$(LIB42_DIR)/gnl/include 
CC = cc
CFLAGS = -Wall -Wextra -Werror -Wunused $(INCLUDE)
DEBUG_FLAGS = $(CFLAGS) -g -O0  -fsanitize=undefined -fno-strict-aliasing -fno-omit-frame-pointer -fstack-protector -DDEBUG -fno-inline
LDFLAGS = -L$(LIB42_DIR) -l42 
# Default version if not specified
VERSION ?= v2
SRC_DIR = src/$(VERSION)
OBJ_DIR = obj/$(VERSION)
NAME_SERVER = server
NAME_CLIENT = client

# Source and object files
SRCS_SERVER = $(SRC_DIR)/$(VERSION)_server.c $(SRC_DIR)/$(VERSION)_utils.c
SRCS_CLIENT = $(SRC_DIR)/$(VERSION)_client.c $(SRC_DIR)/$(VERSION)_utils.c
OBJS_SERVER = $(OBJ_DIR)/$(VERSION)_server.o $(OBJ_DIR)/$(VERSION)_utils.o
OBJS_CLIENT = $(OBJ_DIR)/$(VERSION)_client.o $(OBJ_DIR)/$(VERSION)_utils.o


# Default rule to build server and client
all: $(LIB42_DIR)/lib42.a $(NAME_SERVER) $(NAME_CLIENT)
	@echo "$(GREEN)MINITALK : Version $(VERSION) selected.$(RESET_COLOR)"
	@echo "Source files for server: $(SRCS_SERVER)"
	@echo "Source files for client: $(SRCS_CLIENT)"

# Ensure object directory exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Build external libraries if needed, or clone from repo
$(LIB42_DIR)/lib42.a:
	@if [ ! -d "$(LIB42_DIR)" ]; then \
		echo "$(YELLOW)Cloning lib42 from $(LIB42_REPO)...$(RESET_COLOR)"; \
		mkdir -p lib; \
		git clone $(LIB42_REPO) $(LIB42_DIR); \
	else \
		echo "$(YELLOW)Checking for lib42 library...$(RESET_COLOR)"; \
	fi
	@$(MAKE) -C $(LIB42_DIR)

# Build rules for server and client
$(NAME_SERVER): $(OBJS_SERVER)  $(LIB42_DIR)/lib42.a
	@echo "$(YELLOW)MINITALK : Compiling $(NAME_SERVER)...$(RESET_COLOR)"
	$(CC) $(CFLAGS) $(OBJS_SERVER) -o $(NAME_SERVER) $(LDFLAGS)
	@echo "$(GREEN)MINITALK : $(NAME_SERVER) compilation successful!$(RESET_COLOR)"

$(NAME_CLIENT): $(OBJS_CLIENT)  $(LIB42_DIR)/lib42.a
	@echo "$(YELLOW)MINITALK : Compiling $(NAME_CLIENT)...$(RESET_COLOR)"
	$(CC) $(CFLAGS) $(OBJS_CLIENT) -o $(NAME_CLIENT) $(LDFLAGS)
	@echo "$(GREEN)MINITALK : $(NAME_CLIENT) compilation successful!$(RESET_COLOR)"

# Rule to compile .o files from .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rules
clean:
	@echo "$(BLUE)MINITALK : Cleaning object files in $(OBJ_DIR)...$(RESET_COLOR)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(BLUE)MINITALK : Removing executables...$(RESET_COLOR)"
	@rm -f $(NAME_SERVER) $(NAME_CLIENT)
	@$(MAKE) -C $(LIB42_DIR) clean

re: fclean all

bonus: all
# Norminette target
norm:
	@echo "$(BLUE)MINITALK : Running Norminette...$(RESET_COLOR)"
	norminette $(SRCS_SERVER) $(SRCS_CLIENT) | grep -v "OK!"
	@echo "$(GREEN)MINITALK : Norminette check complete!$(RESET_COLOR)"

# Debug target
debug: clean
	@echo "$(RED)MINITALK : Compiling in debug mode...$(RESET_COLOR)"
	$(CC) $(DEBUG_FLAGS) $(SRCS_SERVER) -o $(NAME_SERVER)_debug $(LDFLAGS)
	$(CC) $(DEBUG_FLAGS) $(SRCS_CLIENT) -o $(NAME_CLIENT)_debug $(LDFLAGS)
	@echo "$(GREEN)MINITALK : Debug build complete!$(RESET_COLOR)"

# Version targets for different builds
v1: 
	@$(MAKE) VERSION=v1

v2: 
	@$(MAKE) VERSION=v2

v3: 
	@$(MAKE) VERSION=v3

.PHONY: all clean fclean re norm debug v1 v2 v3 bonus