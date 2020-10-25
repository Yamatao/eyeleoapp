# Eyeleo for Windows
<a href="https://github.com/yamatao/eyeleoapp/graphs/contributors" alt="license"><img src="https://img.shields.io/github/contributors/yamatao/eyeleoapp" /></a>
<a href="https://twitter.com/intent/follow?screen_name=eyeleoapp" alt="follow on twitter"><img src="https://img.shields.io/twitter/follow/eyeleoapp?style=social&logo=twitter" /></a>

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
```
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
set WXWIN=c:\Dev\wxWidgets-3.1.3\
cmake -Bwindowsbuild .
cmake --build windowsbuild --config Release
```

## Support
Support the development by becoming a patron at <a href="https://shieldsio-patreon.vercel.app/api/?username=eyeleo&type=patrons"><img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Deyeleo%26type%3Dpatrons&style=for-the-badge" /></a>
