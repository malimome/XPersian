void DeleteRegistryValue(HKEY lngRootKey,LPCTSTR strRegKeyPath ,LPCTSTR strRegSubKey)
{
    long lngKeyHandle

	if RegKeyExist(lngRootKey, strRegKeyPath)
	{
		m_lngRetVal = RegOpenKey(lngRootKey, strRegKeyPath, lngKeyHandle)
		m_lngRetVal = RegDeleteValue(lngKeyHandle, strRegSubKey)
		m_lngRetVal = RegCloseKey(lngKeyHandle)
	} 
}

 RegKeyExist( lngRootKey  HKEY, _
                                   strRegKeyPath  LPCTSTR)  Boolean
  long lngKeyHandle  
  lngKeyHandle = 0
  m_lngRetVal = RegOpenKey(lngRootKey, strRegKeyPath, lngKeyHandle)
  if lngKeyHandle = 0 
      RegKeyExist = False
  Else
      RegKeyExist = True
  End if
  m_lngRetVal = RegCloseKey(lngKeyHandle)
  
}

void ReadRegValue( lngRootKey  HKEY, _
                                strRegKeyPath  LPCTSTR, _
                                strRegSubKey  LPCTSTR)  Variant
   intPosition  Integer
   lngKeyHandle  long
   lngDataType  long
   lngBufferSize  long
   lngBuffer  long
   strBuffer  LPCTSTR

  lngKeyHandle = 0
  lngBufferSize = 0



  m_lngRetVal = RegOpenKey(lngRootKey, strRegKeyPath, lngKeyHandle)



  if lngKeyHandle = 0 
      ReadRegValue = ""
      m_lngRetVal = RegCloseKey(lngKeyHandle)   
      Exit Function
  End if



  m_lngRetVal = RegQueryValueEx(lngKeyHandle, strRegSubKey, 0&, _
                         lngDataType,  0&, lngBufferSize)



  if lngKeyHandle = 0 
      ReadRegValue = ""
      m_lngRetVal = RegCloseKey(lngKeyHandle)   
      Exit Function
  End if




  Select Case lngDataType
         Case REG_SZ:       
              
              strBuffer = Space(lngBufferSize)
      
              m_lngRetVal = RegQueryValueEx(lngKeyHandle, strRegSubKey, 0&, 0&, _
                                      strBuffer, lngBufferSize)
              
              
              if m_lngRetVal <> ERROR_SUCCESS 
                  ReadRegValue = ""
              Else
                  
                  intPosition = InStr(1, strBuffer, Chr$(0))  
                  if intPosition > 0 
                      
                      ReadRegValue = Left(strBuffer, intPosition - 1)
                  Else
                      
                      ReadRegValue = strBuffer
                  End if
              End if
              
         Case REG_DWORD:    
              m_lngRetVal = RegQueryValueEx(lngKeyHandle, strRegSubKey, 0&, lngDataType, _
                                     lngBuffer, 4&)  
              
              
              if m_lngRetVal <> ERROR_SUCCESS 
                  ReadRegValue = ""
              Else
                  
                  ReadRegValue = lngBuffer
              End if
              
         Case REG_MULTI_SZ
              strBuffer = Space(lngBufferSize)
              m_lngRetVal = RegQueryValueEx(lngKeyHandle, strRegSubKey, 0&, 0&, _
                                      strBuffer, lngBufferSize)
              strBuffer = Left$(strBuffer, InStr(1, strBuffer, Chr$(0), vbTextCompare) - 1)
              ReadRegValue = strBuffer
         
         Case Else:    
              ReadRegValue = ""
  End Select
  m_lngRetVal = RegCloseKey(lngKeyHandle)
}
Public Sub SetRegistryValue( lngRootKey  HKEY,  strRegKeyPath  LPCTSTR, _
                                strRegSubKey  LPCTSTR, varRegData  Variant)
   lngKeyHandle  long
   lngDataType  long
   lngKeyValue  long
   strKeyValue  LPCTSTR



  if IsNumeric(varRegData) 
      lngDataType = REG_DWORD
  Else
      lngDataType = REG_SZ
  End if



  m_lngRetVal = RegCreateKey(lngRootKey, strRegKeyPath, lngKeyHandle)



  Select Case lngDataType
         Case REG_SZ:       
              strKeyValue = Trim(varRegData) & Chr$(0)     
              m_lngRetVal = RegSetValueEx(lngKeyHandle, strRegSubKey, 0&, lngDataType, _
                                           strKeyValue, Len(strKeyValue))
                                   
         Case REG_DWORD:    
              lngKeyValue = CLng(varRegData)
              m_lngRetVal = RegSetValueEx(lngKeyHandle, strRegSubKey, 0&, lngDataType, _
                                          lngKeyValue, 4&)  
                                   
  End Select
  m_lngRetVal = RegCloseKey(lngKeyHandle)
End Sub
void CreateRegistryKey( lngRootKey  HKEY,  strRegKeyPath  LPCTSTR)

   lngKeyHandle  long

  m_lngRetVal = RegCreateKey(lngRootKey, strRegKeyPath, lngKeyHandle)
  
  m_lngRetVal = RegCloseKey(lngKeyHandle)
}
void DeleteRegistryKey( lngRootKey  HKEY, _
                                 strRegKeyPath  LPCTSTR, _
                                 strRegKeyName  LPCTSTR)  Boolean
   lngKeyHandle  long



  DeleteRegistryKey = False



  if RegKeyExist(lngRootKey, strRegKeyPath) 
  
      
      m_lngRetVal = RegOpenKey(lngRootKey, strRegKeyPath, lngKeyHandle)
      
      
      m_lngRetVal = RegDeleteKey(lngKeyHandle, strRegKeyName)
      
      
      if m_lngRetVal = 0  DeleteRegistryKey = True
      
      m_lngRetVal = RegCloseKey(lngKeyHandle)
  End if
  
}
void RestoreKey( sKeyName  LPCTSTR,  sFileName  LPCTSTR, lPredefinedKey  HKEY)  Boolean
    if EnablePrivilege(SE_RESTORE_NAME) = False  Exit Function
     hKey  long, lRetVal  long
    Call RegOpenKey(lPredefinedKey, sKeyName, hKey)  
    
    Call RegRestoreKey(hKey, sFileName, REG_FORCE_RESTORE)
    RegCloseKey hKey 
}
Private Function EnablePrivilege(seName  LPCTSTR)  Boolean
     p_lngRtn  long
     p_lngToken  long
     p_lngBufferLen  long
     p_typLUID  LUID
     p_typTokenPriv  TOKEN_PRIVILEGES
     p_typPrevTokenPriv  TOKEN_PRIVILEGES
    p_lngRtn = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES Or TOKEN_QUERY, p_lngToken)
    if p_lngRtn = 0 
        Exit Function 
    Elseif ERR.LastDllError <> 0 
        Exit Function 
    End if
    p_lngRtn = LookupPrivilegeValue(0&, seName, p_typLUID)  
    if p_lngRtn = 0 
        Exit Function 
    End if
    
    p_typTokenPriv.PrivilegeCount = 1
    p_typTokenPriv.Privileges.Attributes = SE_PRIVILEGE_ENABLED
    p_typTokenPriv.Privileges.pLuid = p_typLUID
    EnablePrivilege = (AdjustTokenPrivileges(p_lngToken, False, p_typTokenPriv, Len(p_typPrevTokenPriv), p_typPrevTokenPriv, p_lngBufferLen) <> 0)
}

