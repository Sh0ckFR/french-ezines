{
   Cr‚‚ par Sabrina
   sabrina@inorbit.com pour tout bug, remarques, critiques, erreurs...
   FILE.PAS : Illustration du type Liste Chain‚e (FILE)
}

Program Liste_File;
Type Objet  = Integer;
     Liste   = ^Cellule; { Pointeur vers un type une cellule }
     Cellule = Record
                Obj  : Objet;  { Notre objet }
                Suiv : Liste;  { Notre objet suivant }
               End;
     Filee = Record  { file est un mot r‚serv‚ => on redouble le e final }
              Prem : Liste;
              Der  : Liste;
             End;


PROCEDURE CREER_FILE ( VAR F : Filee );
BEGIN
 F.Prem := NIL;
 F.Der  := NIL;
END;

FUNCTION EST_VIDE ( VAR F : Filee ): Boolean;
BEGIN
 EST_VIDE:= ( F.Prem=Nil );
END;

PROCEDURE AFFICHE_LISTE( F : Filee );
{ Procedure d'affichage de Liste }
BEGIN
 WHILE ( NOT( EST_VIDE(F) )) DO
 BEGIN
  Write( (F.Prem)^.Obj, '->');
  F.Prem:= (F.Prem)^.Suiv;
 END;
 WriteLn('Nil');
END;

PROCEDURE DEFILER ( VAR F : Filee );
BEGIN
 If NOT( EST_VIDE( F ) ) THEN
 F.Prem:= F.Prem^.Suiv;
END;

PROCEDURE ENFILER ( Obj : Objet ; VAR F : Filee );
VAR Nouveau : Liste;
BEGIN
 New( Nouveau );
 Nouveau^.Obj:=  Obj;
 Nouveau^.Suiv:= NIL;
 F.Der^.Suiv:= Nouveau;
 F.Der:= Nouveau;
 IF ( F.Prem=NIL ) THEN F.Prem:= Nouveau;
END;

{ Programme d'Exemple }
VAR F: Filee;
BEGIN
 CREER_FILE( F );
 WriteLn( 'On enfile les ‚l‚ments 1, 5, 10 et 18:');
 ENFILER ( 1 , F);
 ENFILER ( 5 , F);
 ENFILER ( 10, F);
 ENFILER ( 18, F);
 AFFICHE_LISTE( F );
 WriteLn('On defile un ‚l‚ment');
 DEFILER ( F );
 AFFICHE_LISTE( F );
 ReadLn;
END.