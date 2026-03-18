.386p

.model flat


.data

Titre      db "mon programme",0             ;titre de la fenêtre
Message    db "Chouette une fenêtre!",0     ;message à afficher


.code

    extrn MessageBoxA: Proc
    extrn ExitProcess: Proc


DEBUT:
 
    push 0                                  ;style de la fenêtre
    push offset Titre 
    push offset Message
    push 0

    call MessageBoxA                        ;créer la fenêtre

    push 0                                  ;sortie du programme
    call ExitProcess

end DEBUT