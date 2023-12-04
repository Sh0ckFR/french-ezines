
DECLARE SUB code ()
DECLARE SUB decode ()
DECLARE SUB quit ()
CLS
SCREEN 11

PRINT ""
PRINT ""
PRINT ""
PRINT ""
PRINT "              /////  ///////  ///////  //   //"
PRINT "             //     //   //  //        // //"
PRINT "            /////  ///////  ///////     //"
PRINT "           //     //   //       //    //"
PRINT "          /////  //   //  ///////   //"
PRINT "                 CRYPTE           //"
PRINT "       ///////////////////////////"
PRINT ""
PRINT "      AND MAY THE FORCE BE WITH YOU"
PRINT ""
PRINT "eAsY cRYptAgE vErSioN 1.0"
PRINT "bY BubBa"
PRINT "hOmE SitE: www.multimania.com/utils mAiL: toto.best@wanadoo.fr"
PRINT "gReEtZ: cHriStOpHe DeScoTte gEnOn(www.multimania.com/slasky12"
PRINT ""
PRINT ""
PRINT ""


menu:
PRINT "What do you want to do?"
PRINT "1. Crypt a file"
PRINT "2. Uncrypt a file"
PRINT "q. Exit this program"
INPUT "Your choice"; choix%

SELECT CASE choix%
        CASE IS = 1
                code
        CASE IS = 2
                decode
        CASE IS = q
                quit
        CASE ELSE
                PRINT "ERROR 001: Invalide choice, please type 1, 2 or q"
                PRINT ""
                PRINT ""
                GOTO menu
END SELECT

END

erreur:
PRINT "ERROR 002: The file does not exist"

SYSTEM

SUB code
INPUT "Type the path of the file to crypt (Ex: c:\text.txt)", fichier$
ON ERROR GOTO erreur
OPEN fichier$ FOR INPUT AS #1
INPUT "Type the path of the crypte file (Ex: c:\textcryp.txt)", cryp$
OPEN cryp$ FOR OUTPUT AS #2
        DO UNTIL EOF(1)
                tampon$ = INPUT$(1, 1)
                tampon% = ASC(tampon$)
                tampon% = tampon% + 5
                tampon$ = CHR$(tampon%)
                PRINT #2, tampon$;
        LOOP
CLOSE #2
CLOSE #1
PRINT "The file was created successfuly !"
END SUB

SUB decode
INPUT "Type the path of the file to uncrypt (Ex: c:\textcryp.txt)", cryp$
OPEN cryp$ FOR INPUT AS #1
ON ERROR GOTO erreur
INPUT "Type the path of the uncrypt file (Ex: c:\text.txt)", fichier$
OPEN fichier$ FOR OUTPUT AS #2
        DO UNTIL EOF(1)
                tampon$ = INPUT$(1, 1)
                tampon% = ASC(tampon$)
                tampon% = tampon% - 5
                tampon$ = CHR$(tampon%)
                PRINT #2, tampon$;
        LOOP
CLOSE #2
CLOSE #1
PRINT "The file was created !"


END SUB

SUB quit
SHELL "exit"
END SUB

