CLS
DIM vale(15)
DIM code(16)
DIM droit(14)
DIM gauche(14)
DIM resultat(50)
DIM gf(15)
code(1) = 72
code(2) = 238
code(3) = 118
code(4) = 29
code(5) = 103
code(6) = 105
code(7) = 161
code(8) = 27
code(9) = 122
code(10) = 140
code(11) = 71
code(12) = 248
code(13) = 84
code(14) = 149
PRINT " Prog By Furtif"
PRINT " "
PRINT "Tout doit etre en minuscule"
PRINT "entre les valeur hexadecimal que tu as trouv‚"
INPUT "dans la base de registre en <screensave_data>"; val$
o = 0
FOR i = 1 TO 28 STEP 2
o = o + 1
a$ = MID$(val$, i, 2)
droit$ = MID$(a$, 2, 1)
IF droit$ = "1" OR droit$ = "2" OR droit$ = "3" OR droit$ = "4" OR droit$ = "5" OR droit$ = "6" OR droit$ = "7" OR droit$ = "8" OR droit$ = "9" THEN
droit(o) = VAL(droit$)
END IF
IF droit$ = "a" THEN
droit(o) = 10
END IF
IF droit$ = "b" THEN
droit(o) = 11
END IF
IF droit$ = "c" THEN
droit(o) = 12
END IF
IF droit$ = "d" THEN
droit(o) = 13
END IF
IF droit$ = "e" THEN
droit(o) = 14
END IF
IF droit$ = "f" THEN
droit(o) = 15
END IF
g$ = MID$(a$, 1, 1)
IF g$ = "1" OR g$ = "2" OR g$ = "3" OR g$ = "4" OR g$ = "5" OR g$ = "6" OR g$ = "7" OR g$ = "8" OR g$ = "9" THEN
gauche(o) = VAL(g$)
END IF
IF g$ = "a" THEN
gauche(o) = 10
END IF
IF g$ = "b" THEN
gauche(o) = 11
END IF
IF g$ = "c" THEN
gauche(o) = 12
END IF
IF g$ = "d" THEN
gauche(o) = 13
END IF
IF g$ = "e" THEN
gauche(o) = 14
END IF
IF g$ = "f" THEN
gauche(o) = 15
END IF
gauche(o) = gauche(o) * 16
resultat(i) = gauche(o) + droit(o)
NEXT i
h = 0
FOR i = 1 TO 28 STEP 2
h = h + 1
resultat(h) = resultat(i)
NEXT i
PRINT "le mot de passe doit etre :"
FOR i = 1 TO 14
IF resultat(i) = 0 THEN
EXIT FOR
END IF
PRINT CHR$(resultat(i) XOR code(i));
NEXT i




