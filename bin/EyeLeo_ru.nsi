!include "MUI2.nsh"
ShowInstDetails show

!define APPNAME "EyeLeo"
!define VERSION "1.3"

; The name of the installer
Name "${APPNAME} Installer"

; The file to write
OutFile "${APPNAME}_Installer_${VERSION}_ru.exe"

; The default installation directory
InstallDir $PROGRAMFILES\${APPNAME}

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\${APPNAME}" "Install_Dir"

; Request application privileges for Windows Vista
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
  
  ;SetDetailsPrint textonly
  DetailPrint "Installing ${APPNAME} files..."
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\Langpacks
  File "Langpacks\langpack.ru.xml"
  File "Langpacks\langpack.en.xml"
  
  SetOutPath $INSTDIR\Personages\leopard
  File "Personages\leopard\leopard_blink.png"
  File "Personages\leopard\leopard_close_tightly.png"
  File "Personages\leopard\leopard_default.png"
  File "Personages\leopard\leopard_look_down.png"
  File "Personages\leopard\leopard_look_left.png"
  File "Personages\leopard\leopard_look_right.png"
  File "Personages\leopard\leopard_look_up.png"
  
  SetOutPath $INSTDIR
  File "ActivityMonitor.dll"
  File "msvcp90.dll"
  File "msvcr90.dll"
  File "EyeLeo.exe"
  File "icon.ico"
  File "icongray.ico"
  File "EyeLeo.exe.manifest"
  File "Microsoft.VC90.CRT.manifest"
  
  File "alarm-clock.png"
  File "alarm-clock-blue.png"
  File "balloon.png"
  File "bell.png"
  File "can-close-notifs.png"
  File "control.png"
  File "control-pause.png"
  File "cross-button.png"
  File "eyeleo_title.png"
  File "icon_settings.png"
  File "info_16.png"
  File "info_32.png"
  File "minipause_window.png"
  File "notification_leopard.png"
  File "skin2.png"
  File "skin3.png"
  File "skin4.png"
  File "users.png"
  File "window.png"
  File "wrench-screwdriver.png"
  File "readme.txt"
  File "license.txt"
  File "config.xml"
  
  File "information.ico"
  File "long_break.ico"
  File "notification.ico"
  File "pause.ico"
  File "resume.ico"
  File "settings.ico"
  File "short_break.ico"
  File "sound.ico"
  File "strict_mode.ico"
  
  
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
  CreateDirectory "$SMPROGRAMS\${APPNAME}"
  CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\EyeLeo.exe" "$INSTDIR\icon.ico" "$INSTDIR\EyeLeo.exe" 0
SectionEnd

Section "Add EyeLeo to Startup"
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
  Delete "$INSTDIR\Langpacks\*.*"
  Delete "$INSTDIR\Personages\leopard\*.*"
  RMDir "$INSTDIR\Langpacks"
  RMDir "$INSTDIR\Personages\leopard\"
  RMDir "$INSTDIR\Personages\"
  RMDir "$INSTDIR"
SectionEnd

