##
## Makefile for Polymorphic Shellcode Engine project
## 
## Made by eee
## 
## Started on  Tue Apr 12 15:55:08 2005 eee
## Last update Mon Apr 18 16:21:57 2005 eee
##

NAME	=	pse

PSE	=	src/pse

all	:
		@echo ""
		@echo "Building The Most Er0tic Polymorphic Shellcode Engine"
		@echo ""
		@cd src && $(MAKE)
		@cp $(PSE) .

clean	:
		@echo "Cleaning src/"
		@cd src && $(MAKE) -s clean
		@cd header && rm -f *~
		@rm -f $(NAME) *~ 
		@echo "Done"

fclean	:	clean
		rm -f shellcode.bin shellcode.c 
		cd test/ && rm -f shellcode.c shellcode

re	:	clean all

peace	:
		@echo "Not war ..."
