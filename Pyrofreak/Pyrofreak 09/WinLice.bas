Attribute VB_Name = "WinLice"
Option Explicit
''''''''''''''''''''''''''''''''''''''
'            WinLice.Bas             '
'             By: Sozah              '
'                                    '
'This Bas File is the First (Big) Bas'
'File Ever Done by me o_O. This Code '
' Was Made by Sozah, And its Good for'
' The Average program, and Also Good '
'      For Making a Basic Trojan.    '
'                                    '
' © 1999 - [Strukture, Cold Fusionz] '
'     http://www.ColdFusionz.net     '
'  http://www.Cryogen.com/Strukture  '
''''''''''''''''''''''''''''''''''''''
'This Bas File is Easy to use, and has
'All the Usage Examples Before every
'code It was espcially made for the
'Intermediate to Advance programmer.
Declare Function FindWindow Lib "user" (ByVal lpClassName As Any, ByVal lpWindowName As Any) As Integer
Declare Function FindChildByTitle% Lib "vbwfind.dll" (ByVal Parent%, ByVal Title$)
Declare Function SetFocusAPI Lib "user" Alias "SetFocus" (ByVal hWnd As Integer) As Integer
Declare Function SetActiveWindow Lib "user" (ByVal hWnd As Integer) As Integer
Declare Function SendMessage Lib "user" (ByVal hWnd As Integer, ByVal wMsg As Integer, ByVal wParam As Integer, lParam As Any) As Long
Declare Function SendMessageA Lib "user32" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Integer, ByVal lParam As Long) As Long
Declare Function GetWindowRect Lib "user32" (ByVal hWnd As Long, lpRect As RECT) As Long
Declare Function sndPlaySoundA Lib "winmm.dll" (ByVal lpszSoundName$, ByVal ValueFlags As Long) As Long
Declare Function GetClassName& Lib "user32" Alias "GetClassNameA" (ByVal hWnd As Long, ByVal lpClassName As String, ByVal nMaxCount As Long)
Declare Function GetClientRect Lib "user32" (ByVal hWnd As Long, lpRect As RECT) As Long
Declare Function CreateRectRgn Lib "gdi32" (ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long
Declare Function SetWindowRgn Lib "user32" (ByVal hWnd As Long, ByVal hRgn As Long, ByVal bRedraw As Boolean) As Long
Declare Function SendMessageByString& Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As String)
Declare Function ScreenToClient Lib "user32" (ByVal hWnd As Long, lpPoint As POINTAPI) As Long
Declare Function CombineRgn Lib "gdi32" (ByVal hDestRgn As Long, ByVal hSrcRgn1 As Long, ByVal hSrcRgn2 As Long, ByVal nCombineMode As Long) As Long
Declare Function Shell_NotifyIcon Lib "shell32" Alias "Shell_NotifyIconA" (ByVal dwMessage As Long, pnid As NOTIFYICONDATA) As Boolean
Declare Function RegCloseKey Lib "advapi32.dll" (ByVal Hkey As Long) As Long
Declare Function RegCreateKey Lib "advapi32.dll" Alias "RegCreateKeyA" (ByVal Hkey As Long, ByVal lpSubKey As String, phkResult As Long) As Long
Declare Function RegDeleteKey Lib "advapi32.dll" Alias "RegDeleteKeyA" (ByVal Hkey As Long, ByVal lpSubKey As String) As Long
Declare Function RegDeleteValue Lib "advapi32.dll" Alias "RegDeleteValueA" (ByVal Hkey As Long, ByVal lpValueName As String) As Long
Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long
Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Declare Function RegOpenKey Lib "advapi32.dll" Alias "RegOpenKeyA" (ByVal Hkey As Long, ByVal lpSubKey As String, phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32.dll" Alias "RegQueryValueExA" (ByVal Hkey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, lpType As Long, lpData As Any, lpcbData As Long) As Long
Declare Function BitBlt Lib "gdi32" (ByVal hDestDC As Integer, ByVal X As Integer, ByVal Y As Integer, ByVal nWidth As Integer, ByVal nHeight As Integer, ByVal hSrcDC As Integer, ByVal xSrc As Integer, ByVal ySrc As Integer, ByVal dwRop As Long) As Integer
Declare Function GetDC Lib "user32" (ByVal hWnd As Long) As Long
Declare Function GetDesktopWindow Lib "user32" () As Long
Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
Declare Function SetWindowPos Lib "user32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Declare Function SystemParametersInfo Lib "user32" Alias "SystemParametersInfoA" (ByVal uAction As Long, ByVal uParam As Long, ByVal lpvParam As String, ByVal fuWinIni As Long) As Long
Declare Function RegSetValueEx Lib "advapi32.dll" Alias "RegSetValueExA" (ByVal Hkey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long
Private Declare Function GetKeyboardState Lib "user32" (pbKeyState As Byte) As Long
Private Declare Function GetWindow Lib "user32" (ByVal hWnd As Long, ByVal wCmd As Long) As Long
Private Declare Function fCreateShellLink Lib "STKIT432.DLL" (ByVal lpstrFolderName As String, ByVal lpstrLinkName As String, ByVal lpstrLinkPath As String, ByVal lpstrLinkArgs As String) As Long
Private Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpBuffer As String, nSize As Long) As Long
Private Declare Function GetComputerName Lib "kernel32" Alias "GetComputerNameA" (ByVal lpBuffer As String, nSize As Long) As Long
Private Declare Function GetWindowTextLength Lib "user32" Alias "GetWindowTextLengthA" (ByVal hWnd As Long) As Long
Private Declare Function mciSendString Lib "winmm.dll" Alias "mciSendStringA" (ByVal lpstrCommand As String, ByVal lpstrReturnString As String, ByVal uReturnLength As Long, ByVal hwndCallback As Long) As Long
Private Declare Function SetKeyboardState Lib "user32" (lppbKeyState As Byte) As Long
Private Declare Function FindWindowA Lib "user32" (ByVal lpClassName As Any, ByVal lpWindowName As Any) As Integer
Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hWnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function GetWindowThreadProcessId Lib "user32" (ByVal hWnd As Long, lpdwProcessId As Long) As Long
Private Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Function ReleaseDc Lib "user" (ByVal hWnd As Integer, ByVal hDC As Integer) As Integer
Public Declare Function ExitWindowsEx Lib "user32" (ByVal uFlags As Long, ByVal dwReserved As Long) As Long
Public Declare Function lstrcat Lib "kernel32" Alias "lstrcatA" (ByVal lpString1 As String, ByVal lpString2 As String) As Long
Public Declare Function GetWindowDc Lib "user" (ByVal hWnd As Integer) As Integer
Public Declare Function SHBrowseForFolder Lib "shell32" (lpbi As BrowseInfo) As Long
Public Declare Function SHGetPathFromIDList Lib "shell32" (ByVal pidList As Long, ByVal lpBuffer As String) As Long
Public Declare Function SHFileOperation Lib "shell32.dll" Alias "SHFileOperationA" (lpFileOp As SHFILEOPSTRUCT) As Long
Public Declare Function MessageBeep Lib "user32" (ByVal wType As Long) As Long
Public Declare Function SetForegroundWindow Lib "user32" (ByVal hWnd As Long) As Long
Public Declare Sub CoTaskMemFree Lib "ole32.dll" (ByVal hMem As Long)
Public Declare Sub keybd_event Lib "user32" (ByVal bVk As Byte, ByVal bScan As Byte, ByVal dwFlags As Long, ByVal dwExtraInfo As Long)
Declare Sub releaseCapture Lib "user32" Alias "ReleaseCapture" ()
Global Const GFM_BACKSHADOW = 1
Global Const GFM_DROPSHADOW = 2
Global Const EWX_SHUTDOWN = 1
Global Const SND_SYNC = &H0
Global Const SND_ASYNC = &H1
Global Const SND_NODEFAULT = &H2
Global Const SND_LOOP = &H8
Global Const SND_NOSTOP = &H10
Global Const EWX_REBOOT = 2
Global Const VK_SPACE = &H20
Global Const WM_KEYDOWN = &H100
Global Const WM_KEYUP = &H101
Public Const RGN_XOR = 3
Public Const SPI_SCREENSAVERRUNNING = 97
Public Const HKEY_CLASSES_ROOT = &H80000000
Public Const HKEY_CURRENT_USER = &H80000001
Public Const HKEY_LOCAL_MACHINE = &H80000002
Public Const HKEY_USERS = &H80000003
Public Const LB_FINDSTRING = &H18F
Public Const LB_FINDSTRINGEXACT = &H1A2
Public Const HKEY_PERFORMANCE_DATA = &H80000004
Public Const ERROR_SUCCESS = 0&
Public Const REG_SZ = 1
Public Const REG_DWORD = 4
Public Const HTCAPTION = 2
Public Const WM_NCLBUTTONDOWN = &HA1
Public Const FO_COPY = &H2
Public Const FO_DELETE = &H3
Public Const FO_MOVE = &H1
Public Const SRCCOPY = &HCC0020
Public Const SRCAND = &H8800C6
Public Const SRCINVERT = &H660046
Public Const FO_RENAME = &H4
Public Const FOF_ALLOWUNDO = &H40
Public Const WM_LBUTTONDBLCLICK = &H203
Public Const WM_RBUTTONUP = &H205
Public Const WM_MOUSEMOVE = &H200
Public Const NIM_ADD = &H0
Public Const TheScreen = 0
Public Const theform = 1
Public Const NIM_MODIFY = &H1
Public Const NIM_DELETE = &H2
Public Const NIF_MESSAGE = &H1
Public Const NIF_ICON = &H2
Public Const NIF_TIP = &H4
Public Const FOF_CONFIRMMOUSE = &H2
Public Const FOF_FILESONLY = &H80
Public Const FOF_MULTIDESTFILES = &H1
Public Const FOF_NOCONFIRMATION = &H10
Public Const FOF_NOCONFIRMMKDIR = &H200
Public Const FOF_RENAMEONCOLLISION = &H8
Public Const FOF_SILENT = &H4
Public Const FOF_SIMPLEPROGRESS = &H100
Public Const FOF_WANTMAPPINGHANDLE = &H20
Public Const BIF_RETURNONLYFSDIRS = 1
Public Const MAX_PATH = 260
Public Const SW_SHOWNORMAL = 1
Const GW_CHILD = 5
Const WM_DESTROY = &H2
Const WM_CLOSE = &H10
Const GW_HWNDFIRST = 0
Const GW_HWNDLAST = 1
Const GW_HWNDNEXT = 2
Const GW_HWNDPREV = 3
Const GW_MAX = 5
Const GW_OWNER = 4
Global FileDestination As String
Global XMove As Long
Global YMove As Long
Public CIAsystray As NOTIFYICONDATA
Type POINTAPI
X As Long
Y As Long
End Type
Type RECT
Left As Long
Top As Long
Right As Long
Bottom As Long
End Type
Public Type SHFILEOPSTRUCT
hWnd As Long
wFunc As Long
pFrom As String
pTo As String
fFlags As Integer
fAborted As Boolean
hNameMaps As Long
sProgress As String
End Type
Public Type BrowseInfo
hwndOwner As Long
pIDLRoot As Long
pszDisplayName As Long
lpszTitle As Long
ulFlags As Long
lpfnCallback As Long
lParam As Long
iImage As Long
End Type
Public Type NOTIFYICONDATA
cbSize As Long
hWnd As Long
uId As Long
uFlags As Long
ucallbackMessage As Long
hIcon As Long
szTip As String * 64
End Type
Function ExitWindows(BootMode As Integer)
On Error Resume Next
'Usage:
'-------------
'BootModes are as Follows:
'1 = Shutdown Windows
'2 = Reboot Windows
'-------------
'To use this Put this code in a Command
'button or anywhere you want to use it
'-------------
'Exitwindows 1 '- To shutdown windows
'Exitwindows 2 '- To reboot windows
'-------------
Dim bootans As Integer
Dim bootvalue As Long
Select Case BootMode
Case 1 'Shutdown Windows
bootans = MsgBox("Are you sure you want to shutdown windows?", vbQuestion Or vbYesNo, "Shutdown Windows")
If bootans = vbYes Then
bootvalue = ExitWindowsEx(EWX_SHUTDOWN, 0&)
End If
Case 2 ' Reboot Windows
bootans = MsgBox("Are you sure you want to reboot windows?", vbQuestion Or vbYesNo, "Reboot Windows")
If bootans = vbYes Then
bootvalue = ExitWindowsEx(EWX_REBOOT, 0&)
End If
Case Else
End Select
End Function
Function ShellLink(ShellLinkDescription, ShellLinkSource As String, ShellLinkDir As Integer)
On Error Resume Next
'Usage:
'-------------
'Shell Link Directory are as Follows:
'1 = Adds Link to Desktop
'2 = Adds Link to Program Menu
'3 = Adds Link to Startup Group
'4 = Adds Link to The Directory you Specify (Other)
'-------------
'To use this put the Following code
'In a Command button
'-------------
'ShellLink "Shortcut to Calculator","C:\Windows\Calc.exe",1 '- Adds to Desktop
'ShellLink "Shortcut to Calculator","C:\Windows\Calc.exe",2 '- Adds to Program Menu
'ShellLink "Shortcut to Calculator","C:\Windows\Calc.exe",3 '- Adds to Startup Group
'-------------
Dim shellrtrn As Long
Select Case ShellLinkDir
Case 1
shellrtrn = fCreateShellLink("..\..\Desktop", ShellLinkDescription, ShellLinkSource, "")
Case 2
shellrtrn = fCreateShellLink("", ShellLinkDescription, ShellLinkSource, "")
Case 3
shellrtrn = fCreateShellLink("\Startup", ShellLinkDescription, ShellLinkSource, "")
Case Else
End Select
End Function
Sub ProgressBar(pb As Control, PBColor As String, ByVal Percent)
'Usage:
'------------------
'This works great, Just put in the Percentage
'Or make a Program by which you can manipulate
'the Precentange for every byte Downloaded
'or etc...
'------------------
'Colors are as Follows:
'++++++++++++++++++
'vbBlue = Blue
'vbRed = Red
'vbYellow = Yellow
'vbGreen = Green
'vbBlack = Black
'vbWhite = White
'++++++++++++++++++
'vbBlue + vbRed = Purple
'vbBlue + vbGreen = Blue Green (Kind of Like Aqua too)
'vbBlue + vbYellow = Grey
'++++++++++++++++++
'------------
'Progressbar Picture1, vbBlue, 10%
'------------
Dim num$
If Not pb.AutoRedraw Then
pb.AutoRedraw = -1
End If
pb.Cls
pb.ScaleWidth = 100
pb.DrawMode = 10
num$ = Format$(Percent, "###") + "%"
pb.CurrentX = 50 - pb.TextWidth(num$) / 2
pb.CurrentY = (pb.ScaleHeight - pb.TextHeight(num$)) / 2
pb.FontBold = True
pb.Print num$
pb.Line (0, 0)-(Percent, pb.ScaleHeight), PBColor, BF
pb.Refresh
End Sub
Function FormControlShadow(f As Form, c As Control, ShadowEffect As Integer, ShadowWidth As Integer, ShadowColor As Integer)
'Usage:
'-------------
'*Note With the Shadow Color just Put in
'the integer 0
'-------------
'ShadowEffect as Follows:
'-------------
'1 = Top Shadow
'2 = Bottom Shadow
'-------------
'FormControlShadow Me, Picture1, 1, 1, 0
'-------------
Dim shColor As Long
Dim shWidth As Integer
Dim oldWidth As Integer
Dim oldScale As Integer
shWidth = ShadowWidth
shColor = ShadowColor
oldWidth = f.DrawWidth
oldScale = f.ScaleMode
f.ScaleMode = 3
f.DrawWidth = 1
Select Case ShadowEffect
Case GFM_DROPSHADOW
f.Line (c.Left + shWidth, c.Top + shWidth)-Step(c.Width - 1, c.Height - 1), shColor, BF
Case GFM_BACKSHADOW
f.Line (c.Left - shWidth, c.Top - shWidth)-Step(c.Width - 1, c.Height - 1), shColor, BF
End Select
f.DrawWidth = oldWidth
f.ScaleMode = oldScale
End Function
Public Sub MakeFormTransparent(frm As Form)
'Usage:
'------------
'MakeFormTransparent Me
'------------
Dim rctClient As RECT, rctFrame As RECT
Dim hClient As Long, hFrame As Long
GetWindowRect frm.hWnd, rctFrame
GetClientRect frm.hWnd, rctClient
Dim lpTL As POINTAPI, lpBR As POINTAPI
lpTL.X = rctFrame.Left
lpTL.Y = rctFrame.Top
lpBR.X = rctFrame.Right
lpBR.Y = rctFrame.Bottom
ScreenToClient frm.hWnd, lpTL
ScreenToClient frm.hWnd, lpBR
rctFrame.Left = lpTL.X
rctFrame.Top = lpTL.Y
rctFrame.Right = lpBR.X
rctFrame.Bottom = lpBR.Y
rctClient.Left = Abs(rctFrame.Left)
rctClient.Top = Abs(rctFrame.Top)
rctClient.Right = rctClient.Right + Abs(rctFrame.Left)
rctClient.Bottom = rctClient.Bottom + Abs(rctFrame.Top)
rctFrame.Right = rctFrame.Right + Abs(rctFrame.Left)
rctFrame.Bottom = rctFrame.Bottom + Abs(rctFrame.Top)
rctFrame.Top = 0
rctFrame.Left = 0
hClient = CreateRectRgn(rctClient.Left, rctClient.Top, rctClient.Right, rctClient.Bottom)
hFrame = CreateRectRgn(rctFrame.Left, rctFrame.Top, rctFrame.Right, rctFrame.Bottom)
CombineRgn hFrame, hClient, hFrame, RGN_XOR
SetWindowRgn frm.hWnd, hFrame, True
End Sub
Public Function GetRegistryString(Hkey As Long, strPath As String, strValue As String)
'Usage:
'------------
'Call GetRegistryString(HKEY_LOCAL_MACHINE, "Software\Microsoft\Windows\", App.EXEName)
'------------
Dim r
Dim lValueType
Dim keyhand As Long
Dim datatype As Long
Dim lResult As Long
Dim strBuf As String
Dim lDataBufSize As Long
Dim intZeroPos As Integer
r = RegOpenKey(Hkey, strPath, keyhand)
lResult = RegQueryValueEx(keyhand, strValue, 0&, lValueType, ByVal 0&, lDataBufSize)
If lValueType = REG_SZ Then
strBuf = String(lDataBufSize, " ")
lResult = RegQueryValueEx(keyhand, strValue, 0&, 0&, ByVal strBuf, lDataBufSize)
If lResult = ERROR_SUCCESS Then
intZeroPos = InStr(strBuf, Chr$(0))
If intZeroPos > 0 Then
GetRegistryString = Left$(strBuf, intZeroPos - 1)
Else
GetRegistryString = strBuf
End If
End If
End If
End Function
Public Sub SaveRegistryString(Hkey As Long, strPath As String, strValue As String, strdata As String)
'Usage:
'----------
'Call SaveRegistryString(HKEY_LOCAL_MACHINE, "Software\Microsoft\Windows\", App.EXEName, App.EXEName)
'----------
Dim keyhand As Long
Dim r As Long
r = RegCreateKey(Hkey, strPath, keyhand)
r = RegSetValueEx(keyhand, strValue, 0, REG_SZ, ByVal strdata, Len(strdata))
r = RegCloseKey(keyhand)
End Sub
Public Function DeleteRegistryString(ByVal Hkey As Long, ByVal strPath As String, ByVal strValue As String)
'Usage:
'-----------
'Call DeleteRegistryString(HKEY_LOCAL_MACHINE, "Software\Microsoft\Windows", App.EXEName)
'-----------
Dim keyhand As Long
Dim r As Long
r = RegOpenKey(Hkey, strPath, keyhand)
r = RegDeleteValue(keyhand, strValue)
r = RegCloseKey(keyhand)
End Function
Sub GetRunningApplications(frm As Form, lst As ListBox)
'Usage:
'--------------
'Put this in a Command Button
'Let's Say your Listbox's name is List1
'Then Do
'--------------
'GetRunningApplications Me, list1
'---------------
lst.Clear
Dim lLgthChild As Long
Dim sNameChild As String
Dim lLgthOwner As Long
Dim sNameOwner As String
Dim lHwnd As Long
Dim lHwnd2 As Long
Const vbTextCompare = 1
lHwnd = GetWindow(frm.hWnd, GW_HWNDFIRST)
While lHwnd <> 0
lHwnd2 = GetWindow(lHwnd, GW_OWNER)
lLgthOwner = GetWindowTextLength(lHwnd2)
sNameOwner = String$(lLgthOwner + 1, Chr$(0))
lLgthOwner = GetWindowText(lHwnd2, sNameOwner, lLgthOwner + 1)
lLgthChild = GetWindowTextLength(lHwnd)
sNameChild = String$(lLgthChild + 1, Chr$(0))
lLgthChild = GetWindowText(lHwnd, sNameChild, lLgthChild + 1)
If lLgthChild <> 0 Then
sNameChild = Left$(sNameChild, InStr(1, sNameChild, Chr$(0), vbTextCompare) - 1)
sNameChild = Trim(sNameChild)
If ApplicationList(lst, sNameChild, 0) > -1 Then GoTo noadd:
lst.AddItem sNameChild & " - [HWND: " & lHwnd & "]"
noadd:
End If
lHwnd = GetWindow(lHwnd, GW_HWNDNEXT)
DoEvents
Wend
End Sub
Public Function ShellCopy(ParamArray vntFileName() As Variant) As Long
'Usage:
'---------------
'To use this put this in a command button
'lets suppose you want to copy the Windows folder
'So you do:
'---------------
'ShellCopy "C:\Windows"
'---------------
Dim i As Integer
Dim sFileNames As Variant
Dim Dick As String
Dim SHFileOp As SHFILEOPSTRUCT
For i = LBound(vntFileName) To UBound(vntFileName)
sFileNames = sFileNames & vntFileName(i) & vbNullChar
Next
sFileNames = sFileNames & vbNullChar
Dick = FileDestination
With SHFileOp
.wFunc = &H2
.pFrom = sFileNames
.fFlags = FOF_ALLOWUNDO
.pTo = Dick
End With
ShellCopy = SHFileOperation(SHFileOp)
End Function
Public Function ShellMove(ParamArray vntFileName() As Variant) As Long
'Usage:
'---------------
'To use this put this in a command button
'lets suppose you want to move the Windows folder
'So you do:
'---------------
'ShellMove "C:\Windows"
'---------------
Dim i As Integer
Dim sFileNames As Variant
Dim Dick As String
Dim SHFileOp As SHFILEOPSTRUCT
For i = LBound(vntFileName) To UBound(vntFileName)
sFileNames = sFileNames & vntFileName(i) & vbNullChar
Next
sFileNames = sFileNames & vbNullChar
Dick = FileDestination
With SHFileOp
.wFunc = &H1
.pFrom = sFileNames
.fFlags = FOF_ALLOWUNDO
.pTo = Dick
End With
ShellMove = SHFileOperation(SHFileOp)
End Function
Public Function ShellDelete(ParamArray vntFileName() As Variant) As Long
'Usage:
'---------------
'To use this put this in a command button
'lets suppose you want to delete the Windows folder
'So you do:
'---------------
'ShellDelete "C:\Windows"
'---------------
Dim i As Integer
Dim sFileNames As String
Dim SHFileOp As SHFILEOPSTRUCT
For i = LBound(vntFileName) To UBound(vntFileName)
sFileNames = sFileNames & vntFileName(i) & vbNullChar
Next
sFileNames = sFileNames & vbNullChar
With SHFileOp
.wFunc = FO_DELETE
.pFrom = sFileNames
.fFlags = FOF_ALLOWUNDO
End With
ShellDelete = SHFileOperation(SHFileOp)
End Function
Public Function ShellRename(ParamArray vntFileName() As Variant) As Long
'Usage:
'---------------
'To use this put this in a command button
'lets suppose you want to rename the Calculator
'In the Windows Folder, Then do
'So you do:
'---------------
'ShellRename "C:\Windows\Calc.exe"
'---------------
Dim i As Integer
Dim sFileNames As String
Dim Dick As String
Dim SHFileOp As SHFILEOPSTRUCT
For i = LBound(vntFileName) To UBound(vntFileName)
sFileNames = sFileNames & vntFileName(i) & vbNullChar
Next
sFileNames = sFileNames & vbNullChar
Dick = FileDestination
With SHFileOp
.wFunc = &H4
.pFrom = sFileNames
.fFlags = FOF_ALLOWUNDO
.pTo = Dick
End With
ShellRename = SHFileOperation(SHFileOp)
End Function
Function TimedMessageBox(MinutesToWait, Method As Integer, MessageToSend As String)
On Error Resume Next
'Usage:
'------------------
'To use this put the following code in a Timer
'With an Interval of 60000 which is equal to a Minute
'Let's Suppose you want to Send the Message "Sex and Candy"
'Every 5 Minutes, then Do
'------------------
'TimedMessageBox 5, "Sex and Candy"
'------------------
'* Note: If you dont want it to be a Messagebox
'Feel Free to Edit the Area indicated.
Dim Vigit As Byte
Vigit = Vigit + 1
If Vigit = MinutesToWait Then
Select Case Method
Case 0
MsgBox MessageToSend 'You can Edit this to your liking
Vigit = "0"
Case Else
End Select
End If
End Function
Function TilePicture(Mainfrm As Form, PictureToTile As PictureBox)
On Error Resume Next
'Usage:
'Put the Following Code in your Forms'
'Form_Load *Note: You might want to put it
'in Form_Resize Aswell
'Let's Suppose you name the Picturebox
'As Picture1, Then Do
'----------------
'TilePicture Me, Picture1
'----------------
Mainfrm.ScaleMode = 3
Mainfrm.AutoRedraw = True
PictureToTile.ScaleMode = 3
Dim FormHeight As Long
Dim FormWidth As Long
Dim PictureHeight As Long
Dim PictureWidth As Long
Dim X%, Y%
FormHeight = Mainfrm.ScaleHeight
FormWidth = Mainfrm.ScaleWidth
PictureHeight = PictureToTile.ScaleHeight
PictureWidth = PictureToTile.ScaleWidth
For Y = 0 To FormHeight Step PictureHeight
For X = 0 To FormWidth Step PictureWidth
Mainfrm.PaintPicture PictureToTile.Picture, X, Y
Next X
Next Y
PictureToTile.Visible = False
End Function
Function HingeForm(Mainfrm As Form, HingedFrm As Form, Method As Integer)
On Error Resume Next
'Usage:
'---------------------
'There are 8 Methods
'1 = Top
'2 = Top Right
'3 = Right
'4 = Bottom Right
'5 = Bottom
'6 = Bottom Left
'7 = Left
'8 = Top Left
'--------------------
'Let's Suppose the Form you want to Hinge
'is Form2, and you want it at the Bottom Right
'Then Put this Code in a Timer
'With an Interval of "1"
'---------------------
'HingeForm Me, Form2, 4
'---------------------
Dim G As Boolean
HingedFrm.Visible = True
If HingedFrm.Visible = True Then
G = True
Else
G = False
End If
If G = True Then
Select Case Method
Case 1
HingedFrm.Left = Mainfrm.Left
HingedFrm.Top = Mainfrm.Top - HingedFrm.Height
Case 2
HingedFrm.Left = Mainfrm.Left + Mainfrm.Width
HingedFrm.Top = Mainfrm.Top - HingedFrm.Height
Case 3
HingedFrm.Left = Mainfrm.Left + Mainfrm.Width
HingedFrm.Top = Mainfrm.Top
Case 4
HingedFrm.Left = Mainfrm.Left + Mainfrm.Width
HingedFrm.Top = Mainfrm.Top + Mainfrm.Height
Case 5
HingedFrm.Left = Mainfrm.Left
HingedFrm.Top = Mainfrm.Top + Mainfrm.Height
Case 6
HingedFrm.Left = Mainfrm.Left - HingedFrm.Width
HingedFrm.Top = Mainfrm.Top + Mainfrm.Height
Case 7
HingedFrm.Left = Mainfrm.Left - HingedFrm.Width
HingedFrm.Top = Mainfrm.Top
Case 8
HingedFrm.Left = Mainfrm.Left - HingedFrm.Width
HingedFrm.Top = Mainfrm.Top - HingedFrm.Height
Case Else
End Select
ElseIf G = False Then
End If
End Function
Public Sub SystemTrayDeleteIcon(Form As Form)
'Usage:
'------------
'SystemTrayDeleteIcon Me
'------------
Call Shell_NotifyIcon(NIM_DELETE, CIAsystray)
End Sub
Public Function SystemTrayAddIcon(Form As Form)
'Usage:
'------------
'Make sure to add the code listed at the bottom
'To your Forms Mousedown
'------------
'SystemTrayAddIcon Me
'------------
CIAsystray.cbSize = Len(CIAsystray)
CIAsystray.hWnd = Form.hWnd
CIAsystray.uId = vbNull
CIAsystray.uFlags = NIF_ICON Or NIF_TIP Or NIF_MESSAGE
CIAsystray.ucallbackMessage = WM_MOUSEMOVE
CIAsystray.hIcon = Form.Icon
CIAsystray.szTip = Form.Caption & vbNullChar
Call Shell_NotifyIcon(NIM_ADD, CIAsystray)
App.TaskVisible = False
Form.Hide
'----PUT THIS IN YOUR FORM mousemove CODE
'Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
'On Error Resume Next
'Static lngMsg As Long
'Static blnFlag As Boolean
'Dim result As Long
'lngMsg = X / Screen.TwipsPerPixelX
'If blnFlag = False Then
'blnFlag = True
'Select Case lngMsg
'Case WM_LBUTTONDBLCLICK 'Double Click
'Me.Show
'Case WM_RBUTTONUP 'Right Button
'PopupMenu TRAYMNU
'result = SetForegroundWindow(Me.hwnd)
'End Select
'blnFlag = False
'End If
'End Sub
'----------End of Code for Form
End Function
Public Sub SaveINISetting(ByVal sSection As String, ByVal sKey As String, ByVal vntValue As Variant)
'Usage:
'----------
'This Also Works good with the If Statement
'Heres an Example on how to use this with the
'If Statement and a Checkbox.
'----------
'If Check2.Value = 1 Then
'SaveINISetting "Example", "Settings", "True"
'Else
'SaveINISetting "Example", "Settings", "False"
'End If
'----------
Dim SFilename As String
SFilename = App.path & "\Settings.ini"
#If Win32 Then
Dim xRet          As Long
#Else
Dim xRet          As Integer
#End If
xRet = WritePrivateProfileString(sSection, sKey, CStr(vntValue), SFilename)
End Sub
Public Function GetINISetting(ByVal sSection As String, ByVal sKey As String) As Variant
'Usage:
'-----------
'This works great with the if Statement
'Heres an Example on how to use, with the If Statement
'-----------
'If GetINISetting("Example", "Settings") = "True" Then
'Msgbox "True"
'Else
'End If
'-----------
Dim SFilename As String
SFilename = App.path & "\settings.ini"
#If Win32 Then
Dim xRet          As Long
#Else
Dim xRet          As Integer
#End If
Dim sReturnStr    As String
Dim nStringLen    As Integer
nStringLen = 255
sReturnStr = String(nStringLen, Chr$(0))
xRet = GetPrivateProfileString(sSection, sKey, "", sReturnStr, nStringLen, SFilename)
GetINISetting = Left(sReturnStr, xRet)
End Function
Public Sub DisableCAD()
'Usage:
'-------------
'This Code Disables Control + Alt + Delete
'Put this in a Command Button or Anywhere
'you want the Code to be Used.
'-------------
'DisableCAD
'-------------
Dim ret As Integer
Dim pOld As Boolean
ret = SystemParametersInfo(SPI_SCREENSAVERRUNNING, True, pOld, 0)
End Sub
Public Sub EnableCAD()
'Usage:
'-------------
'This Code Enables Control + Alt + Delete
'Put this in a Command Button or Anywhere
'you want the Code to be Used.
'-------------
'EnableCAD
'-------------
Dim ret As Integer
Dim pOld As Boolean
ret = SystemParametersInfo(SPI_SCREENSAVERRUNNING, False, pOld, 0)
End Sub
Function OpenCDDoor()
'Usage:
'-------------
'OpenCDDoor
'-------------
mciSendString "set cd door open", 0, 0, 0
End Function
Function CloseCDDoor()
'Usage:
'-------------
'CloseCDDoor
'-------------
mciSendString "set cd door closed", 0, 0, 0
End Function
Public Function SaveTextbox(TextBoxName As TextBox, TextDir, TextFile As String)
'Usage:
'------------
'Let's Suppose your textbox's name is Text1
'And you want to Save it in "C:\Windows\Desktop"
'Put this in a Command Button and Do:
'------------
'SaveTextbox Text1,"C:\Windows\Desktop","Text1.txt"
'------------
'* Note You can save your Files
'With a Diffrent Extension
On Error Resume Next
TextFile = TextDir & "\" & TextFile
Open (TextFile) For Output As #1
Print #1, TextBoxName
Close #1
End Function
Public Function LoadTextbox(TextFile, TextDir As String, TextBoxName As TextBox)
'Usage:
'------------
'Again, Let's Suppose your Textbox's name
'Is Text1 and the Directory where The
'Text File is Located is "C:\Windows\Desktop\"
'And your text Files name is wok.txt
'Then Do:
'------------
'LoadTextbox "Text1.txt","C:\Windows\Desktop",Text1
'------------
'* Note You can load other File Extensions
On Error Resume Next
TextFile = TextDir & "\" & TextFile
Open TextFile For Input As #1
TextBoxName.Text = Input(LOF(1), #1)
Close #1
End Function
Public Function StayTop(frm As Form, fOnTop As Boolean)
'Usage:
'------------
'StayTop Me, True '- Sets it as The Top Most
'StayTop Me, False '- Doesn't Set it as the Top Most
'------------
Const HWND_TOPMOST = -1
Const HWND_NOTOPMOST = -2
Dim lState As Long
Dim iLeft As Integer, iTop As Integer, iWidth As Integer, iHeight As Integer
With frm
iLeft = .Left / Screen.TwipsPerPixelX
iTop = .Top / Screen.TwipsPerPixelY
iWidth = .Width / Screen.TwipsPerPixelX
iHeight = .Height / Screen.TwipsPerPixelY
End With
If fOnTop Then
lState = HWND_TOPMOST
Else
lState = HWND_NOTOPMOST
End If
Call SetWindowPos(frm.hWnd, lState, iLeft, iTop, iWidth, iHeight, 0)
End Function
Public Function MoveForm(Mainfrm As Form)
'Usage
'-----------
'Put this in a MouseDown Event.
'For example: if you want this for a Label
'you put it in Label_Mousedown
'-----------
'MoveForm Me
'-----------
releaseCapture
Call SendMessageA(Mainfrm.hWnd, &HA1, 2, 0&)
End Function
Function SaveListBox(List As ListBox, ListDir, ListFile As String)
'Usage:
'-----------
'Ok, Let's Suppose your Listbox's name is List1
'And you want to save it as Savelist, in
'"C:\Windows\Desktop", Then Do:
'-----------
'SaveListBox List1, "C:\Windows\Desktop","Savelist.txt"
'-----------
'* Note You can save your Files
'With a Diffrent Extension
Dim SaveList As Long
ListFile = ListDir & "\" & ListFile
Open ListFile For Output As #1
For SaveList& = 0 To List.ListCount - 1
Print #1, List.List(SaveList&)
Next SaveList&
Close #1
End Function
Function PCBeep()
'Usage:
'-----------
'This makes your PC make that little Beep
'or Tick Sound, To use it do:
'-----------
'PCBeep
'-----------
MessageBeep -1&
End Function
Function LoadListBox(ListBox As ListBox, ListDir, File As String)
On Error Resume Next
'Usage:
'-----------
'Let's Suppose your Listbox's name is List1
'And your File's Name is "Savelist.txt",and its
'Located in "C:\Windows\Desktop", Then Do:
'-----------
'LoadListBox List1,"C:\Windows\Desktop","Savelist.txt"
'-----------
'* Note You can load other File Extensions
Dim G$, free%
free = FreeFile
File = ListDir & "\" & File
Open File For Input As #free
Do Until EOF(free)
Line Input #free, G$
ListBox.AddItem G$
Loop
End Function
Function LoadPic(PictureBoxName As PictureBox, PicDirectory As String)
On Error Resume Next
'Usage:
'----------------
'LoadPic picture1,"C:\Windows\Desktop\Blah.jpg"
'----------------
PictureBoxName.Picture = LoadPicture(PicDirectory)
End Function
Public Function DrawGradientForm(frm As Object, Color1 As Long, Color2 As Long)
'Usage:
'---------------
'Colors are as Follows:
'++++++++++++++++++
'vbBlue = Blue
'vbRed = Red
'vbYellow = Yellow
'vbGreen = Green
'vbBlack = Black
'vbWhite = White
'++++++++++++++++++
'vbBlue + vbRed = Purple
'vbBlue + vbGreen = Blue Green (Kind of Like Aqua too)
'vbBlue + vbYellow = Grey
'++++++++++++++++++
'Put the following code in your in a Timer
'With an Interval of 1
'----------------
'DrawGradientForm Me, vbRed, vbBlack
'----------------
'Or if your Using a Combination then do:
'----------------
'DrawGradientForm me, vbBlue + vbRed, vbBlack
'---------------
Timeout 0.5
Dim r1, g1, b1, r2, g2, b2, boxStep, posY, i As Integer
Dim redStep, greenStep, BlueStep As Integer
r1 = Color1 Mod &H100
g1 = (Color1 \ &H100) Mod &H100
b1 = (Color1 \ &H10000) Mod &H100
r2 = Color2 Mod &H100
g2 = (Color2 \ &H100) Mod &H100
b2 = (Color2 \ &H10000) Mod &H100
boxStep = frm.ScaleHeight / 63
posY = 0
If g1 > g2 Then
greenStep = 0
ElseIf g2 > g1 Then
greenStep = 1
Else
greenStep = 2
End If
If r1 > r2 Then
redStep = 0
ElseIf r2 > r1 Then
redStep = 1
Else
redStep = 2
End If
If b1 > b2 Then
BlueStep = 0
ElseIf b2 > b1 Then
BlueStep = 1
Else
BlueStep = 2
End If
For i = 1 To 63
frm.Line (0, posY)-(frm.ScaleWidth, posY + boxStep), RGB(r1, g1, b1), BF
If redStep = 1 Then
r1 = r1 + 4
If r1 > r2 Then
r1 = r2
End If
ElseIf redStep = 0 Then
r1 = r1 - 4
If r1 < r2 Then
r1 = r2
End If
End If
If greenStep = 1 Then
g1 = g1 + 4
If g1 > g2 Then
g1 = g2
End If
ElseIf greenStep = 0 Then
g1 = g1 - 4
If g1 < g2 Then
g1 = g2
End If
End If
If BlueStep = 1 Then
b1 = b1 + 4
If b1 > b2 Then
b1 = b2
End If
ElseIf BlueStep = 0 Then
b1 = b1 - 4
If b1 < b2 Then
b1 = b2
End If
End If
posY = posY + boxStep
Next i
End Function
Sub PCInform()
'Usage:
'------------
'This Gets The Username and Computername
'of a Computer
'------------
'PCInform
'------------
Dim szUser As String * 255
Dim vers As String * 255
Dim lang, lReturn, comp As Long
Dim s, X As Long
lReturn = GetUserName(szUser, 255)
comp = GetComputerName(vers, 1024)
MsgBox "User name is " & szUser, vbOKOnly, ""
MsgBox "Computer's name is " & vers, vbOKOnly, ""
End Sub
Sub Timeout(lngInterval As Long)
'Usage:
'--------------
'This is an Extremly Accurate Timeout Code
'despite the users computer lnginterval is
'same interval as a timer
'--------------
'Timeout 0.5
'--------------
Dim lngEnd As Long, lngNow, count1 As Long
lngEnd = GetTickCount()
lngEnd = count1 + lngInterval
Do
DoEvents
lngNow = GetTickCount()
Loop Until lngNow >= lngEnd
End Sub
Function PrintScreen(PictureBoxName As PictureBox)
'Usage
'---------------
'This simulates the "Alt + PrintScreen"
'and adds it to your Program.
'Let's Suppose your PictureBox's Name is
'Picture1 , Then Do:
'---------------
'Printscreen Picture1
'---------------
'*Note: Theres some Flaw to this
'It gets the Form's PrintScreen.
keybd_event vbKeySnapshot, TheScreen, 0&, 0&
DoEvents
PictureBoxName = Clipboard.GetData(vbCFBitmap)
End Function
Function FloatingControls(Mainfrm As Form)
'Usage:
'---------------
'This Function makes AutoRedraw True
'All you need to Do is Change your
'Borderstyle to 0 (None)
'*Note: Put Images on a Image Control, Not on the Form
'---------------
'FloatingControls me
'---------------
Dim A, B, c, d, Desk&
Mainfrm.AutoRedraw = True
A = Screen.TwipsPerPixelX
B = Screen.TwipsPerPixelY
c = Mainfrm.Top / B
d = Mainfrm.Left / A
Desk& = GetDesktopWindow
BitBlt Mainfrm.hDC, 0, 0, Mainfrm.Width, Mainfrm.Height, GetDC(Desk&), d, c, SRCCOPY
End Function
Public Function FindList(ListBox As ListBox, Text As String, Optional Mode As Byte) As Integer
'Usage:
'------------
'Modes Are as Follows:
'0 = Find String
'1 = Find String Exact
'------------
'Let's Suppose your List's Name is List1
'And you want to Look for the String "Sex"
'Then Do:
'------------
'Findlist List1, "Sex", 0
'------------
Dim found As Integer
If Mode = 0 Then
found = SendMessageByString(ListBox.hWnd, LB_FINDSTRING, -1, Text)
ListBox.ListIndex = found
Else
found = SendMessageByString(ListBox.hWnd, LB_FINDSTRINGEXACT, -1, Text)
ListBox.ListIndex = found
End If
End Function
Function PlayWav(File As String)
Dim SoundName$, ValueFlags%, X%
SoundName$ = File
ValueFlags% = SND_ASYNC Or SND_NODEFAULT
X% = sndPlaySoundA(SoundName$, ValueFlags%)
Exit Function
End Function
Public Function OpenIt(frm As Form, ToOpen As String)
'Usage
'------------
'Use this to Open most things Ex:
'------------
'OpenIt "C:\Windows\Calc.exe" '- Opens File
'OpenIt "http://www.Cryogen.com/Strukture" '- Opens Default Browser
'OpenIt "Mailto:Sozi3@hotmail.com" '- Opens Default Mail Server
'------------
ShellExecute frm.hWnd, "Open", ToOpen, &O0, &O0, SW_SHOWNORMAL
End Function
Sub AOLResetNew(sn As String, pth As String)
'Usage:
'-------------
'This Will Reset AOL to NewUser
'Its leet id you use phish's
'sn = The New Screen Name you Want
'-------------
'AOLResetNew "EliteSozah","C:\AOL30"
'-------------
Screen.MousePointer = 11
Static m0226 As String * 40000
Dim l9E68 As Long
Dim l9E6A As Long
Dim l9E6C As Integer
Dim l9E6E As Integer
Dim l9E70 As Variant
Dim l9E74 As Integer
Dim tru_sn, paath$
If UCase$(Trim$(sn)) = "NEWUSER" Then MsgBox ("AOL is already reset to NewUser!"): Exit Sub
On Error GoTo no_reset
If Len(sn) < 7 Then MsgBox ("The Screen Name will not work unless it is at least 7 characters, including spaces"): Exit Sub
tru_sn = "NewUser" + String$(Len(sn) - 7, " ")
Let paath$ = (pth & "\idb\main.idx")
Open paath$ For Binary As #1
l9E68& = 1
l9E6A& = LOF(1)
While l9E68& < l9E6A&
m0226 = String$(40000, Chr$(0))
Get #1, l9E68&, m0226
While InStr(UCase$(m0226), UCase$(sn)) <> 0
Mid$(m0226, InStr(UCase$(m0226), UCase$(sn))) = tru_sn
Wend
Put #1, l9E68&, m0226
l9E68& = l9E68& + 40000
Wend
Seek #1, Len(sn)
l9E68& = Len(sn)
While l9E68& < l9E6A&
m0226 = String$(40000, Chr$(0))
Get #1, l9E68&, m0226
While InStr(UCase$(m0226), UCase$(sn)) <> 0
Mid$(m0226, InStr(UCase$(m0226), UCase$(sn))) = tru_sn
Wend
Put #1, l9E68&, m0226
l9E68& = l9E68& + 40000
Wend
Close #1
Screen.MousePointer = 0
no_reset:
Screen.MousePointer = 0
Exit Sub
Resume Next
End Sub
Sub SetCapslock(Value As Boolean)
'Usage:
'-------------
'SetCapsLock True '- Turns Caps Lock On
'SetCapsLick False '-Turns Caps Lock Off
'-------------
Call SetKeyState(vbKeyCapital, Value)
End Sub
Sub SetNumlock(Value As Boolean)
'Usage:
'-------------
'SetNumLock True '- Turns Num Lock On
'SetNumLock False '-Turns Num Lock Off
'-------------
Call SetKeyState(vbKeyNumlock, Value)
End Sub
Private Sub SetKeyState(intKey As Integer, fTurnOn As Boolean)
'Usage:
'--------------
'This sets the Keys for the
'SetCapsLock and the SetNumLock
'Options, If you want to use it
'in another way, you can
'--------------
Dim abytBuffer(0 To 255) As Byte
GetKeyboardState abytBuffer(0)
abytBuffer(intKey) = CByte(Abs(fTurnOn))
SetKeyboardState abytBuffer(0)
End Sub
Function ThreeDText(frm As Form, Top As Integer, Left As Integer, Depth As Integer, ForeColor, FontName, FontSize, Text As String)
'Usage:
'--------------
'Colors are as Follows:
'++++++++++++++++++
'vbBlue = Blue
'vbRed = Red
'vbYellow = Yellow
'vbGreen = Green
'vbBlack = Black
'vbWhite = White
'++++++++++++++++++
'vbBlue + vbRed = Purple
'vbBlue + vbGreen = Blue Green (Kind of Like Aqua too)
'vbBlue + vbYellow = Grey
'++++++++++++++++++
'--------------
'The Following Code will add the 3D text
'You can adjust it accordingly
'--------------
'ThreeDText Me, 5, 5, 2 , vbWhite, "Arial", 24 , "3D Text Example"
'--------------
Dim Times As Integer
frm.ScaleMode = 3
frm.FontName = FontName
frm.FontSize = FontSize
frm.ForeColor = &HA0A0A0
For Times = 0 To Depth
frm.CurrentY = Top + Times
frm.CurrentX = Left + Times
If Times = Depth Then
frm.ForeColor = ForeColor
Else
End If
frm.Print Text
Next Times
End Function
Function LabelCaptionScroll(Lbl As Label)
On Error GoTo k
'Usage:
'--------------
'You can put this in Form_Load
'If you want. Let's Suppose your
'Lable's name is Label1 Then Do:
'--------------
'LabelCaptionScroll Label1
'--------------
Dim X As Integer
Dim Current As Variant
Dim Y As String
Y = Lbl.Caption
Lbl.Caption = ""
For X = 0 To Len(Y)
If X = 0 Then
Lbl.Caption = ""
Current = Timer
Do While Timer - Current < 0.1
DoEvents
Loop
GoTo done
Else: End If
Lbl.Caption = Left(Y, X)
Current = Timer
Do While Timer - Current < 0.05
DoEvents
Loop
done:
Next X
k:
Exit Function
End Function
Sub UnloadAll()
On Error GoTo Y
'Usage
'------------
'This unloads all the Forms in your
'Project. This is Better then the
'End Command (When Unloading Forms)
'------------
'UnloadAll
'------------
Dim frm As Form
For Each frm In Forms
Unload frm
Next frm
Y:
Exit Sub
End Sub
Function TextBoxScroll(TextBoxName As TextBox)
'Usage:
'------------
'Put this in your Text_Change
'This will make the Text's Scrollbar
'Stay at the bottom when its Changed
'Much like mIRC's Channel Window
'------------
'TextBoxScroll Text1
'------------
TextBoxName.SelStart = Len(TextBoxName.Text)
End Function
Function KillWindow(Optional lst As ListBox, Optional Text As String)
On Error GoTo j
'Usage:
'------------
'This Works Great with the GetRunningApplications Function
'It lets you use it in 2 Different ways
'One of them is for a Listbox, and the Other
'Is for your input, Ex:
'------------
'KillWindow List1 '- For the Listbox
'KillWindow , "Untitled - Notepad" '- Kills whatever you Input
'------------
Dim hWnd&
Dim Res&
Dim Lis As Boolean
Dim tex As Boolean
If lst Is Nothing Then
Lis = False
tex = True
ElseIf Text = "" Then
tex = False
Lis = True
End If
If Lis = True Then
hWnd = FindWindowA(vbNullString, lst.Text)
KillClass (hWnd)
End If
If tex = True Then
hWnd = FindWindowA(vbNullString, Text)
KillClass (hWnd)
End If
j:
Exit Function
End Function
Private Function KillClass(hWnd&)
'Usage:
'------------
'This Private Function was Made
'For the KillWindow Function
'This is just to set the hWnd&
'-------------
Dim Res&
Res = SendMessageA(hWnd, WM_CLOSE, 0, 0)
Res = SendMessageA(hWnd, WM_DESTROY, 0, 0)
End Function
Function FormUnloadDownUp(frm As Form)
On Error GoTo m
'Usage:
'------------
'This Bounces the Form Down to Up
'Then Unloads it.
'Put This in the Unload Button
'Or in Form_Unload
'------------
'FormUnloadDownUp Me
'------------
Do
DoEvents
frm.Top = frm.Top + 1
Loop Until frm.Top = Screen.Height - frm.Height
Do
DoEvents
frm.Top = frm.Top - 1
Loop Until frm.Top = 0
UnloadAll
m:
Exit Function
End Function
Function FormUnloadRightLeft(frm As Form)
On Error GoTo u
'Usage:
'--------------
'This Bounces the Form From left to Right
'then Unloads.
'Put this in the unload button or
'In Form_Unload
'--------------
'FormUnloadRightLeft Me
'--------------
Do
DoEvents
frm.Left = frm.Left + 1
Loop Until frm.Left = Screen.Width - frm.Width
Do
DoEvents
frm.Left = frm.Left - 1
Loop Until frm.Left = 0
UnloadAll
u:
Exit Function
End Function
Function FormUnloadTVClose(frm As Form)
On Error GoTo l
'Usage:
'----------------
'Closes Form like a TV (notice the word "LIKE")
'Put This in the Unload Button
'Or in Form_Unload
'----------------
'FormUnloadTVClose Me
'----------------
Dim GoInto
Dim GotoVal
GotoVal = frm.Height / 2
For GoInto = 1 To GotoVal
DoEvents
frm.Height = frm.Height - 10
frm.Top = (Screen.Height - frm.Height) \ 2
If frm.Height <= 11 Then GoTo horiz
Next GoInto
horiz:
frm.Height = 30
GotoVal = frm.Width / 2
For GoInto = 1 To GotoVal
DoEvents
frm.Width = frm.Width - 10
frm.Left = (Screen.Width - frm.Width) \ 2
Next GoInto
UnloadAll
l:
Exit Function
End Function
Function FreeProcess()
'Usage:
'-----------
'Unfreezes a locked Loop
'Or subroutine
'-----------
'FreeProcess
'-----------
Dim Process%
Do: DoEvents
Process = Process + 1
If Process = 50 Then Exit Do
Loop
End Function
Private Function WinampFormMove()
'Usage:
'----------
'This is Just Information on how to
'Make the Form like Winamp moves its Window
'----------
'This Works with Either a  Label, Picturebox
'And or an Image.
'In This case we are Going to Use a Label as
'An example, The Code will be the Same for the
'Image, and the Picturebox
'----------Code Begin-------------
'Private Sub Label_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
'XMove = X
'YMove = Y
'End Sub
'----------Code End ---------------
'----------Code Begin--------------
'Private Sub Label_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
'If Button = 1 Then
'Me.Left = Me.Left - (XMove - X)
'Me.Top = Me.Top - (YMove - Y)
'End If
'End Sub
'----------Code End ---------------
'*Note: XMove and YMove are already
'Set To use, Because the Bas has already
'Defined what each is
End Function
Public Function ApplicationList(ListBox As ListBox, Text As String, Optional Mode As Byte) As Integer
'Usage:
'---------------
'This is For the GetRunningApplication
'Function, If you want to use it for
'Something else go ahead
'---------------
Dim found As Integer
If Mode = 0 Then
found = SendMessageByString(ListBox.hWnd, LB_FINDSTRING, -1, Text)
Else
found = SendMessageByString(ListBox.hWnd, LB_FINDSTRINGEXACT, -1, Text)
End If
ApplicationList = found
End Function
