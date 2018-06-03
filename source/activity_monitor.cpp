#include "activity_monitor.h"
#include "main.h"

HHOOK hMouseHook = 0;
HHOOK hKeyHook = 0;

typedef void (*EventCallback)(void);
typedef void (*SetupCallback)(EventCallback cb);

HINSTANCE hDll = 0;
SetupCallback SetupMouseCallback = 0;
SetupCallback SetupKeyCallback = 0;
HOOKPROC hookMouseProc = 0;
HOOKPROC hookKeyProc = 0;

void ActivityMonitor_MouseCallback()
{
	//getApp()->OnUserActivity();
}

void ActivityMonitor_KeyCallback()
{
	getApp()->OnUserActivity();
}

void PrepareActivityMonitor()
{
	hDll = LoadLibrary(L"ActivityMonitor.dll");
	//SetupMouseCallback = (SetupCallback)GetProcAddress(hDll, "SetupMouseCallback");
	SetupKeyCallback = (SetupCallback)GetProcAddress(hDll, "SetupKeyCallback");
	//hookMouseProc = (HOOKPROC)GetProcAddress(hDll, "_MouseProc@12");
	hookKeyProc = (HOOKPROC)GetProcAddress(hDll, "_KeyProc@12");
}

void InstallActivityMonitor()
{
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, hookMouseProc, hDll, 0); 
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookKeyProc, hDll, 0); 
	
	//if (hMouseHook)
	//	SetupMouseCallback(ActivityMonitor_MouseCallback);
	if (hKeyHook)
		SetupKeyCallback(ActivityMonitor_KeyCallback);
}

void UninstallActivityMonitor()
{
	if (hKeyHook)
		UnhookWindowsHookEx(hKeyHook);

	//if (hMouseHook)
	//	UnhookWindowsHookEx(hMouseHook);

	hKeyHook = 0;
	hMouseHook = 0;
}