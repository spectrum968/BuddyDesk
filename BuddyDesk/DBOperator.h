#pragma once
#include "stdafx.h"
#include "idbinterface.h"
#include <vector>

using namespace std;

static const int ctn_MAX_DB_RECONNECT = 3;

class CDBOperator :
	public IDBInterface
{
public:
	CDBOperator();
	~CDBOperator(void);
public:
	bool Initial(const CString& strConnectString);
	bool Initial(const CString& strServer, const CString& strDB);
	bool IsConnected() {return m_bConnected;}

protected:
	CString GetConnString(const CString& strServer, const CString& strDB);
	int GetReconnNum() {return m_nReconnNum;}
	CString GetCollectString(const variant_t index) {return (_bstr_t)GetRecordSet()->GetCollect(index);}
	long GetCollectLong(const variant_t index) {return (long)GetRecordSet()->GetCollect(index);}
	COleDateTime GetCollectDateTime(const variant_t index) 
	{
		variant_t vDateTime = GetRecordSet()->GetCollect(index);
		::VariantChangeType(&vDateTime,&vDateTime,0,VT_DATE);
		return vDateTime.date;
	}

	virtual bool ExecuteSQL(const CString& strSQL);
	virtual bool ExecuteNoneQuery(const vector<CString>& vecSQL);
	virtual bool ExecuteNoneQuery(const CString& strSQL);

	CString GetServer() {return m_strServer;}
	CString GetDB() {return m_strDB;}

private:
	CString m_strServer;
	CString m_strDB;
	int m_nReconnNum;
	bool m_bConnected;
};

