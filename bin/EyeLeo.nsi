!include "MUI2.nsh"
ShowInstDetails show

!define APPNAME "EyeLeo"
!define VERSION "1.33"

; The name of the installer
Name "${APPNAME} Installer"

; The file to write
OutFile "${APPNAME}_Installer_${VERSION}.exe"

; The default installation directory
InstallDir $PROGRAMFILES\${APPNAME}

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${APPNAME}" "Install_Dir"

; Request application privileges
RequestExecutionLevel admin

;--------------------------------

; Pages

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_CHECKED
!define MUI_FINISHPAGE_RUN_TEXT "Launch EyeLeo immediately"
!define MUI_FINISHPAGE_RUN_FUNCTION "LaunchApp"
!define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
;!insertmacro MUI_UNPAGE_FINISH


!insertmacro MUI_LANGUAGE "English"

;--------------------------------

; The stuff to install
Section "Install EyeLeo (required)"

  SectionIn RO
  
  ; Install for all users
  SetShellVarContext all
  
  ; SetDetailsPrint textonly
  DetailPrint "Installing ${APPNAME} files..."
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\Langpacks
  File "Langpacks\langpack.en.xml"
  
  SetOutPath $INSTDIR\Personages\leopard
  File "Personages\leopard\leopard_blink.png"
  File "Personages\leopard\leopard_close_tightly.png"
  File "Personages\leopard\leopard_default.png"
  File "Personages\leopard\leopard_look_down.png"
  File "Personages\leopard\leopard_look_left.png"
  File "Personages\leopard\leopard_look_right.png"
  File "Personages\leopard\leopard_look_up.png"
  
  SetOutPath $INSTDIR\Resources
  File "Resources\icon.ico"
  File "Resources\icongray.ico"
  File "Resources\alarm-clock.png"
  File "Resources\alarm-clock-blue.png"
  File "Resources\balloon.png"
  File "Resources\bell.png"
  File "Resources\can-close-notifs.png"
  File "Resources\control.png"
  File "Resources\control-pause.png"
  File "Resources\cross-button.png"
  File "Resources\eyeleo_title.png"
  File "Resources\icon_settings.png"
  File "Resources\info_16.png"
  File "Resources\info_32.png"
  File "Resources\minipause_window.png"
  File "Resources\notification_leopard.png"
  File "Resources\skin2.png"
  File "Resources\skin3.png"
  File "Resources\skin4.png"
  File "Resources\users.png"
  File "Resources\window.png"
  File "Resources\wrench-screwdriver.png"
  File "Resources\information.ico"
  File "Resources\long_break.ico"
  File "Resources\notification.ico"
  File "Resources\pause.ico"
  File "Resources\resume.ico"
  File "Resources\settings.ico"
  File "Resources\short_break.ico"
  File "Resources\sound.ico"
  File "Resources\strict_mode.ico"

  SetOutPath $INSTDIR
  File "activity-monitor.dll"
  File "msvcp120.dll"
  File "msvcr120.dll"
  File "EyeLeo.exe"
  File "readme.txt"
  File "license.txt"
  File "/oname=config.xml" "config.en.xml"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\${APPNAME} "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
SectionEnd

Section "Start Menu Shortcuts"
  SetShellVarContext all ;for all users
  CreateDirectory "$SMPROGRAMS\${APPNAME}"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\EyeLeo.exe" "$INSTDIR\icon.ico" "$INSTDIR\EyeLeo.exe" 0
SectionEnd

Section "Add EyeLeo to Startup"
  SetShellVarContext all ;for all users
  CreateShortCut "$SMSTARTUP\${APPNAME}.lnk" "$INSTDIR\EyeLeo.exe" "$INSTDIR\icon.ico" "$INSTDIR\EyeLeo.exe" 2 SW_SHOWNORMAL ALT|CTRL|SHIFT|F5 "Launch EyeLeo and start reducing your eye strain."
SectionEnd


Function LaunchApp
  ExecShell "" "$INSTDIR\EyeLeo.exe"
FunctionEnd
;--------------------------------

; Uninstaller

Section "Uninstall"
  ; Close active instance
  FindProcDLL::FindProc "EyeLeo.exe"
  IntCmp $R0 1 0 notRunning
      MessageBox MB_OK|MB_ICONEXCLAMATION "EyeLeo is running. Please close the application before uninstalling." /SD IDOK
      Abort
  notRunning:
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
  DeleteRegKey HKLM SOFTWARE\${APPNAME}
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\${APPNAME}\*.*"
  Delete "$SMSTARTUP\${APPNAME}.lnk"

  ; Remove directories used
  RMDir /r "$SMPROGRAMS\${APPNAME}"
  
  Delete "$INSTDIR\*.*"
  Delete "$INSTDIR\Resources\*.*"
  Delete "$INSTDIR\Langpacks\*.*"
  Delete "$INSTDIR\Personages\leopard\*.*"
  RMDir "$INSTDIR\Langpacks"
  RMDir "$INSTDIR\Resources"
  RMDir "$INSTDIR\Personages\leopard\"
  RMDir "$INSTDIR\Personages\"
  RMDir "$INSTDIR"
SectionEnd

