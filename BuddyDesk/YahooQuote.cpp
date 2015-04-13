#include "stdafx.h"
#include "YahooQuote.h"

#include <afxinet.h>


CYahooQuote::CYahooQuote(void)
{
}


CYahooQuote::~CYahooQuote(void)
{
}

CString CYahooQuote::GetHistory(const CString& strSym, const CTime& timeStart, char ch)
{
	m_vecArray.clear();

	CString strUrl;
	strUrl.Format(_T("http://ichart.finance.yahoo.com/table.csv?s=%s&a=%.2i&b=%i&c=%i&g=%c"),
		strSym,
		timeStart.GetMonth()-1,
		timeStart.GetDay(),
		timeStart.GetYear(),
		ch);

	CInternetSession session;
	CStdioFile* pFile = session.OpenURL(strUrl);

	if (pFile)
	{
		CString strRead;
		while (pFile->ReadString(strRead))
		{
#ifdef _UNICODE
			strRead = CString((LPCSTR)(LPCTSTR)strRead);
#endif
			m_vecArray.push_back(strRead);
		}

		pFile->Close();
		delete pFile;
	}

	session.Close();
	return _T("");
}

CString CYahooQuote::GetHistory(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch)
{
	m_vecArray.clear();


	return _T("");
}

void CYahooQuote::QuoteParser(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch, vector<CString>& vecQuote)
{
	// format URL to pass to GetHttpConnection.
	CString strURL;

	strURL.Format(_T("http://ichart.finance.yahoo.com/table.csv?s=%s&a=%.2i&b=%i&c=%i&d=%.2i&e=%i&f=%i&g=%c"),
		strSym,
		timeStart.GetMonth()-1,
		timeStart.GetDay(),
		timeStart.GetYear(),
		timeEnd.GetMonth()-1,
		timeEnd.GetDay(),
		timeEnd.GetYear(),
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

void CYahooQuote::QuoteAssembler(const vector<CString>& vecQuote, CString& strQuote)
{
	
}
