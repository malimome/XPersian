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
 * RegWin32.cpp: Defines all registry functions.
 * This file is part of the XPersian - XP Persian interface -
*/

#include "RegWin32.h"
using namespace std;

extern TCHAR* StrAppend(TCHAR* , TCHAR* );
extern void getAppDir(TCHAR buf[],TCHAR path[]);

CRegisterWIN32::CRegisterWIN32()
{
	m_hKey = NULL;
    m_szTemp = new TCHAR[MAX_SIZE];
}

CRegisterWIN32::~CRegisterWIN32()
{
    if(m_szTemp)
        delete []m_szTemp;
}

BOOL CRegisterWIN32::CreateKey(HKEY m_RootKey, LPCTSTR m_szPath)
{
	BOOL ret= (RegCreateKeyEx(m_RootKey, m_szPath, 0, 0,
						REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
						0, &m_hKey, 0) == ERROR_SUCCESS );
	RegCloseKey(m_hKey);
	return (ret);
}

int CRegisterWIN32::GetOSversion()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osv))
	{
		switch(osv.dwPlatformId)
		{
			case VER_PLATFORM_WIN32s:
				return 0;//WIN32;
				break;
			case VER_PLATFORM_WIN32_WINDOWS:
				switch(osv.dwMinorVersion)
				{
					case 0:
						return 1;//WIN95;
						break;
					case 10:
						return 2;//WIN98;
						break;
					case 90:
						return 3;//WINME;
						break;
				} //end switch in
				break;
			case VER_PLATFORM_WIN32_NT:
				if (osv.dwMajorVersion<5)
					return 4;//WINNT;
				else if (osv.dwMajorVersion==5 && osv.dwMinorVersion==0)
					return 5;//WIN2K;
				else if (osv.dwMajorVersion==5 && osv.dwMinorVersion==1)
					return 6;//WINXP;
				else if (osv.dwMajorVersion>=5 && osv.dwMinorVersion>1)
					return 7;//LONGHORN;
				break;
			default:
				return 8;//OTHER;
				break;
		}//end switch out
	}//end if
	return 8;
}//end func
BOOL SysLanguageName(LPCTSTR lang_name)
{
    //Returns the System Language Name eg : English (United Kingdom)
    long lLangID; 
    TCHAR *sBuffer;
    long lBuffSize;
    long lRet; 

    lLangID = GetSystemDefaultLangID();
    //Setup a buffer to receive the settings
    lBuffSize = 100;
    sBuffer = new TCHAR[lBuffSize];
    lRet = VerLanguageName(lLangID, sBuffer, lBuffSize);
    if (lRet > 0)
	{
		lang_name=new TCHAR[lRet];
        memmove((void *)lang_name ,sBuffer, lRet);
		return TRUE;
	}

	return FALSE;
}

BOOL CRegisterWIN32::WriteStrVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey, LPCTSTR _szValue)
{
	BOOL bResult = FALSE;
	DWORD dLen = (DWORD)wcslen(_szValue);
	if ( dLen > 0)
	{
		dLen = dLen * 2 +1;
		RegCreateKey(m_RootKey,_szKey,&m_hKey);
		bResult = ( RegSetValueEx(m_hKey, SubKey,0,
						REG_SZ, (CONST BYTE*)_szValue,
						dLen ) == ERROR_SUCCESS );
	}
	RegCloseKey(m_hKey);
	return bResult;
}

BOOL CRegisterWIN32::WriteIntVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey, int _iVal)
{
	RegCreateKey(m_RootKey,_szKey,&m_hKey);
	BOOL ret = ( RegSetValueEx(m_hKey, SubKey,0,
					REG_DWORD, (CONST BYTE*)&_iVal, sizeof(DWORD)) == ERROR_SUCCESS );
	RegCloseKey(m_hKey);
	return(ret);
}

LPCTSTR CRegisterWIN32::ReadStrVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey)
{
   m_szTemp = _T("");

   BOOL  bResult = FALSE;
   DWORD dwType;
   DWORD dwSize = MAX_SIZE;

	if ( wcslen(_szKey) > 0 )
	{
		RegOpenKey(m_RootKey, _szKey, &m_hKey);
		if (!m_hKey) return 0;

		bResult = ( RegQueryValueEx(m_hKey, SubKey, 0, &dwType,
                              (BYTE*)m_szTemp, 
                              &dwSize) == ERROR_SUCCESS);
	}
	RegCloseKey(m_hKey);
    return m_szTemp;
}

int CRegisterWIN32::ReadIntVal(HKEY m_RootKey, LPCTSTR _szKey, LPCTSTR SubKey)
{
	BOOL bResult = FALSE;
    DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	RegOpenKey(m_RootKey,_szKey,&m_hKey);
	if (!m_hKey) return (-1);

	if (! RegQueryValueEx(m_hKey, SubKey, 0, &dwType,(BYTE*)&m_dTemp, &dwSize) == ERROR_SUCCESS)
		return (-1);

	RegCloseKey(m_hKey);
	return (int)m_dTemp;
}

BOOL CRegisterWIN32::DeleteValue(HKEY m_RootKey, LPCTSTR _szKey,LPCTSTR _szValue)
{
   RegOpenKey(m_RootKey, _szKey, &m_hKey);
   BOOL bResult = (RegDeleteValue(m_hKey, _szValue) == ERROR_SUCCESS );
   RegCloseKey(m_hKey);
   return bResult;
}

BOOL CRegisterWIN32::DeleteKey(HKEY m_RootKey, LPCTSTR _szPath)
{
	BOOL bResult  = FALSE;

	LPTSTR pszSubKey = new TCHAR[_MAX_PATH];
	LPTSTR pszPath = new TCHAR[_MAX_PATH];
	LPTSTR pDest= NULL;
	LPTSTR pTemp = NULL;
	//** If Path empty we get out
	if( wcslen(_szPath) > 0)
   {
      pDest= (LPTSTR)wcsrchr(_szPath, '\\');
		if (pDest)
		{
			//** Must extract the Last Key
			pTemp = pDest;
			pDest++;
			wcscpy(pszSubKey, pDest);
			pDest = pTemp;
			int iCount=0;
			__w64 int Result=0;
			Result = pDest - _szPath ;
			do
			{
				pszPath[iCount] = _szPath[iCount];
				iCount++;
			}while ( iCount < Result );
			pszPath[iCount]= '\0';
		}
		else
		{
			wcscpy(pszSubKey, _szPath);
			wcscpy(pszPath, __T(""));
		}

		HKEY hKey;
		if ( RegOpenKeyEx(m_RootKey, pszPath, 0L, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS )
		{
		   if ( GetOSversion() >= 4 )
			   if (DeleteNTway(hKey, pszSubKey)) bResult = TRUE;
		   else
			   if (RegDeleteKey(hKey , pszSubKey) == ERROR_SUCCESS) bResult= TRUE;

         RegCloseKey(hKey);
       }
   }

   if(pszSubKey)  
      delete pszSubKey;
   if (pszPath)
      delete pszPath;
	return bResult;
}

BOOL CRegisterWIN32::DeleteNTway(HKEY _hKey, LPCTSTR _szSubKey)
{
	BOOL bResult = FALSE;
   LPTSTR pszTemp = new TCHAR[_MAX_PATH];
	HKEY  lhKey;
	FILETIME ft;
	LONG  lResult;
	DWORD dwKeyLen;
	
	//** If Path empty we get out
	if( wcslen(_szSubKey) > 0)
   {
      if(RegOpenKeyEx(_hKey, _szSubKey, 0L, KEY_ENUMERATE_SUB_KEYS, 
               &lhKey) == ERROR_SUCCESS )
		{
			//** So far the Key exist
			do
			{
				dwKeyLen = _MAX_PATH;
				lResult = RegEnumKeyEx(lhKey, 0, pszTemp, &dwKeyLen, NULL, NULL,
						      NULL, &ft);
				switch (lResult)
				{
					case ERROR_NO_MORE_ITEMS:
						//** No more Subkey so delete the base
						if ( RegDeleteKey(_hKey ,_szSubKey) == ERROR_SUCCESS )
						{
							bResult= TRUE;
							break;
						}
						break;
					case ERROR_SUCCESS:
						if( DeleteNTway(lhKey, pszTemp) )
							bResult = TRUE;
						break;
				}
			}while( lResult == ERROR_SUCCESS);
		RegCloseKey(lhKey);
		}
   }
   if(pszTemp)
      delete []pszTemp;
	return bResult;
}

BOOL CRegisterWIN32::RestoreKey(HKEY hKey, LPCTSTR _szKey, LPCTSTR filename)
{
    SetPrivilege(SE_RESTORE_NAME, true);
	SetPrivilege(SE_BACKUP_NAME, true);
    HKEY hKeyRet;
	RegCreateKey(hKey,_szKey,&hKeyRet);
    //RegOpenKey(hKey, _szKey, &hKeyRet);
    RegRestoreKey(hKeyRet, filename, REG_FORCE_RESTORE);
    RegCloseKey (hKeyRet);
	SetPrivilege(SE_RESTORE_NAME, false);
	SetPrivilege(SE_BACKUP_NAME, false);
	return TRUE;
}

void CRegisterWIN32::RegLocalized(BOOL blnLoad)
{
	wstring rgCLSID[]={
		_T("{0DF44EAA-FF21-4412-828E-260A8728E7F1}"),
		_T("{208D2C60-3AEA-1069-A2D7-08002B30309D}"),
		_T("{20D04FE0-3AEA-1069-A2D8-08002B30309D}"),
		_T("{2227A280-3AEA-1069-A2DE-08002B30309D}"),
		_T("{2559a1f0-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2559a1f1-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2559a1f3-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2728520d-1ec8-4c68-a551-316b684c4ea7}"),
		_T("{645FF040-5081-101B-9F08-00AA002F954E}"),
		_T("{6DFD7C5C-2451-11d3-A299-00C04F8EF6AF}"),
		_T("{7007ACC7-3202-11D1-AAD2-00805FC1270E}"),
		_T("{85BBD920-42A0-1069-A2E4-08002B30309D}"),
		_T("{992CFFA0-F557-101A-88EC-00DD010CCC48}"),
		_T("{D20EA4E1-3957-11d2-A40B-0C5020524153}"),
		_T("{D6277990-4C6A-11CF-8D87-00AA0060F5BF}"),
		_T("{E211B736-43FD-11D1-9EFB-0000F8757FCD}"),
		_T("{D20EA4E1-3957-11d2-A40B-0C5020524152}"),
		_T("{00021401-0000-0000-C000-000000000046}"),
		_T("{025A5937-A6BE-4686-A844-36FE4BEC8B6D}"),
		_T("{04731B67-D933-450a-90E6-4ACD2E9408FE}"),
		_T("{0A4286EA-E355-44FB-8086-AF3DF7645BD9}"),
		_T("{15eae92e-f17a-4431-9f28-805e482dafd4}"),
		_T("{17cd9488-1228-4b2f-88ce-4298e93e0966}"),
		_T("{1BA783C1-2A30-4ad3-B928-A9A46C604C28}"),
		_T("{1FA9085F-25A2-489B-85D4-86326EEDCD87}"),
		_T("{21EC2020-3AEA-1069-A2DD-08002B30309D}"),
		_T("{2559a1f2-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2559a1f4-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2559a1f5-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{2559a1f7-21d7-11d4-bdaf-00c04f60b9f0}"),
		_T("{26EE0668-A00A-44D7-9371-BEB064C98683}"),
		_T("{2728520d-1ec8-4c68-a551-316b684c4ea7}"),
		_T("{2DF874B6-41CB-41F4-8ECC-A5F8461E63B5}"),
		_T("{2F2165FF-2C2D-4612-87B2-CC8E5002EF4C}"),
		_T("{3080F90D-D7AD-11D9-BD98-0000947B0257}"),
		_T("{3080F90E-D7AD-11D9-BD98-0000947B0257}"),
		_T("{335a31dd-f04b-4d76-a925-d6b47cf360df}"),
		_T("{35786D3C-B075-49b9-88DD-029876E11C01}"),
		_T("{36eef7db-88ad-4e81-ad49-0e313f0c35f8}"),
		_T("{375C3A49-8654-49C6-BD32-7E7FE88509B4}"),
		_T("{37efd44d-ef8d-41b1-940d-96973a50e9e0}"),
		_T("{3ad05575-8857-4850-9277-11b85bdb8e09}"),
		_T("{3e7efb4c-faf1-453d-89eb-56026875ef90}"),
		_T("{3f6bc534-dfa1-4ab4-ae54-ef25a74e0107}"),
		_T("{4026492F-2F69-46B8-B9BF-5654FC07E423}"),
		_T("{4336a54d-038b-4685-ab02-99bb52d3fb8b}"),
		_T("{44C39C96-0167-478F-B68D-783294A2545D}"),
		_T("{450D8FBA-AD25-11D0-98A8-0800361B1103}"),
		_T("{48e7caab-b918-4e58-a94d-505519c795dc}"),
		_T("{514B5E31-5596-422F-BE58-D804464683B5}"),
		_T("{5399E694-6CE5-4D6C-8FCE-1D8870FDCBA0}"),
		_T("{58E3C745-D971-4081-9034-86E34B30836A}"),
		_T("{60632754-c523-4b62-b45c-4172da012619}"),
		_T("{67718415-c450-4f3c-bf8a-b487642dc39b}"),
		_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}"),
		_T("{6D3951EB-0B07-4fb8-B703-7C5CEE0DB578}"),
		_T("{74246bfc-4c96-11d0-abef-0020af6b0b7a}"),
		_T("{76052C5C-2EB4-4C40-B1F1-2A5C8554590A}"),
		//_T(""),
		//_T(""),
		//_T(""),
		//_T(""),
		//_T(""),
	};

	wstring rgLS[60];
	if (blnLoad)
	{
		rgLS[0]=_T("منوي شروع و ميله وظيفه");
		rgLS[1]=_T("مکان هاي تحت شبکه");
		rgLS[2]=_T("کامپيوتر من");
		rgLS[3]=_T("فاکس و چاپگر ها");
		rgLS[4]=_T("جستجو");
		rgLS[5]=_T("راهنمايي و حمايت");
		rgLS[6]=_T("اجرا...");
		rgLS[7]=_T("برنامه نصب شبکه");
		rgLS[8]=_T("سطل بازيافت");
		rgLS[9]=_T("تنظبمات نمايش پوشه");
		rgLS[10]=_T("اتصالات شبکه");
		rgLS[11]=_T("چمدان");
		rgLS[12]=_T("اتصالات شبکه");
		rgLS[13]=_T("ابزار هاي مديريت سيستم");
		rgLS[14]=_T("وظايف زمان بندي شده");
		rgLS[15]=_T("دوربين و اسکنرها");
		rgLS[16]=_T("فونت");
		rgLS[17]=_T("میانبر");
		rgLS[18]=_T("تنظیمات منبع‌تغزیه");
		rgLS[19]=_T("پوشه جستجو");
		rgLS[20]=_T("اجراکننده مدیای ویندوز");
		rgLS[21]=_T("نصب برنامه‌های جدید");
		rgLS[22]=_T("تنظیم پیش‌فرض‌های کاربر");
		rgLS[23]=_T("ساختن لینک جدید");
		rgLS[24]=_T("مدیریت شبکه‌های بدون سیم");
		rgLS[25]=_T("صفحه کنترل");
		rgLS[26]=_T("امنیت ویندوز");
		rgLS[27]=_T("اینترنت");
		rgLS[28]=_T("ایمیل");
		rgLS[29]=_T("تنظیم دسترسی به برنامه‌ها و پیش‌فرض‌های آن");
		rgLS[30]=_T("صفحه کنترل");
		rgLS[31]=_T("نصب آسان شبکه");
		rgLS[32]=_T("نمایشگر اطمینان");
		rgLS[33]=_T("گزینه‌های ایندکس کردن پیشرفته");
		rgLS[34]=_T("نمایش دسکتاپ");
		rgLS[35]=_T("تعویض پنجره");
		rgLS[36]=_T("مرکز بازیابی و پشتیبانی");
		rgLS[37]=_T("وسایل قابل حمل");
		rgLS[38]=_T("به روز رسانی ویندوز");
		rgLS[39]=_T("نصب فضای ملاقات ویندوز");
		rgLS[40]=_T("ویژگی‌های میله کنار دسکتاپ");
		rgLS[41]=_T("عملیات فایل");
		rgLS[42]=_T("گرفتن برنامه از اینترنت");
		rgLS[43]=_T("بازیابی سیستم");
		rgLS[44]=_T("فایروال ویندوز");
		rgLS[45]=_T("عمومی");
		rgLS[46]=_T("مدیریت نام‌های شبکه");
		rgLS[47]=_T("اسناد");
		rgLS[48]=_T("پوشه منوی شروع");
		rgLS[49]=_T("گزینه‌های زبانی و منطقه‌ای");
		rgLS[50]=_T("صفحه کنترل");
		rgLS[51]=_T("گزینه‌های تشخیص صوت");
		rgLS[52]=_T("حساب‌های کاربران");
		rgLS[53]=_T("ویژگی‌های ویندوز");
		rgLS[54]=_T("اجراکننده مدیای ویندوز");
		rgLS[55]=_T("گزینه‌های پیشرفته ایندکس کردن");
		rgLS[56]=_T("مدیریت ابزار");
		rgLS[57]=_T("اتصال به پروژکتور شبکه");
		//rgLS[58]=_T("");
		//rgLS[59]=_T("");
		//rgLS[60]=_T("");
		//rgLS[61]=_T("");
	}
	else
	{
		rgLS[0]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-32517");
		rgLS[1]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-9217 ");
		rgLS[2]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-9216");
		rgLS[3]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-9319");
		rgLS[4]=_T("@explorer.exe,-7020");
		rgLS[5]=_T("@explorer.exe,-7021");
		rgLS[6]=_T("@explorer.exe,-7023");
		rgLS[7]=_T("Network Setup Wizard");
		rgLS[8]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-8964");
		rgLS[9]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-22985");
		rgLS[10]=_T("@%SystemRoot%\\system32\\netshell.dll,-1200");
		rgLS[11]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-22978");
		rgLS[12]=_T("@%SystemRoot%\\system32\\netshell.dll,-1200");
		rgLS[13]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-22982");
		rgLS[14]=_T("@%SystemRoot%\\System32\\mstask.dll,-3408");
		rgLS[15]=_T("@%SystemRoot%\\system32\\wiashext.dll,-331");
		rgLS[16]=_T("@%SystemRoot%\\system32\\SHELL32.dll,-22981");
		rgLS[17]=_T("@%SystemRoot%\\system32\\shell32.dll,-50197");
		rgLS[18]=_T("@%SystemRoot%\\System32\\powercpl.dll,-1");
		rgLS[19]=_T("@%SystemRoot%\\system32\\shell32.dll,-30523");
		rgLS[20]=_T("@%programfiles%\\WI4EB4~1\\wmpband.dll,-101");
		rgLS[21]=_T("@%SystemRoot%\\system32\\appwiz.cpl,-156");
		rgLS[22]=_T("@%SystemRoot%\\System32\\sud.dll,-1");
		rgLS[23]=_T("@%SystemRoot%\\system32\\appwiz.cpl,-13056");
		rgLS[24]=_T("@%SystemRoot%\\system32\\wlanpref.dll,-20001");
		rgLS[25]=_T("@%SystemRoot%\\system32\\shell32.dll,-4161");
		rgLS[26]=_T("@%SystemRoot%\\explorer.exe,-7022");
		rgLS[27]=_T("@%SystemRoot%\\explorer.exe,-7024");
		rgLS[28]=_T("@%SystemRoot%\\explorer.exe,-7025");
		rgLS[29]=_T("@%SystemRoot%\\system32\\appwiz.cpl,-130");
		rgLS[30]=_T("@%SystemRoot%\\system32\\shell32.dll,-4161");
		rgLS[31]=_T("Network Setup Wizard");
		rgLS[32]=_T("@%systemroot%\\system32\\relmon.dll,-502");
		rgLS[33]=_T("@%SystemRoot%\\system32\\srchadmin.dll,-617");
		rgLS[34]=_T("@%SystemRoot%\\system32\\shell32.dll,-10113");
		rgLS[35]=_T("@shell32.dll,-10114");
		rgLS[36]=_T("@%SystemRoot%\\System32\\brcpl.dll,-1");
		rgLS[37]=_T("@%SystemRoot%\\system32\\wpdshext.dll,-510");
		rgLS[38]=_T("@%SystemRoot%\\system32\\wucltux.dll,-1");
		rgLS[39]=_T("@%programfiles%\\Windows Collaboration\\WinCollabRes.dll,-7500");
		rgLS[40]=_T("@%ProgramFiles%\\Windows Sidebar\\sidebar.exe,-11003");
		rgLS[41]=_T("@%SystemRoot%\\system32\\shell32.dll,-50176");
		rgLS[42]=_T("@%SystemRoot%\\system32\\appwiz.cpl,-1070");
		rgLS[43]=_T("@%SystemRoot%\\system32\\appwiz.cpl,-190");
		rgLS[44]=_T("@%SystemRoot%\\system32\\FirewallSettings.exe,-12122");
		rgLS[45]=_T("@%SystemRoot%\\system32\\shell32.dll,-9113");
		rgLS[46]=_T("@%SystemRoot%\\system32\\netprof.dll,-248");
		rgLS[47]=_T("@%SystemRoot%\\system32\\shell32.dll,-9227");
		rgLS[48]=_T("@%SystemRoot%\\system32\\shell32.dll,-21786");
		rgLS[49]=_T("@%systemroot%\\system32\\intl.cpl,-1");
		rgLS[50]=_T("@%SystemRoot%\\system32\\shell32.dll,-4161");
		rgLS[51]=_T("@%SystemRoot%\\System32\\Speech\\SpeechUX\\speechuxcpl.dll,-1");
		rgLS[52]=_T("@%SystemRoot%\\System32\\usercpl.dll,-1");
		rgLS[53]=_T("@appwiz.cpl,-186");
		rgLS[54]=_T("@%SystemRoot%\\system32\\wmploc.dll,-1721");
		rgLS[55]=_T("@%SystemRoot%\\system32\\srchadmin.dll,-618");
		rgLS[56]=_T("@%SystemRoot%\\System32\\devmgr.dll,-4");
		rgLS[57]=_T("@%SystemRoot%\\system32\\NetProjW.dll,-6500");
		//rgLS[58]=_T("");
		//rgLS[59]=_T("");
		//rgLS[60]=_T("");
	}
	TCHAR strRG[129] = {0};
	for (int i=0;i<58;i++)   //38
	{
		wcscpy(strRG, _T("SOFTWARE\\Classes\\CLSID\\"));
		wcsncat(strRG, rgCLSID[i].c_str(),128);
		CRegisterWIN32::WriteStrVal(HKEY_LOCAL_MACHINE, strRG, _T("LocalizedString"), rgLS[i].c_str());
	}

	rgCLSID[0] = _T("ControlPanel");
	rgCLSID[0] = _T("EnableDragDrop");
	rgCLSID[0] = _T("Favorites");
	rgCLSID[0] = _T("HoverOpen");
	rgCLSID[0] = _T("MyComp");
	rgCLSID[0] = _T("MyDocs");
	rgCLSID[0] = _T("MyGames");
	rgCLSID[0] = _T("MyMusic");
	rgCLSID[0] = _T("MyPics");
	rgCLSID[0] = _T("NetConn");
	rgCLSID[0] = _T("NotifyNew");
	rgCLSID[0] = _T("SearchComm");
	rgCLSID[0] = _T("SearchFiles");
	rgCLSID[0] = _T("SearchInternet");
	rgCLSID[0] = _T("SearchPrograms");
	rgCLSID[0] = _T("ShowAdminTools");
	rgCLSID[0] = _T("ShowHelp");
	rgCLSID[0] = _T("ShowNetPlaces");
	rgCLSID[0] = _T("ShowPrinters");
	rgCLSID[0] = _T("ShowRun");
	rgCLSID[0] = _T("ShowSearch");
	rgCLSID[0] = _T("ShowSetProgramAccessAndDefaults");
	rgCLSID[0] = _T("SortByName");
	rgCLSID[0] = _T("UseLargeIcons");
	rgCLSID[0] = _T("User");

	if (blnLoad)
	{
		rgLS[0]=_T("صفحه کنترل");
		rgLS[1]=_T("فعال کردن منوی محیطی و عمل گرفتن و انداختن با ماوس");
		rgLS[2]=_T("منوی برگزیده‌ها");
		rgLS[3]=_T("زیرمنوی‌های منوها را هنگامی که با ماوس روی آنها توقف می‌کنم باز کن");
		rgLS[4]=_T("کامپیوتر");
		rgLS[5]=_T("اسناد");
		rgLS[6]=_T("بازی‌ها");
		rgLS[7]=_T("موسیقی");
		rgLS[8]=_T("تصاویر");
		rgLS[9]=_T("اتصال به");
		rgLS[10]=_T("برجسته‌کردن برنامه‌های جدید");
		rgLS[11]=_T("جستجوی ارتباطات");
		rgLS[12]=_T("جستجوی فایل‌ها");
		rgLS[13]=_T("جستجوی سابقه و برگزیده‌ها");
		rgLS[14]=_T("جستجوی برنامه‌ها");
		rgLS[15]=_T("ابزار‌های مدیریتی سیستم");
		rgLS[16]=_T("راهنما");
		rgLS[17]=_T("شبکه");
		rgLS[18]=_T("پرینترها");
		rgLS[19]=_T("دستور اجرا");
		rgLS[20]=_T("جستجو");
		rgLS[21]=_T("برنامه‌های پیش‌فرض");
		rgLS[22]=_T("مرتب‌سازی منوی برنامه‌ها با نام");
		rgLS[23]=_T("استفاده از آیکون‌های بزرگ");
		rgLS[24]=_T("پوشه شخصی");
	}
	else
	{
		rgLS[0]=_T("@shell32.dll,-30488");
		rgLS[1]=_T("@shell32.dll,-30475");
		rgLS[2]=_T("@shell32.dll,-30484");
		rgLS[3]=_T("@shell32.dll,-30573");
		rgLS[4]=_T("@shell32.dll,-30480");
		rgLS[5]=_T("@shell32.dll,-30485");
		rgLS[6]=_T("@shell32.dll,-30579");
		rgLS[7]=_T("@shell32.dll,-30487");
		rgLS[8]=_T("@shell32.dll,-30486");
		rgLS[9]=_T("@%SystemRoot%\\system32\\van.dll,-7204");
		rgLS[10]=_T("@shell32.dll,-30574");
		rgLS[11]=_T("@shell32.dll,-30568");
		rgLS[12]=_T("@shell32.dll,-30576");
		rgLS[13]=_T("@shell32.dll,-30570");
		rgLS[14]=_T("@shell32.dll,-30569");
		rgLS[15]=_T("@shell32.dll,-30515");
		rgLS[16]=_T("@shell32.dll,-30489");
		rgLS[17]=_T("@shell32.dll,-30481");
		rgLS[18]=_T("@shell32.dll,-30493");
		rgLS[19]=_T("@shell32.dll,-30483");
		rgLS[20]=_T("@shell32.dll,-30517");
		rgLS[21]=_T("@sud.dll,-1");
		rgLS[22]=_T("@shell32.dll,-30571");
		rgLS[23]=_T("@shell32.dll,-30572");
		rgLS[24]=_T("@shell32.dll,-30497");
	}
	for (int i=0;i<25;i++)   //38
	{
		wcscpy(strRG, _T("\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\StartMenu\\StartPanel\\"));
		wcsncat(strRG, rgCLSID[i].c_str(),128);
		CRegisterWIN32::WriteStrVal(HKEY_LOCAL_MACHINE, strRG, _T("Text"), rgLS[i].c_str());
	}
}

void CRegisterWIN32::InitReg()
{
	TCHAR Dir[MAX_PATH+1];
	getAppDir(Dir,Dir);
    RestoreKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\ShellNoRoam\\MUICache"),StrAppend(Dir, _T("XPerAll.xrg")));
    RestoreKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\Shell\\LocalizedResourceName"), StrAppend(Dir, _T("XPerAcc.xrg")));
	//RestoreKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Classes\\CLSID"), StrAppend(Dir, _T("CLF.xrg")));
	
	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{450D8FBA-AD25-11D0-98A8-0800361B1103}"), _T(""), _T("فايل ها و اسناد"));
    WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{208D2C60-3AEA-1069-A2D7-08002B30309D}"), _T(""), _T("مکان هاي شبکه"));
    WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{20D04FE0-3AEA-1069-A2D8-08002B30309D}"), _T(""), _T("کامپيوتر"));
    WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}"), _T(""), _T("سطل بازيافت"));
    WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{871C5380-42A0-1069-A2EA-08002B30309D}"), _T(""), _T("مرورگر اينترنت"));
    
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\0"), _T("LocalizedString"), _T("براي فايل و پوشه ها..."));
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\0\\HelpText"), _T("LocalizedString"), _T("جستجو براي فايل و پوشه ها"));
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\1"), _T("LocalizedString"), _T("براي کامپيوترها..."));
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\1\\HelpText"), _T("LocalizedString"), _T("جستجو براي کامپيوترهاي تحت شبکه"));
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\2"), _T("LocalizedString"), _T("براي چاپگرها..."));
    WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\2\\HelpText"), _T("LocalizedString"), _T("جستجو براي چاپگرها"));

	RegLocalized(TRUE);
}

void CRegisterWIN32::DelReg()
{
	//TCHAR Dir[MAX_PATH+1];
	//getAppDir(Dir,Dir);
	//RestoreKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Classes\\CLSID"), StrAppend(Dir, _T("CLE.xrg")));
	DeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\ShellNoRoam\\MUICache"));
	DeleteKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\Shell\\LocalizedResourceName"));

	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{450D8FBA-AD25-11D0-98A8-0800361B1103}"), _T(""), _T("Document"));
	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{208D2C60-3AEA-1069-A2D7-08002B30309D}"), _T(""), _T("Network Places"));
	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{20D04FE0-3AEA-1069-A2D8-08002B30309D}"), _T(""), _T("Computer"));
	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{645FF040-5081-101B-9F08-00AA002F954E}"), _T(""), _T("Recycle Bin"));
	WriteStrVal(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CLSID\\{871C5380-42A0-1069-A2EA-08002B30309D}"), _T(""), _T("Internet Explorer"));

	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\0"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23232"));
	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\0\\HelpText"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23296"));
	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\1"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23233"));
	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\1\\HelpText"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23297"));
	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\2"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23234"));
	WriteStrVal(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindExtensions\\Static\\ShellSearch\\2\\HelpText"), _T("LocalizedString"), _T("@%SystemRoot%\\system32\\SHELL32.dll,-23298"));
	
	RegLocalized(FALSE);
}

BOOL CRegisterWIN32::SetPrivilege(LPCTSTR seName, bool enabled)
{
     long p_lngRtn;
     HANDLE p_lngToken;
     DWORD p_lngBufferLen;
     LUID p_typLUID;
     TOKEN_PRIVILEGES p_typTokenPriv;
     TOKEN_PRIVILEGES p_typPrevTokenPriv;
     p_lngRtn = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &p_lngToken);
     
	 if (p_lngRtn == 0)
        return FALSE;
	 else if (GetLastError()!=0)
        return FALSE;

     p_lngRtn = LookupPrivilegeValue(0, seName, &p_typLUID);
     if (p_lngRtn == 0)
        return FALSE;
  
    // Set it up to adjust the program's security privilege.
    p_typTokenPriv.PrivilegeCount = 1;
	if (enabled)
		p_typTokenPriv.Privileges->Attributes = SE_PRIVILEGE_ENABLED;
	else
		p_typTokenPriv.Privileges->Attributes = 0;
    p_typTokenPriv.Privileges->Luid = p_typLUID;
	BOOL ret=AdjustTokenPrivileges(p_lngToken, FALSE, &p_typTokenPriv, sizeof(p_typPrevTokenPriv), &p_typPrevTokenPriv, &p_lngBufferLen);
	return ret;
}