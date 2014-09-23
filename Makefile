##
## Makefile for  in /home/tolsa_c//projets/malloc/malloc_simple
## 
## Made by camille tolsa
## Login   <tolsa_c@epitech.net>
## 
## Started on  Wed Jan 25 13:52:05 2012 camille tolsa
## Last update Sun Feb  5 20:24:40 2012 camille tolsa
##

NAME= 	libmy_malloc_$(HOSTTYPE).so

SRC=	malloc.c

CC =	gcc

OBJS=	$(SRC:.c=.o)

CFLAGS=	-fPIC

all:	$(OBJS)
	$(CC) -shared -o $(NAME) $(OBJS) -lpthread

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all 
