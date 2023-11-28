{
   Cr‚‚ par Sabrina
   sabrina@inorbit.com pour tout bug, remarques, critiques, erreurs...
   PILE.PAS : Illustration du type Liste Chain‚e (PILE)
}

Program Pile; { Illustration de la structure de Pile }
Type
    Objet = Integer; { On prend des integer pour simplifier les exemples }
    Liste = ^Noeud;  { Pointeur vers un type Liste }
    Noeud = Record
             Obj: Objet;  { Notre Objet }
             Suiv : Liste;{ Le Suivant  }
            End;

FUNCTION CREER ( Obj : Objet ): Liste;
{ Creation d'une liste de 1 ‚l‚ment }
VAR L : Liste;
BEGIN
 New(L);         { Allocation m‚moire de taille L }
 L^.Obj:= Obj;   { On mets notre objet }
 L^.Suiv:= NIL;
 CREER:= L;
END;

FUNCTION EST_VIDE ( L : Liste ) : Boolean;
{ True si L est vide }
BEGIN
 EST_VIDE:= ( L=NIL );
END;

FUNCTION SUIVANT ( L : Liste ) : Liste;
{ Renvoie la liste suivant L }
BEGIN
 SUIVANT:= L^.Suiv;
END;

FUNCTION TETE ( L:Liste ) : Objet ;
BEGIN
 TETE:= L^.Obj;
END;

PROCEDURE AFFICHE_LISTE( L : Liste );
{ Procedure d'affichage de Liste }
BEGIN
 WHILE ( NOT( EST_VIDE(L) )) DO
 BEGIN
  Write(L^.obj, '->');
  L:= SUIVANT(L);
 END;
WriteLn('Nil');
END;

FUNCTION AJOUTER ( Obj : Objet ; L : Liste ) : Liste;
{ Ajoute Obj au d‚but de la liste L }
{ Ce qui caract‚rise le type Pile }
VAR LNouvRecord : Liste;
BEGIN
 LNouvRecord:= CREER( Obj );
 LNouvRecord^.Suiv:= L;
 AJOUTER:= LNouvRecord;
END;


FUNCTION AJOUTERK ( Obj : Objet ; VAR LP : Liste ; k : integer): Liste;
{ Ajoute Obj … la kiŠme position dans la liste L }
{ Si k est plus grand que la taille de la liste on ajoute … la fin de L }
VAR LNouvObj : Liste;
    L : Liste;
BEGIN
 L:=LP; { On manipule LP par L }
 LNouvObj:= CREER( Obj ); { Cr‚ation d'une Liste … 1 ‚l‚ment }
 WHILE ( NOT( EST_VIDE( L )) and (k>1)) DO
       BEGIN
       L:= SUIVANT( L );    { On parcours la liste L jusqu'… arriver au }
       k:= k-1;             { kieme ‚l‚ment                             }
       END;
 If ( EST_VIDE( L ) ) THEN
  L:=LNouvObj { On est arriv‚ plus loin que la taille de L }
 ELSE BEGIN
       LNouvObj^.Suiv:= SUIVANT( L ); { On met la suite de L derriŠre }
       L^.Suiv:= LNouvObj;
      END;
 AJOUTERK:= LP;
END;

FUNCTION SUPPRIMERK ( LP : Liste ; k : integer): Liste;
{ Suppression du kieme ‚l‚ment                  }
{ Si k est plus grand que la taille de la liste }
{ Alors on supprime le dernier ‚l‚ment de L     }
VAR L : Liste;
BEGIN
 L:=LP;
 IF NOT( EST_VIDE( L ) ) THEN
 BEGIN
  WHILE ( NOT( EST_VIDE( L^.Suiv ) ) and ( k>2 ) ) DO BEGIN
   L:= SUIVANT( L );    { On parcours la liste L jusqu'… arriver au }
   k:= k-1;             { kieme ‚l‚ment                             }
   END;
  If ( EST_VIDE( L^.Suiv ) ) THEN
   L^.Suiv:= NIL        { On est arriv‚ plus loin que la taille de L }
  ELSE L^.Suiv:= (L^.Suiv)^.Suiv; {On en saute un }
 END;
 SUPPRIMERK:= LP;
END;


FUNCTION INSERER ( Obj : Objet ; LP : Liste ): Liste;
{ La liste L est suppos‚ tri‚ }
VAR LNouvRecord : Liste;
    L, Prec : Liste; { Prec le pointeur pr‚cedent L^.Suiv }
BEGIN
 L:=LP;
 IF ( EST_VIDE( L ) ) THEN L:= CREER( Obj )
 ELSE BEGIN
       Prec:= L;
       { La liste est tri‚ - cette information nous sert pour savoir quand }
       { arreter }
       WHILE (( L^.Obj>Obj )  and NOT( EST_VIDE( L ) ) ) DO BEGIN
        Prec:= L;         { On stocke le Prec (pour aprŠs) }
        L:= SUIVANT( L ); { On parcours...                 }
       END;

       IF ( EST_VIDE( L ) ) THEN Prec^.Suiv:= CREER( Obj )
                                 { On est arriv‚ … la fin de L }
       ELSE If ( Prec<>L  ) THEN BEGIN
                                  { Gestion du dernier ‚l‚ment de la liste }
                                  LNouvRecord:= CREER( Obj );
                                  LNouvRecord^.Suiv:= L;
                                  Prec^.Suiv:= LNouvRecord;
                                 END
                            ELSE LP:= Ajouter( Obj, LP );
      END;
INSERER:= LP;
END;

{ Programme d'Illustration }

VAR L : Liste;
BEGIN
L:=CREER( 5 );
Affiche_Liste(L);
Writeln('Ajout de 10, 17 et 80:');

L:= Ajouter(15, L);
L:= Ajouter(17, L);
L:= Ajouter(80, L);
Affiche_Liste(L);

writeln;
writeln('Tete    : ',Tete(L));
write(  'Suivant : ');
Affiche_Liste( Suivant( L ));
writeln;
writeln('Ajout de 12 … la position 3');
L:= AjouterK(12, L, 3);
Affiche_Liste(L);

writeln('Supprimer 17 … la position 2');
L:= SupprimerK( L, 2);
Affiche_Liste(L);

Writeln('Insertion de 13 dans la liste tri‚:');
L:=Inserer(13, L);
Affiche_Liste(L);
readln;
END.
