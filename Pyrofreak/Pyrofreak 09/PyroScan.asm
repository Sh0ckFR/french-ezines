.model tiny
.stack
.code
  org 200h


presentation     db "Bienvenue dans pyroscan",13,10,'$'

init             db "[I] Initialiser le modem$"
parleur          db "[H] Reglage des hauts parleurs$"
parleurm         db "Reglage du volume des hauts parleurs",13,10, '$'

tel              db "[T] Telephone d'acces$"
sort             db "[Q] quitter$"
back             db "[R] Retour au menu?$"
exit2            db "Merci d'utiliser pyroscan!",13,10,'$'
choix            db "Commande:$"

begin:

;initialisation du PSP
      mov ax,@data
      mov ds,ax

;efface ecran
      mov ax,0013h
      int 10h
      mov ax,003h
      int 10h

      mov dh,2
      mov dl,25
      mov ah,02h
      xor bh,bh
      int 10h

;appel du texte de saisie
      mov dx,offset presentation
      mov ah,09h
      int 21h


      mov dh,8
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h

;appel du texte de saisie
      mov dx,offset init
      mov ah,09h
      int 21h
      mov dh,9
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h

;appel du texte de saisie
      mov dx,offset parleur
      mov ah,09h
      int 21h

      mov dh,10
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h
      mov dx,offset tel
      mov ah,09h
      int 21h

      mov dh,11
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h
      mov dx,offset sort
      mov ah,09h
      int 21h

      mov dh,23
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h
      mov dx,offset choix
      mov ah,09h
      int 21h



; la ligne mov ah,01h est indispensable pour chaque saisie de touche
; sinon rien ne fonctionne

      mov ah,01h
      int 21h

      cmp al,'i'    ; si la touche minuscule a est appuyee alors on affiche 
      je menu       ; le menu:

      cmp al,'h'
      je menu1

      cmp al,'t'
      je menu2

      cmp al,'q'    ; si la touche q minuscule est appuyee alors on affiche
      je sortie     ; le menu de sortie

menu:
     mov ax,0013h               ;appel du mode texte
     int 10h                    ;mode video
     mov ax,0003h               ;efface l'ecran
     int 10h                    ;mode video

     mov dx,offset presentation ;appel du texte 
     mov ah,09h                 ;procedure d'affichage
     int 21h                    ;du mode msdos

      mov dh,22
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h

     mov dx,offset back         ;appelle de la saisie2
     mov ah,09h                 ;affiche le message
     int 21h                    ;sous msdos

     mov ah,01h                 ;appel du mode saisie,sans ca rien ne 
     int 21h                    ;fonctionne
     
     cmp al,'r'                 ;si la touche r minuscule est appuyee alors
     jmp begin                  ;on retourne au menu principale

menu1:
     mov ax,0013h               ;appel du mode texte
     int 10h                    ;mode video
     mov ax,0003h               ;efface l'ecran
     int 10h                    ;mode video
     
     mov dx,offset parleurm
     mov ah,09h
     int 21h


     mov dh,22
     mov dl,1
     mov ah,02h
     xor bh,bh
     int 10h

     mov dx,offset back         ;appelle de la saisie2
     mov ah,09h                 ;affiche le message
     int 21h                    ;sous msdos

     mov ah,01h                 ;appel du mode saisie,sans ca rien ne 
     int 21h                    ;fonctionne
     
     cmp al,'r'                 ;si la touche r minuscule est appuyee alors
     jmp begin                  ;on retourne au menu principale


menu2:
     mov ax,0013h               ;appel du mode texte
     int 10h                    ;mode video
     mov ax,0003h               ;efface l'ecran
     int 10h                    ;mode video


     mov dx,offset back         ;appelle de la saisie2
     mov ah,09h                 ;affiche le message
     int 21h                    ;sous msdos

     mov ah,01h                 ;appel du mode saisie,sans ca rien ne 
     int 21h                    ;fonctionne
     
     cmp al,'r'                 ;si la touche r minuscule est appuyee alors
     jmp begin                  ;on retourne au menu principale

sortie:


      mov dh,24
      mov dl,1
      mov ah,02h
      xor bh,bh
      int 10h

     mov dx,offset exit2        ;procedure habituel d'affichage de texte
     mov ah,09h
     int 21h

     mov al,00h                 ;reinitialise le clavier avant la sortie
     mov ax,4c00h               ;retourne sous le promt (win95)
     int 21h                    ;shell msdos
end begin


;tasm /o menu.asm
;tlink menu.obj









