# COLORS #
GREEN = @echo "\033[0;32m"
BLUE = @echo "\033[0;34m" 
PURPLE = @echo "\033[0;35m"
CYAN = @echo "\033[0;36m"
RESET = "\033[1;0m"

# RESOURCES #
SRCS =	srcs/main.c \
		srcs/elf.c \
		srcs/debug.c \
		srcs/huffman.c

# FLAGS #
OBJS_DIR = objets
OBJS = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)

NAME = woody_woodpacker

LIBFT_A = libft/libft.a

CC = gcc

CFLAGS = -std=c99   -Wall -Wextra -fsanitize=address #-Werror

# Instructions #
all: $(NAME)

$(NAME): $(OBJS)
# @make -C ./libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) 
#$(LIBFT_A)
	$(BLUE) NM set Up $(RESET)

$(OBJS_DIR)/%.o: srcs/%.c
#Creamos las carpetas necesarias para compilar
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -c $<  -o $@
	$(CYAN) NM Object Compiled $< $(RESET)

clean:
# @make fclean -s -C./libft
	@rm -rf $(OBJS_DIR)
	$(PURPLE) Cleaned NM Objects $(RESET)

fclean: clean
	rm -rf $(NAME)
	$(PURPLE) Cleaned NM Executable $(RESET)	

re: fclean all

.PHONY: all clean fclean re test





