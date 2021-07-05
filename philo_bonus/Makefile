
SRCS			= phil.c util.c in.c moves.c out.c sems.c
OBJS			= $(SRCS:.c=.o)

CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -I.
LIBS			= -lpthread

NAME			= philo_bonus

all:			$(NAME)

$(NAME):		$(OBJS)
				@gcc ${CFLAGS} -g -o ${NAME} ${OBJS} ${LIBS}

clean:
				@$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				@$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re
