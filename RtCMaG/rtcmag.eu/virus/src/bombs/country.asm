;-------[  RtC Mag, At the end of the universe  ]

;--------------[  Country  ]

COUNTRY:
	mov ah,38h
	mov al,0
	mov dx,offset buffer
	int 21h
	cmp bx,XX
	ret

;-------[  EOF
