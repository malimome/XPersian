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
 * SingleInstance.h: Mutex implementation for instance searching.
 * This file is part of the XPersian - XP Persian interface -
*/

#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SingleInstance
{
	DWORD  LastError;
	HANDLE hMutex;
public:
	SingleInstance(char* strMutexName)
	{
		// strMutexName must be unique
		hMutex = CreateMutexA(NULL, FALSE, strMutexName);
		LastError = GetLastError();
	}

	~SingleInstance()
	{
		if(hMutex) {
			CloseHandle(hMutex);
			hMutex = NULL;
		}
	}

	BOOL IsAnotherInstanceRunning() { return (ERROR_ALREADY_EXISTS == LastError); }
};

#endif // SINGLEINSTANCE_H