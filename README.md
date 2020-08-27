# Eyeleo for Windows

## License
The source code is published under CC BY-NC 4.0 (https://creativecommons.org/licenses/by-nc/4.0/deed.en)
You are free to use and modify the source code, but don't use it for commercial purposes.
Merge requests are welcome.

## Supported systems
* Windows 7, 8, 10

## Third-party libraries
* wxWidgets 3.x
* PugiXml (included as submodule)

## Build instructions
1) Update git submodules
2) Download wxWidgets latest version (tested on 3.1.3)
3) Set up and build wxWidgets (Release, x64) following the instructions in `docs/msw`. Set up WXWIN environment variable to the directory with wxWidgets (for example: `c:\Dev\wxWidgets-3.1.3\`).
4) Use CMake to generate a Visual Studio solution and build EyeLeo for x86_64. Example script for the Visual Studio 2019:
````
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set WXWIN=c:\Dev\wxWidgets-3.1.3\
cmake -Bwindowsbuild .
cmake --build windowsbuild --config Release
```

## Support
You may support the development by becoming a patron at https://www.patreon.com/eyeleo
