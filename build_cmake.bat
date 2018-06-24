'cmake -E remove_directory build
'cmake -E make_directory build
cd build
cmake .. -G "Visual Studio 12 2013"
cmake --build . --config Release
pause