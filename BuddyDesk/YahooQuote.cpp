#include "stdafx.h"
#include "YahooQuote.h"

#include <afxinet.h>

CYahooQuote::CYahooQuote(void)
{
}


CYahooQuote::~CYahooQuote(void)
{
}

CString CYahooQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;

	COleDateTime dtEnd;
	dtEnd = COleDateTime::GetCurrentTime();
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

CString CYahooQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

void CYahooQuote::QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote)
{
	// format URL to pass to GetHttpConnection.
	CString strURL;
	char ch='d';
	strURL.Format(_T("http://ichart.finance.yahoo.com/table.csv?s=%s&a=%.2i&b=%i&c=%i&d=%.2i&e=%i&f=%i&g=%c"),
		strId,
		dtStart.GetMonth(),
		dtStart.GetDay(),
		dtStart.GetYear(),
		dtEnd.GetMonth(),
		dtEnd.GetDay(),
		dtEnd.GetYear(),
		ch);

	CInternetSession session;
	CStdioFile* pFile = session.OpenURL(strURL);
	
	if (pFile)
	{
		CString strRead;
		while (pFile->ReadString(strRead))
		{
#ifdef _UNICODE
			strRead = CString((LPCSTR)(LPCTSTR)strRead);
#endif
			vecQuote.push_back(strRead);
		}
		
		pFile->Close();
		delete pFile;
	}
	
	session.Close();

	return;
}

void CYahooQuote::QuoteAssembler(const CString& strId, const MarketType eMarket, const QuoteType eQuote, const vector<CString>& vecQuote, CString& strQuote)
{
	char ch='d';
	CString strDate, strOpen, strHigh, strLow, strClose;
	CString strRet(_T("<buddy>"));
	strRet.AppendFormat(_T("<s code=\"%s\" market=\"0\" type=\"-1\">"), strId);
	bool bHead = true;

	vector<CString>::const_iterator itQuote = vecQuote.begin();
	for ( ; itQuote!=vecQuote.end(); itQuote++)
	{
		if(bHead)
		{
			bHead = false;
			continue;
		}

		vector<CString> vecTemp;
		CStrUtil::SplitString(*itQuote, vecTemp);
		strRet.AppendFormat(_T("<q t=\"%s\" o=\"%s\" h=\"%s\" l=\"%s\" c=\"%s\" />"),
			vecTemp[0], vecTemp[1], vecTemp[2], vecTemp[3], vecTemp[4]);
		
	}

	strRet.Append(_T("</s></buddy>"));

	strQuote = strRet;

	return;
}
