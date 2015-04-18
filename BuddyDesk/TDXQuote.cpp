#include "stdafx.h"
#include "TDXQuote.h"


CTDXQuote::CTDXQuote(void)
{
}


CTDXQuote::~CTDXQuote(void)
{
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, char ch, long lNum)
{
	vector<CString> vecQuotes;

	COleDateTime dtEnd;
	dtEnd = COleDateTime::GetCurrentTime();
	QuoteParser(strId, dtStart, dtEnd, ch, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, ch, vecQuotes, strQuote);

	return strQuote;
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch, long lNum)
{
	vector<CString> vecQuotes;
	QuoteParser(strId, dtStart, dtEnd, ch, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, ch, vecQuotes, strQuote);

	return strQuote;
}

void CTDXQuote::QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch, long lNum, vector<CString>& vecQuote)
{
	return;
}

void CTDXQuote::QuoteAssembler(const CString& strId, char ch, const vector<CString>& vecQuote, CString& strQuote)
{
	return;
}

bool CTDXQuote::GetFileList(const CString& strTdxFolder, vector<CString>& vecFiles)
{
	
}
