/*
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
 * func.cpp : The Windows handling functions.
 * This file is part of the XPersian - XP Persian interface -
*/

#include "func.h"
//#pragma warning( disable : 4312)
//#pragma warning( disable : 4311)

extern BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);

CWindow_Enumer::CWindow_Enumer()
{
	DumCap.push_back( _T("START MENU"));
    DumCap.push_back( _T("PROGRAM MANAGER"));
	DumCap.push_back( _T("XPERSIA*"));
	DumCap.push_back( _T("*WINAMP*"));
}

CWindow_Enumer::~CWindow_Enumer()
{
	//delete trans;
}

bool CWindow_Enumer::GetWin()
{
	TCHAR win_caption[MAX_TEXT_LENGTH] = {0};
	long buf;
	HWND hwnd;
	pwnd=0;

	hwnd = GetForegroundWindow();
	buf  = GetWindowTextLength(hwnd);
	//win_caption = new TCHAR[buf+1];
	if (buf <= MAX_TEXT_LENGTH)
		GetWindowText(hwnd, win_caption, buf + 1);
	else
		return FALSE;
	/*win_caption =*/ _wcsupr(win_caption);

	if (win_valid(win_caption,hwnd))
	{
		eng_iter =trans.EngCaption.begin();
		wstr_iter=trans.PerCaption.begin();

		int i=0;
		while(1)
		{
			if ((PathMatchSpec(win_caption,*eng_iter)==TRUE) || ((*wstr_iter!= _T("")) && (PathMatchSpec(win_caption,*wstr_iter)==TRUE)))
			{
				cur_arrnum	= i;
				pwnd		= hwnd;
				cur_per		= *wstr_iter;
				break;
			}
			if (*eng_iter==trans.EngCaption.back())
				break;

			eng_iter++;
			wstr_iter++;
			i++;
		}
	}
	//delete []win_caption;
	if (pwnd)
		return true;
	else
		return false;
}

void CWindow_Enumer::Write_to_Window()
{
	if (pwnd!=0)
	{
		if (*wstr_iter!= _T(""))
			SetWindowText((HWND)pwnd, *wstr_iter);

		EnumChildWindows((HWND)pwnd,EnumChildProc,0);

		if (blnHideShow)
		{
			if (cur_per == _T("نتايج جستجو") || (GetWindowLong(pwnd, GWL_STYLE) & WS_THICKFRAME)) //the only exception
			{
				ShowWindow ((HWND)pwnd, SW_HIDE);
				ShowWindow ((HWND)pwnd, SW_SHOWNORMAL);
			}
			SetWindowLong ((HWND)pwnd, -20, 0x400000 | GetWindowLong((HWND)pwnd, -20));//GWL_EXSTYLE = -20
			RedrawWindow ((HWND)pwnd,0,0, RDW_INVALIDATE);
			blnHideShow = false;
		}
	}
}

bool CWindow_Enumer::win_valid(TCHAR* win_title,HWND hwnd)
{
	if (PathMatchSpec(win_title,_T("SEARCH RESULTS")) | PathMatchSpec(win_title,_T("نتايج جستجو")))
		return true;

	if (win_title==_T("")) 
		return false;

  //  if (GetWindowLong(hwnd, GWL_STYLE) & WS_THICKFRAME)
		//return false;

	wstr_list::const_iterator cIter;
	cIter=DumCap.begin();

	if (PathMatchSpec(win_title, *cIter)==TRUE) 
		return false;

	do
	{
		cIter++;
		if (PathMatchSpec(win_title, *cIter)==TRUE) 
			return false;
	}while (*cIter!=DumCap.back());

	return true;
}

void CWindow_Enumer::TransStartMenu(bool per)
{
	HWND HS,HT;
	HT = FindWindow((LPCTSTR)_T("Shell_TrayWnd"), NULL);
		//Get the start-button's window handle
	HS = FindWindowEx(HT, 0, (LPCTSTR)_T("BUTTON"), NULL);

	if (per)
		SetWindowText(HS, (LPCTSTR)_T("شروع"));
	else
		SetWindowText(HS, (LPCTSTR)_T("start"));

	RedrawWindow(HS, 0,0, RDW_INVALIDATE);
}