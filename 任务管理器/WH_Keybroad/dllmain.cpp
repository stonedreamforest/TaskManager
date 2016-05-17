// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include <stdlib.h>
#include <tchar.h>
#include "iostream"


#define WCHAR_To_CHAR(lp_WChar,lpChar)\
	WideCharToMultiByte(CP_ACP,NULL,lp_WChar,-1,lpChar,_countof(lpChar),NULL,FALSE);

using namespace std;
HINSTANCE g_Histance = 0;
HHOOK     g_Hook     = 0;
FILE *	  g_pFile    =NULL;
LRESULT CALLBACK KeyboardProc(
	int code,       // 消息类型
	WPARAM wParam,  // 虚拟码
	LPARAM lParam   // 按键信息
	)
{
	return 1;
}


BOOL InstallHook() 
{
	g_Hook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_Histance, 0);
	if (NULL == g_Hook)
		return FALSE;
	return TRUE;
}
BOOL UninstallHook()
{
	if (NULL == g_Hook)
		return FALSE;
	return UnhookWindowsHookEx(g_Hook);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_Histance = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

