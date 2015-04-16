#pragma once
#include <AtlFile.h>
#include <ATLComTime.h>
#include <atlbase.h>
#include <atlcore.h>
#include "LitMT.h"
const int C_MaxFileSize = 10485760;//10M
using namespace Buddy;
class CLog
{
public:
	CLog(void);
	virtual ~CLog(void);
	
	BOOL WriteLog(LPCTSTR title,long level,DWORD errcode,LPCTSTR errtext,LPCTSTR data);
	BOOL WriteLog(LPCTSTR title,LPCTSTR data);

	BOOL Open(LPCTSTR filepath);
	BOOL Close();
	DWORD errcode;
	CString errtext;

	static void Log(LPCTSTR title){return Log(NULL,title,0,0,_T(""),_T(""));}

	static void Log(LPCTSTR filename,LPCTSTR title,long level,DWORD errcode,LPCTSTR errtext,LPCTSTR data);
	static void Log(LPCTSTR filename,LPCTSTR title,DWORD errcode,LPCTSTR errtext) {return Log(filename,title,0,errcode,errtext,NULL);}
	static void Log(LPCTSTR filename,LPCTSTR title,LPCTSTR data) {return Log(filename,title,0,0,_T(""),data);}
	static void Log(LPCTSTR title,LPCTSTR data) {return Log(NULL,title,0,0,_T(""),data);}
	static void Log(LPCTSTR filename,DWORD errcode,LPCTSTR errtext) {return Log(filename,_T("generic"),0,errcode,errtext,NULL);}
	static void LogExt(LPCTSTR filename,LPCTSTR title,LPCTSTR data=NULL);
	static void LogArg(LPCTSTR filename,LPCTSTR title,LPCTSTR strFormat,va_list &args );

	static void SetDefaultLogFile(LPCTSTR filename) {m_strFilePath = filename;}
	
	static CString& GetDefaultLogFile() {return m_strFilePath;}
	static void GenRollLogFile(LPCTSTR filename,int nPerMinute = 5,LPCTSTR directory = _T("c:\\temp\\"));
	static bool Init(LPCTSTR strModuleName);
	static bool IsOpenLog(){ return !m_strFilePath.IsEmpty(); }
  //Dump data to an independent file 
 static void Dump(LPCTSTR filename, LPCVOID pBuffer, DWORD nBufSize);
	static CString m_strFilePath;
	static CString m_strOrgFileName;
	static CString m_strOrgFileDir;
protected:
	CAtlFile m_pFile;
	static CLitCriticalSection m_csAccess;

	typedef CLitSingleLock csLock;
	

};
