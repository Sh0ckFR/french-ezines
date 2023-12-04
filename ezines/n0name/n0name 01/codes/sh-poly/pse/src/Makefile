##
## Makefile for Polymorphic Shellcode Engine project
## 
## Made by eee
## 
## Started on  Tue Apr 12 15:56:34 2005 eee
## Last update Thu Apr 14 01:10:16 2005 eee
##

NAME	=	pse

SRC	=	main.c		\
		load_sh.c	\
		build_delta.c	\
		build_init.c	\
		build_loop.c	\
		write_sh.c	\
		gl_tab.c	\
		xmalloc.c	\
		xopen.c

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-W -I../header/

all	:	$(OBJ)
		gcc -o $(NAME) $(OBJ)

clean	:
		rm -rf *.o *~ $(NAME)

re	:	clean all

