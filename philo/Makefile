# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: badal-la <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 17:41:05 by badal-la          #+#    #+#              #
#    Updated: 2025/03/18 17:58:49 by badal-la         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo

CC		=	cc
CFLAGS	=	-g -Wall -Wextra -Werror

SRCDIR	=	Source
INCDIR	=	include

SRCS	=	$(SRCDIR)/errors.c \
			$(SRCDIR)/init_and_check_param.c \
			$(SRCDIR)/main.c \
			$(SRCDIR)/monitoring.c \
			$(SRCDIR)/philo_routine.c \
			$(SRCDIR)/utils.c \
			$(SRCDIR)/utils2.c

OBJS	=	$(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo ✅ "Philo compiled\n"

clean:
	rm -f $(OBJS)
	@echo ✅ "Objects files removed\n"

fclean: clean
	rm -f $(NAME)
	@echo ✅ "Executables and objects files removed\n"

re: fclean $(NAME)

.PHONY: clean fclean re