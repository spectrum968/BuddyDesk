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

bool CCommonTool::GetFiles(const CString& strFolder, const CString& strId, vector<CString>& vecFiles, char ch)
{
	CString strSHDay = strFolder+_T("\\")+cst_DATA_FOLDER+_T("\\")+cst_SH+_T("\\")+ ch=='d' ? cst_DAY_FOLDER : ;
	CString strSZDay = strFolder

	try
	{
		CFileFind FileFinder;
		bool bFound = (FileFinder.FindFile(strFileName)==TRUE);
		if (!bFound)
			return false;
		while (bFound)  
		{  
			bFound = (FileFinder.FindNextFile()==TRUE); 
			CString strDelFilePath  = FileFinder.GetFilePath();  
			vecFiles.push_back(strDelFilePath);
		}  
		FileFinder.Close();
	}
	catch( ... )
	{
		OutputDebugString(_T("Fail to get file"));
		return false;
	}

	return true;
}
vector<CString> CCommonTool::GetDataFileName(const CString& strFolder, const CString& strId, char ch)
{
	CString strFilePath;
	strFilePath.Format(_T("%s\\%s\\%s\\%s\\%s\\*%s"), strFolder, cst_DATA_FOLDER, );
}
