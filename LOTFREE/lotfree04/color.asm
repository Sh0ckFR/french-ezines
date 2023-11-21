;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; assemble with NASM:
;	nasm -f bin -o bios13.com bios13.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

org 100h
[section .data]
string1:
	db "                                    A venir                                     ",13,10,'$'
string2:
	db 'L', 0x90, 'a', 0x98, 'n', 0x97,'d', 0x9f,' ',0x00, 'o', 0x0e,'f',0x21,' ',0x00,'T',0x0a,'h',0x0e,'e',0x0c,' ',0x00,'F',0xf1,'r',0xf9,'e',0xf3,'e',0xfb,' ',0x00,'#',0x09, '4', 0x0f,13,10,'$'
string3:
	db "Au programme :",13,10,'$'
string4:
	db "Fais bouger ton DOS avec l'assembleur !!!",13,10,'$'
string5:
	db "Obtenir passwd non shadowed + cracker",13,10,'$'
string6:
	db "Et un tas d'autre trucs... (Laissez moi le temps ;-)",13,10,'$'
string7:
	db "Par Sirius Black (sirius.black@lycos.fr)",13,10,'$'
string8:
	db "Fuck F-Haine",13,10,'$'

[section .text]
	push ax
	push bx
	push cx
	push dx

; effacer l'ecran
	mov al,0
	mov ah,6

	mov bh,7
	mov cx,0
	mov dh,24
	mov dl,79
	int 0x10

; write string
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x3f ; attribute
	mov cx,80 ; string length
	mov dh,0 ; starting row
	mov dl,0 ; starting col
	push cs
	pop es
	mov bp,string1
	int 10h

; same as above, with attribute bytes in the string
	mov ah,13h
	mov al,3 ; write mode (advance cursor, ASCII+attribute string)
	mov bh,0 ; video page
	mov cx,19 ; string length
	mov dh,2 ; starting row
	mov dl,0 ; starting col
	push cs
	pop es
	mov bp,string2
	int 10h


; write string3
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x03 ; attribute
	mov cx,14 ; string length
	mov dh,4 ; starting row
	mov dl,2 ; starting col
	push cs
	pop es
	mov bp,string3
	int 10h

; write string4
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x0c ; attribute
	mov cx,41 ; string length
	mov dh,6 ; starting row
	mov dl,4 ; starting col
	push cs
	pop es
	mov bp,string4
	int 10h

; write string5
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x06 ; attribute
	mov cx,39 ; string length
	mov dh,8 ; starting row
	mov dl,4 ; starting col
	push cs
	pop es
	mov bp,string5
	int 10h

; write string6
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x0d ; attribute
	mov cx,52 ; string length
	mov dh,10 ; starting row
	mov dl,4 ; starting col
	push cs
	pop es
	mov bp,string6
	int 10h

; write string7
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x0e ; attribute
	mov cx,40 ; string length
	mov dh,15 ; starting row
	mov dl,37 ; starting col
	push cs
	pop es
	mov bp,string7
	int 10h

; write string8
	mov ah,13h
	mov al,1 ; write mode (advance cursor, ASCII string)
	mov bh,0 ; video page
	mov bl,0x02 ; attribute
	mov cx,12 ; string length
	mov dh,18 ; starting row
	mov dl,2 ; starting col
	push cs
	pop es
	mov bp,string8
	int 10h

; exit to DOS
	mov ax,4C00h
	int 21h
	pop dx
	pop cx
	pop bx
	pop ax

	ret