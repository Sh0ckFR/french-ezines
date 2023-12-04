;#####################DECLARATION

      .386
      .model flat, stdcall  ; 32 bit memory model
      option casemap :none  ; case sensitive

      include Project.inc     ; local includes for this file

.code

start:
      invoke GetModuleHandle, NULL
      mov hInstance, eax

      invoke GetCommandLine
      mov CommandLine, eax

      invoke WinMain,hInstance,NULL,CommandLine,SW_SHOWDEFAULT
      invoke ExitProcess,eax

; #########################################################################

WinMain proc hInst     :DWORD,
             hPrevInst :DWORD,
             CmdLine   :DWORD,
             CmdShow   :DWORD

      ;====================
      ; Put LOCALs on stack
      ;====================

      LOCAL wc   :WNDCLASSEX
      LOCAL msg  :MSG
      LOCAL Wwd  :DWORD
      LOCAL Wht  :DWORD
      LOCAL Wtx  :DWORD
      LOCAL Wty  :DWORD

      ;==================================================
      ; Fill WNDCLASSEX structure with required variables
      ;==================================================

      invoke LoadIcon,hInst,500    ; icon ID
      mov hIcon, eax

      szText szClassName,"Project_Class"

      mov wc.cbSize,         sizeof WNDCLASSEX
      mov wc.style,          CS_HREDRAW or CS_VREDRAW
      mov wc.lpfnWndProc,    offset WndProc
      mov wc.cbClsExtra,     NULL
      mov wc.cbWndExtra,     NULL
      m2m wc.hInstance,      hInst
      mov wc.hbrBackground,  COLOR_BTNFACE+1
      mov wc.lpszMenuName,   NULL
      mov wc.lpszClassName,  offset szClassName
      m2m wc.hIcon,          hIcon
        invoke LoadCursor,NULL,IDC_ARROW
      mov wc.hCursor,        eax
      m2m wc.hIconSm,        hIcon

      invoke RegisterClassEx, ADDR wc

      ;================================
      ; Centre window at following size
      ;================================

      mov Wwd, 300
      mov Wht, 200

      invoke GetSystemMetrics,SM_CXSCREEN
      invoke TopXY,Wwd,eax
      mov Wtx, eax

      invoke GetSystemMetrics,SM_CYSCREEN
      invoke TopXY,Wht,eax
      mov Wty, eax

      invoke CreateWindowEx,0,ADDR szClassName,ADDR szDisplayName,WS_VISIBLE or WS_POPUP,
                            Wtx,Wty,Wwd,Wht,NULL,NULL,hInst,NULL
      mov   hWnd,eax

      invoke ShowWindow,hWnd,SW_SHOWNORMAL
      invoke UpdateWindow,hWnd

      ;===================================
      ; Loop until PostQuitMessage is sent
      ;===================================

    StartLoop:
      invoke GetMessage,ADDR msg,NULL,0,0
      cmp eax, 0
      je ExitLoop
      invoke TranslateMessage, ADDR msg
      invoke DispatchMessage,  ADDR msg
      jmp StartLoop
    ExitLoop:

      return msg.wParam

WinMain endp

; #########################################################################

WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD


    LOCAL var    :DWORD
    LOCAL caW    :DWORD
    LOCAL caH    :DWORD
    LOCAL Rct    :RECT
    LOCAL hDC    :DWORD
    LOCAL Pnt	:PAINTSTRUCT
    LOCAL buffer1[128]:BYTE  ; these are two spare buffers
    LOCAL buffer2[128]:BYTE  ; for text manipulation etc..

    .if uMsg == WM_COMMAND
    .elseif uMsg == WM_CREATE
	invoke CreatePolygonRgn, ADDR pt, NUM_PTS, WINDING
	mov gdi1,eax
	invoke SetWindowRgn, hWin, gdi1, FALSE

	invoke ImageList_Create,300,200,ILC_COLOR32,1,1
	mov hImageList, eax
	invoke LoadBitmap,hInstance,offset MainBmp
	push eax
	invoke ImageList_Add,hImageList,eax,NULL
	pop eax
	invoke DeleteObject, eax

	invoke ImageList_Create,35,15,ILC_COLOR4 or ILC_MASK,1,1
	mov hImageList2, eax
	invoke LoadBitmap,hInstance,offset OuiBmp
	push eax
	invoke ImageList_AddMasked,hImageList2,eax,00FFFFFFh
	pop eax
	invoke DeleteObject, eax
	invoke ImageList_SetBkColor,hImageList2,CLR_NONE

      mov btx, 160
      mov bty, 155

	invoke SetTimer,hWin,2500,100,NULL
	mov TimerID,eax

    .elseif uMsg == WM_TIMER
                add tempecoule, 1

    .elseif uMsg == WM_LBUTTONDOWN
		mov eax,lParam
		and eax,0ffffh
		mov hitpoint.x,eax
		mov eax,lParam
		shr eax,16
		mov hitpoint.y,eax

		.if hitpoint.x >= 203 && hitpoint.x <= 260 && hitpoint.y >= 148 && hitpoint.y <= 176
			mov But1,TRUE
		.else
			mov move,TRUE
			mov eax, hitpoint.x
			mov hitpointold.x, eax
			mov eax, hitpoint.y
			mov hitpointold.y, eax
		.endif

    .elseif uMsg == WM_MOUSEMOVE
		mov eax,lParam
		and eax,0ffffh
		mov hitpoint.x,eax
		mov eax,lParam
		shr eax,16
		mov hitpoint.y,eax

		mov eax, btx
		mov ebx, 35
		add ebx, eax
		mov ecx, bty
		mov edx, 15
		add edx, ecx
		.if hitpoint.x >= eax && hitpoint.x <= ebx && hitpoint.y >= ecx && hitpoint.y <= edx
		invoke Alea
		xor ebx,ebx
		mov ecx,ebx
		mov bl, al
		mov cl, ah
		shr ecx,1
		add ecx, 30
		shr ebx,1
		add ebx, 80
		mov btx, ebx
		mov bty, ecx
		invoke InvalidateRect,hWnd,NULL,FALSE
		.endif

		invoke GetCursorPos,ADDR hitpoint
		mov eax,hitpointold.x
		mov ebx,hitpointold.y
		sub hitpoint.x,eax
		sub hitpoint.y,ebx

		.if move
		invoke SetWindowPos,hWin,NULL,hitpoint.x,hitpoint.y,NULL,NULL,SWP_NOSIZE or SWP_NOZORDER
		.endif

    .elseif uMsg == WM_LBUTTONUP
		mov eax,lParam
		and eax,0ffffh
		mov hitpoint.x,eax
		mov eax,lParam
		shr eax,16
		mov hitpoint.y,eax

		.if hitpoint.x >= 203 && hitpoint.x <= 260 && hitpoint.y >= 148 && hitpoint.y <= 176 && But1
			invoke MessageBox,NULL,ADDR szmsgdefin,ADDR szDisplayName,MB_OK
			invoke SendMessage,hWin,WM_SYSCOMMAND,SC_CLOSE,NULL
		.endif
		
		.if But1
			mov But1,FALSE
		.endif

		.if move
			mov move,FALSE
		.endif

    .elseif uMsg == WM_PAINT
        
	invoke BeginPaint, hWin, ADDR Pnt
        mov hDC, eax
	invoke ImageList_Draw,hImageList,0,hDC,0,0,0
	invoke ImageList_Draw,hImageList2,0,hDC,btx,bty,0
	invoke EndPaint, hDC, ADDR Pnt

	return 0

    .elseif uMsg == WM_CLOSE

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        return 0 
    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

; ########################################################################

TopXY proc wDim:DWORD, sDim:DWORD

    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension

    return sDim

TopXY endp

; #########################################################################

Paint_Proc proc hWin:DWORD, hDC:DWORD

    LOCAL btn_hi   :DWORD
    LOCAL btn_lo   :DWORD
    LOCAL Rct      :RECT

    invoke GetSysColor,COLOR_BTNHIGHLIGHT
    mov btn_hi, eax

    invoke GetSysColor,COLOR_BTNSHADOW
    mov btn_lo, eax

    return 0

Paint_Proc endp

Alea proc
	mov eax,tempecoule
	mul eax
	xor eax, edx
	mul eax
	xor eax, edx
	mul eax
	xor eax, edx
	mov ebx, eax
	add ebx, 05f2fh
	ror ebx, 5
	mov tempecoule, eax
	return ebx
Alea endp

; ########################################################################

end start
