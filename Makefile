.SILENT:

NAME = so_long

CC = cc
CFLAGS = -O3 -g #-Wall -Wextra -Werror -g
MLXFLAGS = -L ./minilibx -lm -lmlx -Ilmlx -lXext -lX11 #-fsanitize=address
RM = rm -rf

INCS	= ./includes
SRCS_DIR= ./srcs
OBJS_DIR= ./obj

SRCF    = so_long fps map player enemies

SRCS= $(addprefix ${SRCS_DIR}/, $(addsuffix .c, ${SRCF}))
OBJS= $(addprefix ${OBJS_DIR}/, $(addsuffix .o, ${SRCF}))

LIBFT = libft/libft.a
MINILIBX = minilibx/libmlx.a

all: deps $(NAME)

deps:
	$(MAKE) -C ./libft
#	$(MAKE) -C ./minilibx

$(NAME): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MINILIBX) $(MLXFLAGS) -o $(NAME)

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c | ${OBJS_DIR}
	${CC} ${CFLAGS} -I${INCS} -c $< -o $@

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}

clean:
	${RM} ${OBJS_DIR}
	printf "clean\033[0;32m  [OK]\n\033[0m"

fclean:	clean
	${RM}	${NAME}
	printf "fclean\033[0;32m [OK]\n\033[0m"

re: fclean all