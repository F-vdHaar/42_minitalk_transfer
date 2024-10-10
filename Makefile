# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-der <fvon-der@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/28 04:26:53 by fvon-der          #+#    #+#              #
#    Updated: 2024/08/02 22:57:14 by fvon-der         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER = server
CLIENT = client
SRC_DIR = src/minitalk

# Compiler and flags
CC = cc
MAKE = make
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

# Directories
LIBFT_DIR = lib/libft
FT_PRINTF_DIR = lib/ft_printf
INCLUDE_DIR = include
OBJ_DIR = obj

# Include paths for libraries and headers
INC = -I $(INCLUDE_DIR) -I $(LIBFT_DIR)/include -I $(FT_PRINTF_DIR)/include
LIBRARIES = -L $(LIBFT_DIR) -lft -L $(FT_PRINTF_DIR) -lftprintf

# Targets
all: $(SERVER) $(CLIENT)

# Source and Object files
SERVER_SRC = $(wildcard $(SRC_DIR)/server*.c) $(wildcard $(SRC_DIR)/util*.c)
CLIENT_SRC = $(wildcard $(SRC_DIR)/client*.c) $(wildcard $(SRC_DIR)/util*.c)
SERVER_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SERVER_SRC))
CLIENT_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CLIENT_SRC))

# Ensure libft.a is built before libftprintf.a
$(LIBFT_DIR)/libft.a:
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(FT_PRINTF_DIR)/libftprintf.a: $(LIBFT_DIR)/libft.a
	@echo "Building libftprintf..."
	@$(MAKE) -C $(FT_PRINTF_DIR)

$(SERVER): $(SERVER_OBJ) $(FT_PRINTF_DIR)/libftprintf.a
	@echo "SERVER_OBJ: $(SERVER_OBJ)"
	@echo "Linking $(SERVER) with libraries"
	@$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER) $(INC) $(LIBFT_DIR)/libft.a $(FT_PRINTF_DIR)/libftprintf.a $(LIBRARIES)
	@echo "$(SERVER) built successfully."

$(CLIENT): $(CLIENT_OBJ) $(FT_PRINTF_DIR)/libftprintf.a
	@echo "CLIENT_OBJ: $(CLIENT_OBJ)"
	@echo "Linking $(CLIENT) with libraries"
	@$(CC) $(CFLAGS) $(CLIENT_OBJ) -o $(CLIENT) $(INC) $(LIBFT_DIR)/libft.a $(FT_PRINTF_DIR)/libftprintf.a $(LIBRARIES)
	@echo "$(CLIENT) built successfully."

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $< to $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

# Ensure object directories exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Clean object files
clean:
	@echo "Cleaning object files..."
	$(RM) $(OBJ_DIR)/*.o
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(FT_PRINTF_DIR) clean
	@echo "Object files cleaned."

# Clean all files
fclean: clean
	@echo "Cleaning executables and libraries..."
	$(RM) $(SERVER) $(CLIENT)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(FT_PRINTF_DIR) fclean
	@echo "All files cleaned."

# Rebuild everything
re: fclean all
	@echo "ALL rebuild.."

# Debug target
debug: CFLAGS += -g
debug: re

.PHONY: all clean fclean re debug
