#include "stdafx.h"
#include "CommonTool.h"

#include <fstream>
#include <windows.h>

CCommonTool::CCommonTool(void)
{
}


CCommonTool::~CCommonTool(void)
{
}

bool CCommonTool::GetFiles(const CString& strFolder, vector<CString>& vecFiles)
{
	
	CString strFolderName = strFolder;
	HANDLE hFile = 0;
	WIN32_FIND_DATA fileData;
	char line[1024];
	wchar_t fn[1000];
	try
	{
		mbstowcs(fn,(const char*)strFolderName.GetBuffer(), 999);
		hFile = FindFirstFile(fn, &fileData);
		FindNextFile(hFile, &fileData);
		while (FindNextFile(hFile, &fileData))
		{
			wcstombs(line, (const wchar_t*)fileData.cFileName, 259);
			vecFiles.push_back(CString(line));
		}
	}
	catch( ... )
	{
		OutputDebugString(_T("Fail to get file"));
		return false;
	}

	return true;
}
