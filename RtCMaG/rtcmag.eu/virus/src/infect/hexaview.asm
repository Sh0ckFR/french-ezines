.386p
.model flat

.data

db 0                            ; cet octet a une fonction qui se justifie dans ce qui suit
HEX_Message db 0,0,0,0,0,0,0    ;
FIN_Message db 0                ;
            db 0                ;

.code                           ;

    extrn ExitProcess:Proc      ;
    extrn MessageBoxA:Proc      ;


DEBUT:                          ;

; Ce programme affiche le contenu de eax en notation Hexadécimale


    lea esi,FIN_Message         ; esi contient un pointeur vers le dernier caractère
                                ; de la chaine qui va contenir la représentation
                                ; en code ASCII du nombre à afficher en Hexadécimal.
    mov eax,14789               ; valeur dont on veut connaitre l'écriture hexadécimale.
    inc esi                     ; ajouter 1 à la valeur dans esi
    mov ecx,10h                 ; mettre 16 dans ecx


HEX_chiffre:                    ;

    dec esi                     ; soustraire 1 à la valeur dans esi
    xor edx,edx                 ; mettre à zero edx 
    div ecx                     ; division de edx:eax par ecx
                                ; le quotient est dans eax
                                ; le reste dans edx, edx contient la valeur du 
                                ; nombre hexa: 0 à F
 
    cmp eax,0Ah                 ; la valeur dans eax est égale ou plus grande que
    jge Lettre                  ; 10? si oui aller au label Lettre 


    add edx,30h                 ; le chiffre est compris entre 0 et 9 
    mov byte ptr [esi],dl       ; la chaine de caractère est complétee avec le code
                                ; ASCII du chiffre Hexa lu
    or eax,eax                  ; eax est nul?
    jnz HEX_chiffre             ; non? il reste des chiffres à lire
                      

Lettre:

    add edx,61h-0Ah             ; le chiffre lu est compris entre A et F
    mov byte ptr [esi],dl       ;
    or eax,eax                  ;
    jnz HEX_chiffre             ;
       
 
; Afficher le résultat


    push 0                      ;
    push offset Titre           ;
    push offset Hex_Message     ;
    push 0                      ;
    call MessageBoxA            ;

; Exit

    push 0                      ;
    call ExitProcess            ;
      
end DEBUT