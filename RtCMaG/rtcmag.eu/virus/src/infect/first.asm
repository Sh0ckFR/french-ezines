; Ce code source est issu de l'article: Infection, partie 2
; par DoxtorL/[T.I]. Créé en Février 2001.
;
;
; Ce programme est une illustration élémentaire de l'utilisation
; d'APIs Win32 pour la manipulation de fichiers.
; Ce programme ouvre un fichier dont le nom et la taille sont déterminés dans
; le code source qui suit.
;
; Si les deux premiers octets de ce fichiers sont "VI" le fichier est refermé 
; et laissé tel quel. S'il ne contient pas "VI" à son tout début, 
; elles sont ajoutés et le mot "VIRUS" est ajouté à la fin du fichier.
; Pour ne pas alourdir le listing j'ai omis volontairement les tests
; habituels qui suivent chaque appel à une API; par exemple, aucun test
; n'est effectué sur la réussite de l'opération d'ouverture du fichier.
;
;
; pour tester ce qui suit vous avez besoin d'un fichier de
; 100 octets que vous appellerez: cible.txt (ces paramètres peuvent être
; modifiés)


.386p                 

.model flat


.data

HandleFichier        dd 0
HandleMap            dd 0
AdresseMap           dd 0
TailleMap            dd 0

TailleFichier        dd 100                 ; vous pouvez changer ces deux valeurs
NomFichier           db "cible.txt",0       ; pour tester

.code


DEBUT:                        

    extrn CreateFileA          :Proc        ;
    extrn CreateFileMappingA   :Proc        ;
    extrn MapViewOfFile        :Proc        ;
    extrn UnmapViewOfFile      :Proc        ;
    extrn CloseHandle          :Proc        ;
    extrn SetFilePointer       :Proc        ;
    extrn SetEndOfFile         :Proc        ;
    extrn ExitProcess          :Proc        ;




    mov eax,dword ptr [TailleFichier]       ; on met dans eax la taille du fichier

; on s'occupe maintenant de la taille qu'aurra l'image:

    mov dword ptr [TailleMap],eax           ;
    add dword ptr [TailleMap],5             ; pour tenir compte de l'ajout des
                                            ; 5 lettres du mot "VIRUS"
    call Ouvrir                             ; on ouvre et mappe le fichier
    mov edx,dword ptr [AdresseMap]          ; on met dans edx l'adresse de début de l'image


; on teste les deux premiers octets:

    cmp word ptr [edx],"IV"                 ; remarquez que "VI" est inversée.
                                            ; La lecture en mémoire par le micropro-
                                            ; cesseur est faite à l'envers.
    jnz PasInfecté                          ;
    sub dword ptr [TailleMap],5             ; le fichier est déja modifié. Pour ne
                                            ; pas accroitre inutilement sa taille 
                                            ; on soustrait ce que l'on avait précedemment
                                            ; ajouté.
    call Fermer                             ; le fichier a déja été modifié. 
    call ExitProcess                        ; On demande la fin du programme.


PasInfecté:                                 ; Le fichier n'a pas encore été modifié.

    mov word ptr [edx],"IV"                 ; VI est ajouté au tout début de l'image
                                            ; du fichier en mémoire.
    add edx,dword ptr [TailleFichier]       ; on veut pointer sur la fin réelle du fichier
                                            ; en mémoire. On pointe maintenant sur la
                                            ; zone de 5 octets que l'on s'est octroyé.
    mov dword ptr [edx],"URIV"              ; Le mot "VIRUS" est  écris a la fin
    mov byte ptr [edx+4],"S"                ; de l'image du fichier
    call Fermer                             ; le fichier est maintenant modifié et
                                            ; refermé.                               
    call ExitProcess                        ;

Ouvrir:


; ouverture du fichier:

    pushad                                  ; sauvegarde des registres 

    push 0                                  ;
    push 0                                  ;
    push 3                                  ;
    push 0                                  ;
    push 1                                  ;
    push 80000000h or 40000000h             ;
    lea eax,NomFichier                      ;
    push eax                                ;
    call CreateFileA                        ;

    mov dword ptr [HandleFichier],eax       ; on sauvegarde le handle du fichier

; prépare le mapping:

    push 0                                  ;
    push dword ptr [TailleMap]              ;
    push 0                                  ;
    push 4                                  ;
    push 0                                  ;
    push dword ptr [HandleFichier]          ;
    call CreateFileMappingA                 ;

    mov dword ptr [HandleMap],eax           ; on sauvegarde ce handle

; le fichier est mappé:

    push dword ptr [TailleMap]              ;
    push 0                                  ;
    push 0                                  ;
    push 2                                  ;
    push dword ptr [HandleMap]              ;
    call MapViewOfFile                      ;

    mov dword ptr [AdresseMap],eax          ; on sauvegarde l'adresse de début de l'image
                                            ; crée en mémoire  
    popad                                   ; restauration des registres
    ret                                     ; retour au programme principal


Fermer:

    pushad                                  ;

; l'image crée en mémoire est supprimée et le fichier va prendre en compte
; les modifications apportées.

    push dword ptr [AdresseMap]             ;
    call UnmapViewOfFile                    ;

    push dword ptr [HandleMap]              ;
    call CloseHandle                        ;

; on retaille le fichier si nécessaire:

    push 0                                  ;
    push 0                                  ;
    push dword ptr [TailleMap]              ;
    push dword ptr [HandleFichier]          ;
    call SetFilePointer                     ;

    push dword ptr [HandleFichier]          ;
    call SetEndOfFile                       ;

; on referme le fichier:

    push dword ptr [HandleFichier]          ;
    call CloseHandle                        ;

    popad                                   ;
    ret                                     ; retour au programme principal

end DEBUT