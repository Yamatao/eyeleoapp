#ifndef WINVER
#define WINVER 0x0600 // Build minimum is Vista 
#endif

#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <windows.h>

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

typedef void (*EventCallback)(void);

EventCallback mouseCb = 0;
EventCallback keyCb = 0;

extern "C" DLL_API LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	
	if (mouseCb)
		mouseCb();
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" DLL_API LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (keyCb)
		keyCb();

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" DLL_API void SetupMouseCallback(EventCallback cb)
{
	mouseCb = cb;
}

extern "C" DLL_API void SetupKeyCallback(EventCallback cb)
{
	keyCb = cb;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD /*ul_reason_for_call*/, LPVOID /*lpReserved*/)
{
	return TRUE;
}

