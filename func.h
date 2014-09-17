#ifndef _FUNC_H__
#define _FUNC_H__

#define    NO_SHLWAPI_STRFCNS
#define    NO_SHLWAPI_REG
#define    NO_SHLWAPI_STREAM
#define    NO_SHLWAPI_GDI

#include <windows.h>
#include "shlwapi.h"
#include "Texts.h"

#define MAX_TEXT_LENGTH		1024

class CWindow_Enumer
{
	friend BOOL CALLBACK EnumChildProc(HWND, LPARAM);
	friend void write_HTML_texts(HWND hwnd);

private:
	HWND pwnd;
	int cur_arrnum;
	bool blnHideShow;
	const TCHAR* cur_per;

	wstr_list    DumCap;
	str_list::iterator   str_iter;
	wstr_list::iterator  eng_iter;
	wstr_list::iterator  wstr_iter;

public:
	CTranslator trans;

public:
	CWindow_Enumer();
	bool GetWin();
	void Write_to_Window();
	void TransStartMenu(bool);
	int get_cur_num(){return cur_arrnum;}
	~CWindow_Enumer();

private:
	bool win_valid(TCHAR*,HWND);
};
#endif