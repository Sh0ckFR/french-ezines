@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by APPLI.HPJ. >"hlp\appli.hm"
echo. >>"hlp\appli.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\appli.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\appli.hm"
echo. >>"hlp\appli.hm"
echo // Prompts (IDP_*) >>"hlp\appli.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\appli.hm"
echo. >>"hlp\appli.hm"
echo // Resources (IDR_*) >>"hlp\appli.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\appli.hm"
echo. >>"hlp\appli.hm"
echo // Dialogs (IDD_*) >>"hlp\appli.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\appli.hm"
echo. >>"hlp\appli.hm"
echo // Frame Controls (IDW_*) >>"hlp\appli.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\appli.hm"
REM -- Make help for Project APPLI


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\appli.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\appli.hlp" goto :Error
if not exist "hlp\appli.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\appli.hlp" Debug
if exist Debug\nul copy "hlp\appli.cnt" Debug
if exist Release\nul copy "hlp\appli.hlp" Release
if exist Release\nul copy "hlp\appli.cnt" Release
echo.
goto :done

:Error
echo hlp\appli.hpj(1) : error: Problem encountered creating help file

:done
echo.
