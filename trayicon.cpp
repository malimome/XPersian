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
 * XPersian.cpp : Defines the entry point for the application.
 * This file is part of the XPersian - XP Persian interface -
*/

#include "trayicon.h"
#pragma warning( disable : 4311)

TrayIcon::TrayIcon(HWND hWnd, HICON hIcon, bool iShow, TCHAR *iTip)
{
	nid.cbSize=sizeof(nid);
	nid.hIcon=hIcon;
	nid.hWnd=hWnd;
	wcscpy(nid.szTip, iTip);
	nid.uCallbackMessage=WM_TRAYICON;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nid.uID=(UINT)hWnd;
	if (m_show=iShow) 
		Shell_NotifyIcon(NIM_ADD, &nid); //assign m_show, not compare
}//end func trayIcon

TrayIcon::~TrayIcon()
{
	if (m_show) 
		Shell_NotifyIcon(NIM_DELETE, &nid);
	DestroyIcon(nid.hIcon);
	memset(&nid, 0, sizeof(nid));
}//end func ~TrayIcon

void TrayIcon::show()
{
	if (!m_show)
	{
		Shell_NotifyIcon(NIM_ADD, &nid);
		m_show=true;
	}
}//end func show

void TrayIcon::icon(HICON hIcon)
{
	nid.hIcon=hIcon;
	if (m_show)
		Shell_NotifyIcon(NIM_MODIFY, &nid);
}//end func icon

void TrayIcon::hide()
{
	if(!m_show)
	{
		Shell_NotifyIcon(NIM_DELETE, &nid);
		m_show=false;
	}
}//end func hide

void TrayIcon::tip(TCHAR *newTip)
{
	wcscpy(nid.szTip, newTip);
	if (m_show)
		Shell_NotifyIcon(NIM_MODIFY, &nid);
}//end func tip



