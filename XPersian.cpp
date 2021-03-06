﻿/*
 * Copyright (C) 2001 Mohsen Ali Momeni - All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * XPersian.cpp : Defines the entry point for the application.
 * This file is part of the XPersian - XP Persian interface.
*/

#include "stdafx.h"
#include "XPersian.h"

#include <atlbase.h>
#include <oleacc.h>
#include <mshtml.h>
#include <comdef.h>

#define MAX_LOADSTRING 100
#define ID_BUTTON_CLOSE 1

// Global Variables:
HINSTANCE hInst;		// current instance
HWND hWnd;
HANDLE hThread;
HMENU hMenu;
CWindow_Enumer* winenum;
CRegisterWIN32* reg;
bool endflag;

TCHAR AppDir[MAX_PATH+1];
TCHAR AppPath[MAX_PATH+1];

TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//UINT uShMsg;

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	getAppDir(AppDir,AppPath);
	MSG msg;
	HACCEL hAccelTable;

	SingleInstance xperapp("{XPERSIAN-45Ab04F-A56A-45FE-CE4B-34535AAAA351}");
    //MessageBox(0,_T("This is a test, See all the new features and tell me if everythin is alright!!!"),_T("Test XPersian"),MB_OK);
	if(xperapp.IsAnotherInstanceRunning()) {
		//HWND hOther = NULL;
		//EnumWindows(SearchOtherInstance, (LPARAM)&hOther);

		//if( hOther != NULL ) {
		//	SetForegroundWindow(hOther);
		//	if( IsIconic(hOther))
		//		ShowWindow(hOther, SW_RESTORE);
		//}
		return FALSE;
	}
	// Initialize global strings
	reg=new CRegisterWIN32();
	//int winver=reg->GetOSversion();
	//if (winver!=6)
	//{
	//	MessageBox(0,_T("This program is just intended for windows XP!"),_T("XPersian"),MB_OK);
	//	return FALSE;
    //}

	reg->InitReg();
	
	if (reg->ReadIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("FirstTime"))==-1)
	{
		reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("FirstTime"),1);
		reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("ShowDate"), 1);
		if (MessageBox(0, _T("آيا مي‌خواهيد اين برنامه هنگام شروع ويندوز اجرا شود؟"),
			_T("XPersian"),MB_YESNO | MB_ICONQUESTION)==6) {
			reg->WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),_T("XPersian"),AppPath);
			reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("Startup"),1);
			Exec(StrAppend(AppDir, _T("startup.exe")), _T("--INSTALL"));
		}
	}

	hMenu=LoadMenu(hInst,(LPCTSTR)IDC_XPERSIAN);
	hMenu= GetSubMenu(hMenu,0);

	if (reg->ReadIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("Startup"))==1)
		CheckMenuItem(hMenu, IDM_STARTUP, MF_BYCOMMAND | MF_CHECKED);
	else
		CheckMenuItem(hMenu, IDM_STARTUP, MF_BYCOMMAND | MF_UNCHECKED); 

	if (reg->ReadIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("ShowDate"))==1){
		CheckMenuItem(hMenu, IDM_SHOWDATE, MF_BYCOMMAND | MF_CHECKED);
		Exec(StrAppend(AppDir,_T("ostpd.exe")));
	}else
		CheckMenuItem(hMenu, IDM_SHOWDATE, MF_BYCOMMAND | MF_UNCHECKED);

	delete reg;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_XPERSIAN, szWindowClass, MAX_LOADSTRING);

	InitCommonControls();

	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_XPERSIAN);
	//uShMsg=RegisterWindowMessage(TEXT(_T("SHELLHOOK")));
	DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
	TrayIcon myTray(hWnd, (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_XPERSIAN), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR), true, _T("XPersian Tray"));

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_XPERSIAN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_3DFACE+1);
	wcex.lpszMenuName	= 0;//(LPCTSTR)IDC_XPERSIAN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_XPERSIAN);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   DWORD dwThreadId, dwThrdParam = 1;
   hInst = hInstance; // Store instance handle in our global variable

   const int iWidth = 380;
   const int iHeight= 260;
   int iX, iY;
   iX = ( GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2;  //Get center of the screen
   iY = ( GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_SIZEBOX ,
      iX, iY,iWidth, iHeight, NULL, NULL, hInstance, NULL);
   SetWindowLong ((HWND)hWnd, -20, 0x400000 | GetWindowLong((HWND)hWnd, -20));//GWL_EXSTYLE = -20

   if (!hWnd)
      return FALSE;
   
   winenum=new CWindow_Enumer();
   hThread = CreateThread(NULL,0,InitPro,&dwThrdParam,0,&dwThreadId);

   if (hThread == NULL) return 0;
   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

    ShowWindow(hWnd, SW_MINIMIZE);
	ShowWindow(hWnd, SW_HIDE);
	RefreshDesktop();

   return TRUE;
}

DWORD WINAPI InitPro(LPVOID lpParam)
{
	endflag=true;
	while (endflag)
	{
		if (winenum->GetWin()==true)
		{
			winenum->trans.text_from_arrnum(winenum->get_cur_num());
			winenum->Write_to_Window();
			winenum->trans.unload_list();
		}
		Sleep(200);

		static int Icounter;
		Icounter++;

		if (Icounter == 2)
			winenum->TransStartMenu(true);
		else if (Icounter==15)
			Icounter = 1;
		
	}
	endflag=true;
	return 0;
}

//void write_HTML_texts(HWND hwnd)
//{
//	CoInitialize(NULL);
//	HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
//	TCHAR* wintitle = 0;
//	CComPtr<IHTMLDocument2> spDoc;			
//	LRESULT lRes;
//
//	UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
//	::SendMessageTimeout( hwnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );
//
//	LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, "ObjectFromLresult" );
//	if ( pfObjectFromLresult != NULL )
//	{
//		HRESULT hr;
//		hr = (*pfObjectFromLresult)( lRes, IID_IHTMLDocument2, 0, (void**)&spDoc );
//		if ( SUCCEEDED(hr) )
//		{
//			CComPtr<IHTMLElement> pHTMLElement; 
//			hr=spDoc->get_body(&pHTMLElement);
//			//BSTR bstrText;
//			//pHTMLElement->get_innerText(&bstrText);
//			//if (bstrText)
//			//	wintitle= _bstr_t(bstrText);
//			//LPWSTR
//			BSTR test = SysAllocString(L"<P dir=ltr>محسن علي مومني</P>");
//			if (test)
//				pHTMLElement->put_innerHTML(test);
//			//winenum->str_iter=winenum->trans.EngTexts.begin();
//			//winenum->wstr_iter=winenum->trans.PerTexts.begin();
//			//
//			//if (PathMatchSpec(wintitle,*winenum->str_iter)==TRUE)
//			//{
//			//	BSTR parsiText = SysAllocString(*winenum->wstr_iter);
//			//	pHTMLElement->put_innerHTML(parsiText);
//			//	//winenum->blnHideShow = true;
//
//			//	winenum->trans.EngTexts.erase(winenum->str_iter);
//			//	winenum->trans.PerTexts.erase(winenum->wstr_iter);
//			//	//break;
//			//	//if (*winenum->str_iter == winenum->trans.EngTexts.back())
//			//	//	break;
//			//	winenum->str_iter++;
//			//	winenum->wstr_iter++;
//			//}
//		}
//	}
//	::FreeLibrary( hInst );
//}

#define CLASS_SIZE 500
extern BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam)
{
	//char wndowclass[CLASS_SIZE];
	//if (GetClassNameA(hwnd,wndowclass,CLASS_SIZE)==0)
	//	return TRUE;
	
	//if (!strncmp(wndowclass, "Internet Explorer_Server", 24))
	//{
	//	write_HTML_texts(hwnd);
	//	return TRUE;
	//}

	char wintitle[MAX_TEXT_LENGTH] = {0};
	int len;

	len=GetWindowTextLength(hwnd)+1;
	//wintitle=new char[len];
	if (len <= MAX_TEXT_LENGTH)
		GetWindowTextA(hwnd, wintitle,len);
	else
		return FALSE;
	winenum->str_iter=winenum->trans.EngTexts.begin();
	winenum->wstr_iter=winenum->trans.PerTexts.begin();

	while(1)
	{
		if (wintitle)
			if (PathMatchSpecA(wintitle, *winenum->str_iter)==TRUE)
			{
				SetWindowText(hwnd, *winenum->wstr_iter);
				winenum->blnHideShow = true;
				SetWindowLong ((HWND)hwnd, -20, 0x400000 | GetWindowLong((HWND)hwnd, -20));//GWL_EXSTYLE = -20

				winenum->trans.EngTexts.erase(winenum->str_iter);
				winenum->trans.PerTexts.erase(winenum->wstr_iter);
				break;
			}

		if (*winenum->str_iter == winenum->trans.EngTexts.back())
			break;
		winenum->str_iter++;
		winenum->wstr_iter++;
	}
	//delete []wintitle;
	return TRUE;
}

void termdate()
{
	HWND hdate =FindWindow(_T("ThunderRT6FormDC"),_T("ostpd.exe"));
	DWORD prcid,lpex;
	GetWindowThreadProcessId(hdate, &prcid);
	HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, 0, prcid);
	GetExitCodeProcess(hp,&lpex);
	TerminateProcess(hp,lpex);
	CloseHandle(hp);
}

void CreateChild(HWND hWnd)
{
	HWND btn_hwnd = CreateWindow(_T("Button"),_T("بستن پنجره"),WS_CHILD | WS_VISIBLE, 10,200,100,25,hWnd,(HMENU)ID_BUTTON_CLOSE,hInst,0);
	HWND hgod = CreateWindow(_T("STATIC"),_T("به نام خداوند بخشنده مهربان"),WS_CHILD | WS_VISIBLE,120,10,150,30,hWnd,(HMENU)0,hInst,0);
	HWND hname = CreateWindow(_T("STATIC"),_T("مترجم رابط ویندوز (هاي 2000 به بعد) به زبان فارسی"),WS_CHILD | WS_VISIBLE,
				65,50,450,30,hWnd,(HMENU)0,hInst,0);

	HWND lbl3 =CreateWindow(_T("static"),_T("برنامه‌نویس:             "),WS_CHILD | WS_VISIBLE,
				90,100,250,30,hWnd,(HMENU)0,hInst,0);
	HWND lbl4 =CreateWindow(_T("STATIC"),_T("وب سایت:			"),WS_CHILD | WS_VISIBLE,
				90,120,250,30,hWnd,(HMENU)0,hInst,0);
	HWND lbl2 =CreateWindow(_T("STATIC"),_T("ايميل: "),WS_CHILD | WS_VISIBLE,
				-10,140,350,30,hWnd,(HMENU)0,hInst,0);
	HWND lbl0 =CreateWindow(_T("STATIC"),_T("تمام حقوق این برنامه برای برنامه‌نویس محفوظ است"),WS_CHILD | WS_VISIBLE,
				-50,170,350,30,hWnd,(HMENU)0,hInst,0);
	//HWND lbl1 =CreateWindow(_T("STATIC"),_T("Address: NO.56, 19th Felestin, Felestin St., Mashad, IRAN \nPhone:   0511-8411897"),WS_CHILD | WS_VISIBLE,
	//			15,180,350,30,hWnd,(HMENU)0,hInst,0);
	SetWindowLong ((HWND)lbl3, -20, 0x400000 | GetWindowLong((HWND)lbl3, -20));//GWL_EXSTYLE = -20
	SetWindowLong ((HWND)lbl4, -20, 0x400000 | GetWindowLong((HWND)lbl4, -20));//GWL_EXSTYLE = -20
	SetWindowLong ((HWND)lbl2, -20, 0x400000 | GetWindowLong((HWND)lbl0, -20));//GWL_EXSTYLE = -20
	SetWindowLong ((HWND)lbl0, -20, 0x400000 | GetWindowLong((HWND)lbl0, -20));//GWL_EXSTYLE = -20

	SendMessage(lbl0, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	//SendMessage(lbl1, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(lbl2, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(lbl3, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(lbl4, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(btn_hwnd, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(hgod, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
	SendMessage(hname, WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;
	DWORD stMenu;
	HWND hdate;
	// TODO: implmenting Shell hook to get WM_CREATE messages. Instead of getting windows by GetWin function.
	//This is much better if works correctly.
	//if (uShMsg==message)
	//{
	//	switch(wParam)
	//	{
	//		case HSHELL_WINDOWCREATED:
	//			MessageBox(hWnd,_T("Window created"),_T("dum"),MB_OK);
	//			break;
	//		//case HSHELL_WINDOWACTIVATED:
	//		//	MessageBox(hWnd,_T("Window Activted"),_T("dum"),MB_OK);
	//		//	break;
	//		case HSHELL_TASKMAN:
	//			MessageBox(hWnd,_T("Window Task"),_T("dum"),MB_OK);
	//			break;
	//	}
	//	return 0;
	//}
	switch (message)
	{
		case WM_CREATE:
			CreateChild(hWnd);
			//RegisterShellHookWindow(hWnd);  //Registering shell hook
		//case WM_CTLCOLORSTATIC:
		//	HDC h;
		//	//HWND hw;
		//	h=(HDC) wParam;
		//	//hw=(HWND) lParam;
		//	SelectObject(h,GetStockObject(DC_PEN));
		//	SetDCPenColor(h,RGB(0,0,255));

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
					ShowWindow(hWnd,1);
					break;
				case IDM_STARTUP:
					reg=new CRegisterWIN32();
					stMenu = GetMenuState(hMenu, IDM_STARTUP, MF_BYCOMMAND); 
					if (!(stMenu & MF_CHECKED)) {
						CheckMenuItem(hMenu, IDM_STARTUP, MF_BYCOMMAND | MF_CHECKED); 
						reg->WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),_T("XPersian"),AppPath);
						reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("Startup"),1);
						Exec(StrAppend(AppDir, _T("startup.exe")), _T("--INSTALL"));
					} else {
						CheckMenuItem(hMenu, IDM_STARTUP, MF_BYCOMMAND | MF_UNCHECKED);
						reg->DeleteValue(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),_T("XPersian"));
						reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("Startup"),0);
						Exec(StrAppend(AppDir, _T("startup.exe")), _T("--UNINSTALL"));
					}
					delete reg;
					break;
				case IDM_SHOWDATE:
					reg=new CRegisterWIN32();
					stMenu = GetMenuState(hMenu, IDM_SHOWDATE, MF_BYCOMMAND); 
					if (!(stMenu & MF_CHECKED)) {
						CheckMenuItem(hMenu, IDM_SHOWDATE, MF_BYCOMMAND | MF_CHECKED); 
						reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("ShowDate"),1);
						Exec(StrAppend(AppDir,_T("ostpd.exe")));
					} else {
						CheckMenuItem(hMenu, IDM_SHOWDATE, MF_BYCOMMAND | MF_UNCHECKED);
						reg->WriteIntVal(HKEY_CURRENT_USER, _T("Software\\XPersian"),_T("ShowDate"),0);
						termdate();
					}
					delete reg;
					break;
				case IDM_DATE:
					hdate =FindWindow(_T("ThunderRT6FormDC"),_T("ostpd.exe"));
					PostMessage(hdate, WM_KEYDOWN, 0,0);
					break;
				case IDM_HELP:
					Exec(StrAppend(AppDir, _T("XPerCalender.exe")), _T("praisetheGOD"));
					if (MessageBox(hWnd, _T("تقويم از يونيكد حمايت نمي‌كند. بنابراين ممكن است حروف فارسي به درستي ديده نشوند. \n اگر اينطور است و شما ميخواهيد اين مشكل برطرف شود، دكمه بله را كليك كنيد"),
								_T("XPersian Properties"), MB_YESNO | MB_ICONQUESTION))
						Exec(StrAppend(AppDir, _T("XPerInst.exe")));
					break;
				case IDM_LOGO:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;
				case IDM_SITE:
					Exec(_T("http://www.momeni.us.tc"));
					break;
				case IDM_SNOOZE:
					stMenu = GetMenuState(hMenu, IDM_SNOOZE, MF_BYCOMMAND); 
					if (!(stMenu & MF_CHECKED)) {
						CheckMenuItem(hMenu, IDM_SNOOZE, MF_BYCOMMAND | MF_CHECKED); 
						SuspendThread(hThread);
					} else {
						CheckMenuItem(hMenu, IDM_SNOOZE, MF_BYCOMMAND | MF_UNCHECKED); 
						ResumeThread(hThread);
					}
					break;
				case ID_BUTTON_CLOSE:
					    ShowWindow(hWnd, SW_MINIMIZE);
						ShowWindow(hWnd, SW_HIDE);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	// TODO: Add any drawing code here...
		//	EndPaint(hWnd, &ps);
		//	break;
		case WM_TRAYICON:
				switch(LOWORD(lParam))
				{
				case WM_LBUTTONDBLCLK:
					ShowWindow(hWnd,1);
					break;
				case WM_RBUTTONUP:
						POINT *pt=new POINT;
						GetCursorPos(pt);
						SetMenuDefaultItem(hMenu,IDM_EXIT,TRUE);
						TrackPopupMenuEx(hMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt->x, pt->y, hWnd, NULL);
						delete pt;
					break;
				}
			break;
		//case WM_WHERE_AM_I:
		//	return WM_WHERE_AM_I;
		//	break;
		case WM_CLOSE:
			ShowWindow(hWnd,0);
			break;
		case WM_DESTROY:
			//DeregisterShellHookWindow(hWnd);
			endflag=false;
			DestroyMenu(hMenu);
			ResumeThread(hThread);
			reg=new CRegisterWIN32();
			reg->DelReg();
			delete reg;
			winenum->TransStartMenu(false);
			termdate();
			RefreshDesktop();
			while (!endflag){Sleep(100);}
			CloseHandle(hThread);
			delete winenum;
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//static int tmr;
	switch (message)
	{
		case WM_INITDIALOG:
			SetTimer(hDlg,1,3000,NULL);
			return TRUE;

		case WM_TIMER:
			KillTimer(hDlg,1);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;

		case WM_LBUTTONDOWN:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;

		case WM_COMMAND:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
	}
	return FALSE;
}
