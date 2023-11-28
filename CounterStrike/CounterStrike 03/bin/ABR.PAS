{
   Cr‚‚ par Sabrina
   sabrina@inorbit.com pour tout bug, remarques, critiques, erreurs...
   ABR.PAS : Implantation d'Arbre Binaire de Recherche (ABR)
}

Program Arbre_ABR;
Type  Objet = Integer;  { Notre Record pour signaler l'objet de travail }
       Arbre= ^Noeud;   { Arbre pointe vers un Noeud }
       Noeud = Record
                Obj: Objet;      { Notre Objet     }
                FGauche : Arbre; { Le fils Gauche  }
                FDroit  : Arbre; { et le Droit     }
               End;

PROCEDURE CREER ( Obj : Objet ; VAR Abr : Arbre);
{ Cr‚ation d'un arbre Binaire }
BEGIN
 New(Abr);
 Abr^.Obj:= Obj;
 Abr^.FGauche:= NIL;
 Abr^.FDroit:= NIL;
END;

FUNCTION GAUCHE ( Abr : Arbre ) : Arbre;
{ Retourne de l'arbre GAUCHE }
BEGIN
 GAUCHE:= Abr^.FGauche;
END;

FUNCTION DROIT ( Abr : Arbre ) : Arbre;
{ Retourne L'arbre DROIT }
BEGIN
 DROIT:= Abr^.FDroit;
END;

FUNCTION INSERER ( Obj : Objet ; Abr: Arbre ): Arbre;
{ On insere suivant la valeur de Objet pour respecter la structure ABR }
BEGIN
 IF ( Abr = NIL ) THEN CREER(Obj, Abr)
 ELSE IF ( Abr^.Obj > Obj ) THEN Abr^.FGauche:=INSERER(Obj, GAUCHE(Abr))
                            ELSE Abr^.FDroit :=INSERER(Obj, DROIT(Abr ));
 INSERER:= Abr;
END;

PROCEDURE RECHERCHE_MIN ( VAR Obj : Objet ; VAR Abr : Arbre );
{ Retourne dans Obj la valeur du minimum du sous-arbre droit }
{ Supprimer de minimum de Abr et r‚tablie un ABR             }
VAR temp : Arbre;
BEGIN
 IF ( Abr <> NIL) THEN
  IF ( GAUCHE( Abr ) = NIL ) THEN
   BEGIN
    Obj:= Abr^.Obj;
    Abr:= DROIT( Abr );
   END
  ELSE BEGIN
        temp:= DROIT( Abr );
        RECHERCHE_MIN( Obj , temp );
       END
END;

FUNCTION SUPPRIMER ( Obj : Objet ; Abr: Arbre ): Arbre;
{ Supprime Obj de l'ABR }
VAR temp : Arbre;
BEGIN
 IF (Abr<>Nil) THEN
 BEGIN
   IF ( Abr^.Obj > Obj ) THEN Abr^.FGauche:= SUPPRIMER( Obj, GAUCHE( Abr ))
   ELSE IF ( Abr^.Obj < Obj ) THEN Abr^.FDroit:= SUPPRIMER( Obj, DROIT( Abr ))
        ELSE { Obj = Abr^.Obj }
         IF ( DROIT(Abr) = NIL ) THEN Abr:= GAUCHE( Abr ) { 1er CAS }
         ELSE
          IF ( GAUCHE( Abr ) = NIL ) THEN Abr:= DROIT( Abr ) { 2nd CAS }
          ELSE BEGIN { 3eme CAS }
                temp:= GAUCHE( Abr );
                RECHERCHE_MIN( Obj , temp);
                Abr^.FGauche:= Temp;
                Abr^.Obj:= Obj;
               END;
  END;
 SUPPRIMER:= Abr;
END;

FUNCTION RECH ( Obj : Objet ; Abr: Arbre ): Boolean;
{ Renvoie True si l'‚l‚ment est pr‚sent }
BEGIN
 IF ( Abr = NIL ) THEN RECH:= FALSE
 ELSE
  IF ( Abr^.Obj = Obj ) THEN RECH:= TRUE
  ELSE IF ( Obj < Abr^.Obj )     THEN RECH:= RECH ( Obj , GAUCHE( Abr ) )
       ELSE RECH:= RECH (Obj , DROIT( Abr ) )
END;


BEGIN
END.