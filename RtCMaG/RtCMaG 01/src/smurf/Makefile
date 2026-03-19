##############################################################################
#               RtC Smurf Makefile by S/ash (members of RtC)                 #	
#     trivial makefile for RtC Smurf : tested on some linux engines          #
# RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st          #
# RtC Tech : http://www.rtctech.fr.st                                        #
# S/ash    : sl4sh@ifrance.com                                               #
##############################################################################

all:		disp mdir compile link clean

disp:
		@echo "/------------------------------------------------------------------------------\\"
		@echo "|                                 RtC Smurf                                    |"
		@echo "|                    Created by Sly and S/ash (members of RtC)                 |"
		@echo "|                                Smurfing DoS                                  |"
		@echo "|                         This is a tool of RtC Tech                           |"
		@echo "| RtC      : http://www.rtc.fr.st      --         email : rtc@fr.st            |"
		@echo "| RtC Tech : http://www.rtctech.fr.st                                          |"
		@echo "| Sly      : rtcsly@yahoo.fr                                                   |"
		@echo "| S/ash    : sl4sh@ifrance.com                                                 |"
		@echo "\\------------------------------------------------------------------------------/"

mdir:
		@mkdir -p obj

compile:	ipraw.o rtcsmurf.o

link:
		gcc       -o rtcsmurf         obj/*.o

ipraw.o:
		gcc -c    -o obj/ipraw.o      ipraw.c

rtcsmurf.o:
		gcc -c    -o obj/rtcsmurf.o   rtcsmurf.c

clean:
		@rm -rf obj
		@rm -f  *.o
		@rm -f  *~
		@rm -f  core
