.386p               
.model flat         


.data              

db 0              

.code

    extrn ExitProcess:Proc  


DEBUT:

    push 0
    call ExitProcess

end DEBUT