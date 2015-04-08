#include "stdafx.h"
#include "YahooQuote.h"

#include <afxinet.h>


CYahooQuote::CYahooQuote(void)
{
}


CYahooQuote::~CYahooQuote(void)
{
}

CStringArray& CYahooQuote::GetHistory(const CString& strSym, const CTime& timeStart, char ch)
{
	m_strArray.RemoveAll();

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
			m_strArray.Add(strRead);
		}

		pFile->Close();
		delete pFile;
	}

	session.Close();
	return m_strArray;
}

CStringArray& CYahooQuote::GetHistory(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch)
{
	m_strArray.RemoveAll();

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
			m_strArray.Add(strRead);
		}
		
		pFile->Close();
		delete pFile;
	}
	
	session.Close();
	return m_strArray;
}

BOOL CYahooQuote::GetTime(const CString& arrQuote, CString& strTime)
{
	return AfxExtractSubString(strTime, arrQuote, 0, ',');
}

BOOL CYahooQuote::GetOpen(const CString& arrQuote, CString& strOpen)
{
	return AfxExtractSubString(strOpen, arrQuote, 1, ',');
}

BOOL CYahooQuote::GetHigh(const CString& arrQuote, CString& strHigh)
{
	return AfxExtractSubString(strHigh, arrQuote, 2, ',');
}

BOOL CYahooQuote::GetLow(const CString& arrQuote, CString& strLow)
{
	return AfxExtractSubString(strLow, arrQuote, 3, ',');
}

BOOL CYahooQuote::GetClose(const CString& arrQuote, CString& strClose)
{
	return AfxExtractSubString(strClose, arrQuote, 4, ',');
}
