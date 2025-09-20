:: app specific variables - adjust these to match your system and the script should do the rest
set app_name=QuNeo Editor
set app_version=2.0.3
set package_dir=%app_name% WIN %app_version%
set output_dir=%package_dir%/Editor
set content_source_dir=..\Content
set content_output_dir=%package_dir%/Content
set build_dir=..\build-QuNeo-Desktop_Qt_6_2_4_MSVC2019_64bit-Release\release
set quneo_binary="%build_dir%\QuNeo Editor.exe"
set quneo_binary_console="%build_dir%\QuNeo Editor (Debug Console).exe"
set quneo_output_binary="%output_dir%\QuNeo Editor.exe"
set quneo_console_output_binary="%output_dir%\QuNeo Editor (Debug Console).exe"
set path_to_qtwindeploy="C:\Qt\6.2.4\msvc2019_64\bin\windeployqt.exe"
set path_to_qml_files="..\quneo-qt6-editor\QT6-QuNeo"
set path_to_vcvarsall="c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
set path_to_signtool="C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64\signtool.exe"
set ssl_dir="./ssl"
set "current_dir=%cd%"
:: end variables



@echo off

cls
@echo Please pause dropbox sync before continuing (avoids zip errors)
pause

echo Setting up environment for Qt usage...
set PATH=C:\Qt\6.2.4\msvc2019_64\bin;%PATH%
cd /D C:\Qt\6.2.4\msvc2019_64
call %path_to_vcvarsall% x86_amd64

@echo on

cd %current_dir%

mkdir "%package_dir%"
mkdir "%output_dir%"
mkdir "%content_output_dir%"

xcopy /S /Y "%content_source_dir%" "%content_output_dir%"
xcopy /S /Y .\presets "%output_dir%\Resources\presets\"

%path_to_qtwindeploy% --dir "%output_dir%" %quneo_binary%

xcopy /S /Y %quneo_binary% "%output_dir%"
xcopy /S /Y %quneo_binary_console% "%output_dir%"
xcopy /S /Y %ssl_dir%\*.* "%output_dir%"

%path_to_signtool% sign /debug /a /tr http://timestamp.globalsign.com/tsa/advanced /td SHA256 /fd certHash %quneo_output_binary%
%path_to_signtool% sign /debug /a /tr http://timestamp.globalsign.com/tsa/advanced /td SHA256 /fd certHash %quneo_console_output_binary%

echo "Wait 5 seconds for dropbox to catch up"

TIMEOUT 5

powershell compress-archive "'%package_dir%' '%package_dir%.zip'" 

rmdir /S/Q "%package_dir%"

pause





