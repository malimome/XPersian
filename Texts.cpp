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
 * Texts.cpp: Loading texts and mapping text IDs.
 * This file is part of the XPersian - XP Persian interface -
*/

#include "Texts.h"

#define NUMBER_ALL		49
#define NUMBER_MISC		20
#define NUMBER_MISC2	7

CTranslator::CTranslator()
{
	//cap_size=Max_st_cap;
	Init_Caption();
}

CTranslator::~CTranslator()
{
	EngCaption.clear();
	PerCaption.clear();
}

void CTranslator::unload_list()
{
	EngTexts.clear();
	PerTexts.clear();
}

void CTranslator::Init_Caption()
{
	Load_Captions(EngCaption,PerCaption);
}

int CTranslator::mapnum2id(int arrnum)
{
	if (arrnum >= 0 && (arrnum <= (NUMBER_MISC-1)) )
		return MISC;
	else if ((arrnum >= NUMBER_MISC)  && (arrnum <= (NUMBER_MISC + NUMBER_MISC2 -1)) )
		return SFSDSK;
	else if (arrnum == NUMBER_ALL-1)
		return PROPERTIES;
	else
		return (arrnum - NUMBER_MISC - NUMBER_MISC2);
}

void CTranslator::text_from_arrnum(int arrnum)
{
	int cpid=0;

	cpid=mapnum2id(arrnum);

	switch (cpid)
	{
	case AUDIO_PROP :
		Load_Audio(EngTexts,PerTexts);
		break;
	case CUSTOMIZE :
		Load_CUSTOMIZE(EngTexts,PerTexts);
		break;
	case HARDWARE :
		Load_HARDWARE(EngTexts,PerTexts);
		break;
	case PRINTER :
		Load_PRINTER(EngTexts,PerTexts);
		break;
	case PROPERTIES :
		Load_PROPERTIES(EngTexts,PerTexts);
		break;
	case ACCESS_OPT :
		Load_ACCESS(EngTexts,PerTexts);
		break;
	case CD_WRITE :
		Load_CD(EngTexts,PerTexts);
		break;
	case CONFIRM :
		Load_CONFIRM(EngTexts,PerTexts);
		break;
	case DISK_CLEAN :
		Load_DISK(EngTexts,PerTexts);
		break;
	case DISP_PROP :
		Load_DISP(EngTexts,PerTexts);
		break;
	case FOLDER_OPT :
		Load_FOLDER(EngTexts,PerTexts);
		break;
	case INTERNET_PROP :
		Load_INTERNET(EngTexts,PerTexts);
		break;
	case MOUSE_PROP :
		Load_MOUSE(EngTexts,PerTexts);
		break;
	case CONNECT_WIZ :
		Load_CONNECT(EngTexts,PerTexts);
		break;
	case MODEM_OPT :
		Load_MODEM(EngTexts,PerTexts);
		break;
	case POWER_OPT :
		Load_POWER(EngTexts,PerTexts);
		break;
	case LANGUAGE_OPT :
		Load_LANGUAGE(EngTexts,PerTexts);
		break;
	case SEARCH_RES :
		Load_SEARCH(EngTexts,PerTexts);
		break;
	case SYSTEM_PROP :
		Load_SYSTEM(EngTexts,PerTexts);
		break;
	case FIREWALL :
		Load_FIREWALL(EngTexts,PerTexts);
		break;
	case SFSDSK :
		Load_SFSDSK(EngTexts,PerTexts);
		break;
	case MISC :
		Load_MISC(EngTexts,PerTexts);
		break;
	default :
		break;
	}
	  EngTexts.push_back("&Browse...");
      PerTexts.push_back(_T("..مکان‌ياب"));
      EngTexts.push_back("&Yes");
      PerTexts.push_back(_T("بلي"));
      EngTexts.push_back("&No");
      PerTexts.push_back(_T("خير"));
      EngTexts.push_back("OK");
      PerTexts.push_back(_T("تاييد"));
      EngTexts.push_back("Cancel");
      PerTexts.push_back(_T("انصراف"));
      EngTexts.push_back("&Apply");
      PerTexts.push_back(_T("اعمال تغيير"));
      EngTexts.push_back("&Back");
      PerTexts.push_back(_T("بازگشت"));
      EngTexts.push_back("&Next");
      PerTexts.push_back(_T("بعدي"));

	  text_size=EngTexts.size();
}