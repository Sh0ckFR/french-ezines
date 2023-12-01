##
## Makefile for Polymorphic Shellcode Engine project
## 
## Made by eee
## 
## Started on  Thu Apr 14 00:33:41 2005 eee
## Last update Sat Apr 16 15:21:24 2005 eee
##

NAMEB	=	bind.bin
NAMEC	=	connect.bin

all	:
		nasm -f bin bind.asm -o $(NAMEB)
		nasm -f bin connect.asm -o $(NAMEC)

clean	:
		rm -f $(NAMEB) $(NAMEC) *~

re	:	clean all
