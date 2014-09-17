#pragma once

#include "resource.h"
#include "func.h"
#include "trayicon.h"
#include "RegWin32.h"
#include "SingleInstance.h"

DWORD WINAPI InitPro(LPVOID lpParam);
void  RefreshDesktop();
void  DelReg();

//#define WM_WHERE_AM_I_MSG _T(_T("WM_WHERE_AM_I-{673F5235-6F6D-4ADA-A8DE-E34DC0CEF5F1}"))
//const UINT WM_WHERE_AM_I = RegisterWindowMessage(WM_WHERE_AM_I_MSG);

extern void getAppDir(TCHAR buf[],TCHAR path[])
{
	GetModuleFileName(NULL, path, MAX_PATH);
	wcscpy(buf,path);
	size_t i = (wcsrchr(buf, '\\') - buf);
	buf[i+1]=0;
}

//extern void getAppDir(char buf[],char path[])
//{
//	GetModuleFileNameA(NULL, path, MAX_PATH);
//	strcpy(buf,path);
//	size_t i = (strchr(buf, '\\') - buf);
//	buf[i+1]=0;
//}

extern TCHAR* StrAppend(TCHAR* str1, TCHAR* str2)
{
	TCHAR* res;
	size_t length=wcslen(str1) + wcslen(str2);
	res=new TCHAR[length+1];
	wcscpy(res,str1);
	wcsncat(res, str2,length);
	return res;
}

//extern char* StrAppend(char* str1, char* str2)
//{
//	char* res;
//	size_t length=strlen(str1) + strlen(str2);
//	res=new char[length+1];
//	strcpy(res,str1);
//	strcat(res, str2,length);
//	return res;
//}

void Exec(TCHAR* prog,TCHAR* param = _T(""))
{
  ShellExecute (GetDesktopWindow(), _T("Open"), prog, param, NULL, SW_SHOWNORMAL);
}

void RefreshDesktop()
{
    HWND DeskWin;
    DeskWin = FindWindowEx(0, 0, _T("Progman"), NULL);
    DeskWin = FindWindowEx(DeskWin, 0, _T("SHELLDLL_DefView"), NULL);
    DeskWin = FindWindowEx(DeskWin, 0, _T("SysListView32"), NULL);
    PostMessage(DeskWin, WM_KEYDOWN, 0x74, 0x3F0001);
}

//BOOL CALLBACK SearchOtherInstance(HWND hWnd, LPARAM lParam) {
//	DWORD result;
//	LRESULT ok = SendMessageTimeout(hWnd, WM_WHERE_AM_I, 0, 0,
//		SMTO_BLOCK | SMTO_ABORTIFHUNG, 5000, &result);
//	if(ok == 0)
//		return TRUE;
//	if(result == WM_WHERE_AM_I) {
//		// found it
//		HWND *target = (HWND *)lParam;
//		*target = hWnd;
//		return FALSE;
//	}
//	return TRUE;
//}

//TCHAR *strrepl(TCHAR *Str, size_t BufSiz, TCHAR *OldStr, TCHAR *NewStr)
//{
//      int OldLen, NewLen;
//      TCHAR *p, *q;
//
//      if(NULL == (p = strstr(Str, OldStr)))
//            return Str;
//      OldLen = wcslen(OldStr);
//      NewLen = wcslen(NewStr);
//      if ((wcslen(Str) + NewLen - OldLen + 1) > BufSiz)
//            return NULL;
//      memmove(q = p+NewLen, p+OldLen, wcslen(p+OldLen)+1);
//      memcpy(p, NewStr, NewLen);
//      return q;
//}
