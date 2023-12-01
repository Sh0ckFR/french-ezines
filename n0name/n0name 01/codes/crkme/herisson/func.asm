
extern data1,zone,key

SECTION .toto

global sub_dummy1
global sub_dummy2

; DUMMY2 function ripped from crk3 :)
; Entree :
;	 Gimme chaine in edi
; 	 

sub_dummy2:

      ; First part :-)

      mov    eax,0xfffefdfc
      mov    ecx,0x40

loop2:

      mov    dword [ecx*4+data1-4],eax    ; XXX
      sub    eax,0x4040404
      dec    ecx
      jne    loop2

      ; Second part :-)

      xor    eax,eax

init:
      xor    ebx,ebx
      mov    esi,4
      jmp    poof

loop1:
      inc    bl
      dec    esi
      je     init

poof:

      mov    dl,byte [ecx+data1]
      add    al,byte [edi+ebx]
      add    al,dl
      
      mov    dh,byte [eax+data1]
      mov    byte [ecx+data1],dh		; XXX
      mov    byte [eax+data1],dl		; XXX
      
      inc    cl
      jne    loop1
      ret    


; DUMMY1 :-)
; Entrees : 0x11 -> edi
; zone -> %esi


sub_dummy1:

;      mov    edi,0x11
;      mov    esi,zone

      xor    eax,eax
      xor    ebx,ebx
      xor    ecx,ecx
      xor    edx,edx

loop3:

      inc    bl
      
      mov    dl,byte [ebx+data1]
      add    al,dl
      
      mov    cl,byte [eax+data1]
      mov    byte [ebx+data1],cl
      mov    byte [eax+data1],dl
      add    cl,dl
      
      mov    cl,byte [ecx+data1]

      xor    byte [esi],cl
      inc    esi
      dec    edi
      jne    loop3
      ret    

