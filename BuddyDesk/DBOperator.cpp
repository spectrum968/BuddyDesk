#include "stdafx.h"
#include "DBOperator.h"
#include "Util_String.h"
#include "Log.h"

using namespace Buddy::Util;

CDBOperator::CDBOperator()
{
}
CDBOperator::~CDBOperator(void)
{
	return;
}
bool CDBOperator::Initial(const CString& strConnectString)
{
	SetDBProperties(_T("connectstring"), strConnectString);
	m_nReconnNum = 0;
	m_bConnected = false;

	if (m_bConnected)
		return true;

	if (m_nReconnNum >= ctn_MAX_DB_RECONNECT)
		return false;

	if ( Connect() )
	{
		m_bConnected = true;
		return true;
	}
	else
	{
		m_bConnected = false;
		m_nReconnNum++;
		return false;
	}
}
bool CDBOperator::Initial(const CString& strServer, const CString& strDB)
{
	m_strServer = strServer;
	m_strDB = strDB;
	CString strConnectString = GetConnString(strServer, strDB);
	SetDBProperties(_T("connectstring"), strConnectString);
	m_nReconnNum = 0;
	m_bConnected = false;

	if (m_bConnected)
		return true;

	if (m_nReconnNum >= ctn_MAX_DB_RECONNECT)
		return false;

	if ( Connect() )
	{
		m_bConnected = true;
		return true;
	}
	else
	{
		m_bConnected = false;
		m_nReconnNum++;
		return false;
	}
}
CString CDBOperator::GetConnString(const CString& strServer, const CString& strDB)
{
	if (strServer.IsEmpty() || strDB.IsEmpty())
	{
		CString strLog;
		strLog.Format(_T("Enter param error. Server: %s, DB: %s"), strServer, strDB);
		CLog::Log(_T("CDBOperator"), strLog);
	}
	CString strConnStr;
	strConnStr.Format(_T("Provider=sqloledb;Data Source=%s;Initial Catalog=%s;Integrated Security=SSPI;"), strServer, strDB);

	return strConnStr;
}
bool CDBOperator::ExecuteSQL(const CString& strSQL)
{
	bool bResult = true;
	try
	{
		bResult = IDBInterface::ExecuteSQL(strSQL);
	}
	catch( ... )
	{
		CString strLog;
		strLog.Format(_T("ExecuteSQL exception: SQL: %s"), strSQL);
		CLog::Log(_T("CDBOperator"), strLog);
		bResult = false;
	}

	return bResult;
}
bool CDBOperator::ExecuteNoneQuery(const vector<CString>& vecSQL)
{
	bool bResult = true;

	try
	{
		bResult = IDBInterface::ExecuteNoneQuery(vecSQL);
	}
	catch( ... )
	{
		CString strLog = _T("ExecuteSQL exception: SQL: ");
		for (int i = 0; i < vecSQL.size(); i++)
		{
			strLog.AppendFormat(_T("%s\n"), vecSQL.at(i));
		}
		
		CLog::Log(_T("CDBOperator"), strLog);
		bResult = false;
	}
	return bResult;
}
bool CDBOperator::ExecuteNoneQuery(const CString& strSQL)
{
	bool bResult = true;
	try
	{
		bResult = IDBInterface::ExecuteNoneQuery(strSQL);
	}
	catch( ... )
	{
		CString strLog;
		strLog.Format(_T("ExecuteNoneQuery exception: SQL: %s"), strSQL);
		CLog::Log(_T("CDBOperator"), strLog);
		bResult = false;
	}

	return bResult;
}