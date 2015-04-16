#include "StdAfx.h"
#include ".\log.h"
#ifndef _AFX
#include <AtlComTime.h>
#endif

CString CLog::m_strFilePath;
CLitCriticalSection CLog::m_csAccess;

	CString CLog:: m_strOrgFileName;
	CString CLog::m_strOrgFileDir;

CLog::CLog(void)
{
	errcode = 0;
	errtext = _T("");
}

CLog::~CLog(void)
{
	if(m_pFile.m_h != NULL)
	{
		m_pFile.Close();
		m_pFile.m_h = NULL;
	}
}

void CLog::GenRollLogFile(LPCTSTR filename,int nPerMinute,LPCTSTR directory)
{
	CString strDir;
	strDir = directory;
	if(strDir.Right(1) != _T("\\")) strDir+=_T("\\");
	nPerMinute = 1;
	COleDateTime today;
	today = COleDateTime::GetCurrentTime();
	m_strOrgFileName = filename;
	m_strOrgFileDir = directory;
	CLog::m_strFilePath.Format(_T("%s%s_%s.log"),strDir,filename,today.Format(_T("%Y%m%d_%H%M%S")));
}

BOOL CLog::Close() 
{
	if(m_pFile.m_h != NULL) 
	{
		m_pFile.Close();
		m_pFile.m_h = NULL;
		return TRUE;
	} 
	return FALSE;
}

BOOL CLog::Open(LPCTSTR filepath)
{
	HRESULT hr = m_pFile.Create(filepath,GENERIC_WRITE,FILE_SHARE_READ,OPEN_ALWAYS);
	if(hr != S_OK)
	{
		errcode = ::GetLastError();
		LPVOID lpMsgBuf;
		if (FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				errcode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL ))
		{
			errtext = (LPCTSTR)lpMsgBuf;
			LocalFree( lpMsgBuf );
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CLog::WriteLog(LPCTSTR title,long level,DWORD errcode,LPCTSTR errtext,LPCTSTR data)
{
	if(m_pFile.m_h != NULL)
	{
		ULONGLONG nSize = 0;
		m_pFile.GetSize(nSize);
		
		COleDateTime today = COleDateTime::GetCurrentTime();
		if (nSize > C_MaxFileSize)
		{
			if (!CLog::m_strOrgFileName.IsEmpty())
			{
				m_pFile.Close();
				GenRollLogFile(CLog::m_strOrgFileName,5,CLog::m_strOrgFileDir);
				if (!Open(CLog::m_strFilePath))
					return FALSE;
			}
		}

		m_pFile.Seek(0,FILE_END);
		CString cstr;
		cstr.Format(_T("%s level:%d -> title: %s\r\n\terror code: %lu\terror text: %s\r\n"),today.Format(_T("%Y-%m-%d %H:%M:%S")),level,title,errcode,errtext);
		CStringA strA;
		strA = CT2AEX<>(cstr, CP_UTF8);
		DWORD dw = 0;
		m_pFile.Write(strA.GetBuffer(),strA.GetLength());
		if(data!=NULL)
		{
			m_pFile.Write("//--data-begin--------------\r\n",30);
			strA = CT2AEX<>(data, CP_UTF8);
			m_pFile.Write(strA.GetBuffer(),strA.GetLength());
			m_pFile.Write("\r\n//--data-end----------------\r\n",32);
		}
		m_pFile.Write("\r\n",2);
		return TRUE;
	}
	return FALSE;
}

void CLog::Log(LPCTSTR filename,LPCTSTR title,long level,DWORD errcode,LPCTSTR errtext,LPCTSTR data)
{
	csLock myLock(&CLog::m_csAccess,TRUE);
	CLog myLog;
	CString filepath;

	CString strTempDir;
	DWORD nRet = ::GetEnvironmentVariable(_T("TempDir"),strTempDir.GetBuffer(MAX_PATH),MAX_PATH);
	strTempDir.ReleaseBuffer(nRet);

#ifdef _WINDLL
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			filepath = CLog::m_strFilePath;
		}
	}
	else
	{
		filepath = filename;
	}
#else
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			if(m_strFilePath.Find(_T(":"))>0)
				filepath = m_strFilePath;
			else
				filepath.AppendFormat(_T("%s%s"),(LPCTSTR)strTempDir,m_strFilePath);
		}
	}
	else
	{
		filepath.AppendFormat(_T("%s%s"),(LPCTSTR)strTempDir,m_strFilePath);
	}
#endif
	if(myLog.Open(filepath))
	{
		myLog.WriteLog(title,level,errcode,errtext,data);
		myLog.Close();
	}
}

void CLog::LogArg(LPCTSTR filename,LPCTSTR title,LPCTSTR strFormat,va_list &args)
{
	csLock myLock(&CLog::m_csAccess,TRUE);
	CString strData;
	strData.FormatV( strFormat, args );
	CLog myLog;
	CString filepath;
	CString strTempDir;
	DWORD nRet = ::GetEnvironmentVariable(_T("TempDir"),strTempDir.GetBuffer(MAX_PATH),MAX_PATH);
	strTempDir.ReleaseBuffer(nRet);

#ifdef _WINDLL
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			filepath = CLog::m_strFilePath;
		}
	}
	else
	{
		filepath = filename;
	}
#else
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			if(m_strFilePath.Find(_T(":"))>0)
				filepath = m_strFilePath;
			else
				filepath.AppendFormat(_T("%s%s"),strTempDir,m_strFilePath);
		}
	}
	else
	{
		filepath.AppendFormat(_T("%s%s"),strTempDir,m_strFilePath);
	}
#endif
	if(myLog.Open(filepath))
	{
		myLog.WriteLog(title,strData);
		myLog.Close();
	}
}

void CLog::LogExt(LPCTSTR filename,LPCTSTR title,LPCTSTR data)
{
	csLock myLock(&CLog::m_csAccess,TRUE);
	CLog myLog;
	CString filepath;
	CString strTempDir;
	DWORD nRet = ::GetEnvironmentVariable(_T("TempDir"),strTempDir.GetBuffer(MAX_PATH),MAX_PATH);
	strTempDir.ReleaseBuffer(nRet);
#ifdef _WINDLL
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			filepath = CLog::m_strFilePath;
		}
	}
	else
	{
		filepath = filename;
	}
#else
	if(filename == NULL)
	{
		if(CLog::m_strFilePath.IsEmpty())
		{
			return;
		}
		else
		{
			if(m_strFilePath.Find(_T(":"))>0)
				filepath = m_strFilePath;
			else
				filepath.AppendFormat(_T("%s%s"),strTempDir,m_strFilePath);
		}
	}
	else
	{
		filepath.AppendFormat(_T("%s%s"),strTempDir,m_strFilePath);
	}
#endif
	if(myLog.Open(filepath))
	{
		myLog.WriteLog(title,data);
		myLog.Close();
	}
}


BOOL CLog::WriteLog(LPCTSTR title,LPCTSTR data)
{
	if (m_pFile.m_h != NULL)
	{

		//COleDateTime today = COleDateTime::GetCurrentTime();
		//SYSTEMTIME stTime;
		//today.GetAsSystemTime(stTime);
				ULONGLONG nSize = 0;
		m_pFile.GetSize(nSize);
		COleDateTime today = COleDateTime::GetCurrentTime();
		if (nSize > C_MaxFileSize)
		{
			if (!CLog::m_strOrgFileName.IsEmpty())
			{
				m_pFile.Close();
				GenRollLogFile(CLog::m_strOrgFileName,5,CLog::m_strOrgFileDir);
				if (!Open(CLog::m_strFilePath))
					return FALSE;
			}
		}

		m_pFile.Seek(0,FILE_END);
		SYSTEMTIME stm;

		GetLocalTime(&stm);
		CString cstr;
		if(data)
		{
			cstr.Format(_T("%d-%d-%d %d:%d:%d:%d \ttitle: %s \t\tdata: %s\r\n\r\n"),stm.wYear,stm.wMonth,stm.wDay,
           stm.wHour,stm.wMinute,stm.wSecond,stm.wMilliseconds,title,data);
		}
		else
		{
			cstr.Format(_T("%d-%d-%d %d:%d:%d:%d \ttitle: %s\r\n"),stm.wYear,stm.wMonth,stm.wDay,
           stm.wHour,stm.wMinute,stm.wSecond,stm.wMilliseconds,title);
		}
		CStringA strA;
		strA = CT2AEX<>(cstr, CP_UTF8);
		m_pFile.Write(strA.GetBuffer(),strA.GetLength());
		return TRUE;
	}
	return FALSE;
}

//Dump data to an independent file
void CLog::Dump(LPCTSTR filename, LPCVOID pBuffer, DWORD nBufSize)
{
	csLock myLock(&CLog::m_csAccess,TRUE);
	CLog myLog;

	if(filename == NULL)
		return;

	CAtlFile dumpfile;

	HRESULT hr = dumpfile.Create(filename,GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS);
	if(hr == S_OK)
	{
		dumpfile.Write(pBuffer, nBufSize);
		dumpfile.Close();
	}
}

bool CLog::Init(LPCTSTR strModuleName)
{
	CString strModule = strModuleName;
	if(strModule.IsEmpty())
		return false;

	CString cstr;
	DWORD nRet = GetEnvironmentVariable(strModule + _T("Log"),cstr.GetBuffer(MAX_PATH),MAX_PATH);
	cstr.ReleaseBuffer(nRet);

	if(!cstr.IsEmpty())
	{
		if(CLog::GetDefaultLogFile().IsEmpty())
			CLog::GenRollLogFile(strModule,1,cstr);

		CLog::LogExt(NULL,strModule + _T(" DLL Load."));
		return true;
	}
	return false;
}
