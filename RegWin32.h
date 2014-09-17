#if !defined(AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_)
#define AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_

#if _MSC_VER > 1001
#pragma once
#endif

#include <windows.h>
#include <xstring>
#include <tchar.h>

#define MAX_SIZE 1024
//typedef enum {
//	WIN32,WIN95,WIN98,WINME,WINNT,WIN2K,WINXP,LONGHORN,OTHER
//} enum_win;

class CRegisterWIN32
{
public:
	CRegisterWIN32();
	~CRegisterWIN32();

public:
   void  InitReg();
   void  DelReg();
   BOOL  DeleteKey(HKEY m_RootKey, LPCTSTR _szPath);
   BOOL  DeleteValue(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR _szValue);
   BOOL  CreateKey(HKEY m_RootKey, LPCTSTR _szKey);

   BOOL  WriteStrVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey, LPCTSTR _szValue);
   BOOL  WriteIntVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey, int _iVal);

   LPCTSTR  ReadStrVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey);
   int      ReadIntVal(HKEY m_RootKey, LPCTSTR _szKey,LPCTSTR SubKey);

   int   GetOSversion();
   BOOL  RestoreKey(HKEY _hKeyRoot, LPCTSTR _szKey, LPCTSTR filename);

private:
	DWORD       m_dTemp;
	int         iOSversion;
	HKEY        m_hKey;
	TCHAR*       m_szTemp;

	BOOL  DeleteNTway(HKEY _hKey, LPCTSTR _szSubKey);
	BOOL  SetPrivilege(LPCTSTR seName, bool enabled);
	void  RegLocalized(BOOL blnLoad);
};
#endif // !defined(AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_)
