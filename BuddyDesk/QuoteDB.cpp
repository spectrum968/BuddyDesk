#include "stdafx.h"
#include "QuoteDB.h"


CQuoteDB::CQuoteDB(void)
{
}


CQuoteDB::~CQuoteDB(void)
{
}

bool CQuoteDB::IsTableExist(const CString& strTableName)
{
	bool bRet = false;
	CString strSQL;
	//select * from sysobjects where id = object_id('BUDDYDB.dbo.KLineData') 
	strSQL.Format(_T("SELECT * FROM sysobjects WHERE id = object_id('%s.dbo.%s') "), GetDB(), strTableName);
	if(!ExecuteSQL(strSQL))
	{
		CString strLog;
		strLog.Format(_T("Query security info failed, SQL: %s"), strSQL);
		CLog::Log(_T("Query security info failed."), strLog);

		return bRet;
	}

	if (!GetRecordSet()->adoEOF)
		bRet = true;
	GetRecordSet()->Close();

	return bRet;
}

bool CQuoteDB::CreateTable(const CString& strTableName)
{
	CString strSQL;
	strSQL.Format(_T("CREATE TABLE [dbo].[%s](	\
	[RowId] [int] IDENTITY(1,1) NOT NULL,				\
	[SecurityCode] [int] NULL,							\
	[DataType] [int] NULL,								\
	[DateTime] [datetime] NULL,							\
	[Open] [float] NULL,								\
	[High] [float] NULL,								\
	[Low] [float] NULL,									\
	[Close] [float] NULL,								\
	[Amount] [float] NULL,								\
	[Volumn] [float] NULL,								\
	 CONSTRAINT [PK_%s] PRIMARY KEY NONCLUSTERED		\
	(														\
		[RowId] ASC											\
	)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]	\
	) ON [PRIMARY]"), strTableName, strTableName);

	if (!ExecuteNoneQuery(strSQL))
	{
		CString strLog;
		strLog.Format(_T("Create table failed, SQL: %s"), strSQL);
		CLog::Log(_T("Create table failed."), strLog);

		return false;
	}

	return true;
}

COleDateTime CQuoteDB::GetLastQuoteTime(const CString strTblName, QuoteType eType)
{
	CString strSQL;
	strSQL.Format(_T("SELECT [DateTime] FROM [%s].dbo.[%s] with(nolock) \
					 WHERE DateTime in (SELECT MAX([DateTime]) \
					 FROM  [%s].dbo.[%s])"), GetDB(), strTblName, GetDB(), strTblName);

	COleDateTime dtTime;
	if(!ExecuteSQL(strSQL))
	{
		CString strLog;
		strLog.Format(_T("Get last quote time failed, SQL: %s"), strSQL);
		CLog::Log(_T("Get last quote time failed."), strLog);

		return dtTime;
	}

	if (!GetRecordSet()->adoEOF)
	{
		GetRecordSet()->MoveFirst();
		dtTime = GetCollectDateTime(_T("DateTime"));
	}
	GetRecordSet()->Close();

	return dtTime;
}
bool CQuoteDB::WriteQuote(const CString& strTableName, const CQuotes& quote)
{
	bool bRet(false), bTableExist(false);
	if (!IsTableExist(strTableName))
		bRet = CreateTable(strTableName);
	else
		bTableExist = true;

	if (bRet||bTableExist)
	{
		vector<CString> vecSQL;
		CString strAlertSQL;
		strAlertSQL.Format(_T("UPDATE [%s].dbo.[%s] SET [DataType]=6"), GetDB(), strTableName);
		vecSQL.push_back(strAlertSQL);

		const vector<Quote> vecQuote = quote.GetQuote();
		vector<Quote>::const_iterator itQuote = vecQuote.begin();
		for ( ; itQuote!=vecQuote.end(); itQuote++)
		{
			CString strSQL;
			strSQL.Format(_T("INSERT INTO [%s].dbo.[%s] (SecurityCode, DataType, [DateTime], [Open], High, Low, [Close], \
							 Amount, Volumn) VALUES (%s, %d, '%s', %f, %f, %f, %f, %f, %f)"), GetDB(), strTableName,
							 quote.GetId(), itQuote->eQuote, itQuote->dtTime.Format(), itQuote->dOpen, itQuote->dHigh,
							 itQuote->dLow, itQuote->dClose, itQuote->dAmount, itQuote->dVolumn);
			vecSQL.push_back(strSQL);
		}
		bRet = ExecuteNoneQuery(vecSQL);
	}

	return bRet;
}
bool CQuoteDB::GetSecurityList(map<int, CString>& mapQuotes)
{
	bool bRet(false);
	CString strSQL;
	strSQL.Format(_T("SELECT [SecurityName], [SecurityCode] FROM [%s].dbo.SecurityData with(nolock)"),
		GetDB());

	if(!ExecuteSQL(strSQL))
	{
		CString strLog;
		strLog.Format(_T("Get last quote time failed, SQL: %s"), strSQL);
		CLog::Log(_T("Get last quote time failed."), strLog);

		return bRet;
	}

	if (GetRecordSet()->GetRecordCount())
	{
		GetRecordSet()->MoveFirst();
		do{
			CString strName = GetCollectString(_T("SecurityName"));
			int nCode = GetCollectLong(_T("SecurityCode"));
			mapQuotes.insert(make_pair(nCode, strName));

			GetRecordSet()->MoveNext();
		}while(!GetRecordSet()->adoEOF);

		bRet = true;
	}

	return bRet;
}