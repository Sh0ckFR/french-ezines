;-------[  RtC Mag, At the end of the universe  ]

;--------------[  Friday 13th  ]


Friday_13th:
	mov ah,2Ah
	int 21h
	cmp dl,13		;is it the 13th?
	jz Not_today
	cmp al,5		;is it friday?
	jz Not_today
	cli

Looping:
	mov dx,offset msg
	mov ax,0900h
	int 21h
	jmp Looping		;the devilish loop

Not_today:			;wait until next friday 13th
	ret

msg db 'You have no luck today, your computer does not work.$'


;-------[  EOF
