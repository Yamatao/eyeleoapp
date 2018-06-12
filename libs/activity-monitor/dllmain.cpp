// dllmain.cpp: определяет точку входа для приложения DLL.
#include "stdafx.h"
#include "setup.h"
#include "windows.h"

typedef void (*EventCallback)(void);

EventCallback mouseCb = 0;
EventCallback keyCb = 0;

extern "C" AM_API LRESULT WINAPI MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(NULL, nCode, wParam, lParam); 
	
	if (mouseCb)
		mouseCb();
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" AM_API LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (keyCb)
		keyCb();

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

extern "C" AM_API void SetupMouseCallback(EventCallback cb)
{
	mouseCb = cb;
}

extern "C" AM_API void SetupKeyCallback(EventCallback cb)
{
	keyCb = cb;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

