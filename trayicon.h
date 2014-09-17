#ifndef TRAYICO_H
#define TRAYICO_H

#include "windows.h"
#include "shellapi.h"
#include <tchar.h>

#define WM_TRAYICON WM_USER+6

class TrayIcon
{
private:
	NOTIFYICONDATA	nid;
	bool			m_show;
public:
	TrayIcon(HWND hWnd, HICON hIcon, bool iShow=true, TCHAR *iTip=_T("Tray icon"));
	~TrayIcon();
	void	show();
	void	icon(HICON hIcon);
	void	hide();
	void	tip(TCHAR *newTip);
};

#endif //define TRAYICO_H