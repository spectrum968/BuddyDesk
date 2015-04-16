#pragma once
#ifndef IDB_H
#define IDB_H
#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#import "c:\program files\common files\system\ado\msado15.dll"  rename("EOF","adoEOF"),rename("BOF","adoBOF") 
using namespace ADODB;
using namespace std;

struct threadparam
{
	_ConnectionPtr pConnection;
	const vector<CString>* pVecStr;
	int begPos;
	int endPos;
	bool bLog;
	CString strConnectString;
	threadparam()
	{
		begPos = 0;
		endPos = 0;
	}
};
class IDBInterface
{
public:
	IDBInterface(int threadnumber = 1):m_conNum(threadnumber)
	{
		//HRESULT res=CoInitialize(0);
		_ASSERT(threadnumber>0 && threadnumber<10);
		HRESULT res=CoInitializeEx(0,COINIT_APARTMENTTHREADED);
		for (int i=0;i<threadnumber;i++)
		{
			_ConnectionPtr ptr;
			ptr.CreateInstance("ADODB.Connection");
			m_vecConnectPtr.push_back(ptr);
		}
		m_pRecordset.CreateInstance("ADODB.Recordset");
	}
	virtual ~IDBInterface(void) 
	{
		DisConnect();
		CoUninitialize();
	}

public:
	virtual bool Connect();
	virtual bool DisConnect();
	virtual bool ExecuteNoneQuery(const CString& strSQL);
	virtual bool ExecuteNoneQuery(const vector<CString>& vecSQL);
	virtual bool ExecuteSQL(const CString& strSQL);
	_RecordsetPtr getRecordset()
	{
		return m_pRecordset;
	}
	void SetDBProperties(const CString& strName, const CString& strValue)
	{
		CString strKey = strName;
		m_dbProperties[strKey] = strValue;
	}
public:
	bool GetProperties(const CString& strName, CString& strValue)
	{
		bool bRet = false;
		CString strKey = strName;
		std::map<CString, CString>::iterator it = m_dbProperties.find(strKey);
		if (it != m_dbProperties.end())
		{
			strValue = it->second;
			bRet = true;
		}
		return bRet;
	}

	_RecordsetPtr GetRecordSet() {return m_pRecordset;}

protected:
	std::vector<_ConnectionPtr> m_vecConnectPtr;
	_RecordsetPtr	m_pRecordset;

private:
	static unsigned __stdcall fun(void*);
	std::map<CString, CString> m_dbProperties;
	CString m_strConnectString;
	int m_conNum;
	static bool bThreadRet;
};

#endif