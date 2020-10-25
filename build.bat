call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
echo on

set WXWIN=c:\Dev\wxWidgets-3.1.3\
set BUILD_DIR=build-windows

cmake -B%BUILD_DIR% .
cmake --build %BUILD_DIR% --config Release

rmdir /S /Q %BUILD_DIR%