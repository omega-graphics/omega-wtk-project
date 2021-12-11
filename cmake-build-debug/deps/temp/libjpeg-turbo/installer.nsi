!include x64.nsh
Name "libjpeg-turbo SDK for Visual C++ 64-bit"
OutFile "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}libjpeg-turbo-2.0.7-vc64.exe"
InstallDir "C:\Users\alex\Documents\GitHub\wtk\cmake-build-debug\deps\libjpeg-turbo"

SetCompressor bzip2

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "libjpeg-turbo SDK for Visual C++ 64-bit (required)"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif
	SectionIn RO
!ifdef GCC
	IfFileExists $SYSDIR/libturbojpeg.dll exists 0
!else
	IfFileExists $SYSDIR/turbojpeg.dll exists 0
!endif
	goto notexists
	exists:
!ifdef GCC
	MessageBox MB_OK "An existing version of the libjpeg-turbo SDK for Visual C++ 64-bit is already installed.  Please uninstall it first."
!else
	MessageBox MB_OK "An existing version of the libjpeg-turbo SDK for Visual C++ 64-bit or the TurboJPEG SDK is already installed.  Please uninstall it first."
!endif
	quit

	notexists:
	SetOutPath $SYSDIR
!ifdef GCC
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libturbojpeg.dll"
!else
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}turbojpeg.dll"
!endif
	SetOutPath $INSTDIR\bin
!ifdef GCC
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libturbojpeg.dll"
!else
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}turbojpeg.dll"
!endif
!ifdef GCC
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libjpeg-62.dll"
!else
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}jpeg62.dll"
!endif
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}cjpeg.exe"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}djpeg.exe"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}jpegtran.exe"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}tjbench.exe"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}rdjpgcom.exe"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}wrjpgcom.exe"
	SetOutPath $INSTDIR\lib
!ifdef GCC
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libturbojpeg.dll.a"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libturbojpeg.a"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libjpeg.dll.a"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\libjpeg.a"
!else
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}turbojpeg.lib"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}turbojpeg-static.lib"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}jpeg.lib"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\${BUILDDIR}jpeg-static.lib"
!endif
	SetOutPath $INSTDIR\lib\pkgconfig
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\pkgscripts\libjpeg.pc"
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\pkgscripts\libturbojpeg.pc"
!ifdef JAVA
	SetOutPath $INSTDIR\classes
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\java\turbojpeg.jar"
!endif
	SetOutPath $INSTDIR\include
	File "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo\jconfig.h"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\jerror.h"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\jmorecfg.h"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\jpeglib.h"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\turbojpeg.h"
	SetOutPath $INSTDIR\doc
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\README.ijg"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\README.md"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\LICENSE.md"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\example.txt"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\libjpeg.txt"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\structure.txt"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\usage.txt"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\wizard.txt"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\tjexample.c"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\java\TJExample.java"
!ifdef GCC
	SetOutPath $INSTDIR\man\man1
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\cjpeg.1"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\djpeg.1"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\jpegtran.1"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\rdjpgcom.1"
	File "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code\wrjpgcom.1"
!endif

	WriteRegStr HKLM "SOFTWARE\libjpeg-turbo64 2.0.7" "Install_Dir" "$INSTDIR"

	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo64 2.0.7" "DisplayName" "libjpeg-turbo SDK v2.0.7 for Visual C++ 64-bit"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo64 2.0.7" "UninstallString" '"$INSTDIR\uninstall_2.0.7.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo64 2.0.7" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo64 2.0.7" "NoRepair" 1
	WriteUninstaller "uninstall_2.0.7.exe"
SectionEnd

Section "Uninstall"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif

	SetShellVarContext all

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo64 2.0.7"
	DeleteRegKey HKLM "SOFTWARE\libjpeg-turbo64 2.0.7"

!ifdef GCC
	Delete $INSTDIR\bin\libjpeg-62.dll
	Delete $INSTDIR\bin\libturbojpeg.dll
	Delete $SYSDIR\libturbojpeg.dll
	Delete $INSTDIR\lib\libturbojpeg.dll.a
	Delete $INSTDIR\lib\libturbojpeg.a
	Delete $INSTDIR\lib\libjpeg.dll.a
	Delete $INSTDIR\lib\libjpeg.a
!else
	Delete $INSTDIR\bin\jpeg62.dll
	Delete $INSTDIR\bin\turbojpeg.dll
	Delete $SYSDIR\turbojpeg.dll
	Delete $INSTDIR\lib\jpeg.lib
	Delete $INSTDIR\lib\jpeg-static.lib
	Delete $INSTDIR\lib\turbojpeg.lib
	Delete $INSTDIR\lib\turbojpeg-static.lib
!endif
	Delete $INSTDIR\lib\pkgconfig\libjpeg.pc
	Delete $INSTDIR\lib\pkgconfig\libturbojpeg.pc
!ifdef JAVA
	Delete $INSTDIR\classes\turbojpeg.jar
!endif
	Delete $INSTDIR\bin\cjpeg.exe
	Delete $INSTDIR\bin\djpeg.exe
	Delete $INSTDIR\bin\jpegtran.exe
	Delete $INSTDIR\bin\tjbench.exe
	Delete $INSTDIR\bin\rdjpgcom.exe
	Delete $INSTDIR\bin\wrjpgcom.exe
	Delete $INSTDIR\include\jconfig.h
	Delete $INSTDIR\include\jerror.h
	Delete $INSTDIR\include\jmorecfg.h
	Delete $INSTDIR\include\jpeglib.h
	Delete $INSTDIR\include\turbojpeg.h
	Delete $INSTDIR\uninstall_2.0.7.exe
	Delete $INSTDIR\doc\README.ijg
	Delete $INSTDIR\doc\README.md
	Delete $INSTDIR\doc\LICENSE.md
	Delete $INSTDIR\doc\example.txt
	Delete $INSTDIR\doc\libjpeg.txt
	Delete $INSTDIR\doc\structure.txt
	Delete $INSTDIR\doc\usage.txt
	Delete $INSTDIR\doc\wizard.txt
	Delete $INSTDIR\doc\tjexample.c
	Delete $INSTDIR\doc\TJExample.java
!ifdef GCC
	Delete $INSTDIR\man\man1\cjpeg.1
	Delete $INSTDIR\man\man1\djpeg.1
	Delete $INSTDIR\man\man1\jpegtran.1
	Delete $INSTDIR\man\man1\rdjpgcom.1
	Delete $INSTDIR\man\man1\wrjpgcom.1
!endif

	RMDir "$INSTDIR\include"
	RMDir "$INSTDIR\lib\pkgconfig"
	RMDir "$INSTDIR\lib"
	RMDir "$INSTDIR\doc"
!ifdef GCC
	RMDir "$INSTDIR\man\man1"
	RMDir "$INSTDIR\man"
!endif
!ifdef JAVA
	RMDir "$INSTDIR\classes"
!endif
	RMDir "$INSTDIR\bin"
	RMDir "$INSTDIR"

SectionEnd
