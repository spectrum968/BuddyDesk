#pragma once
#include "iquote.h"
class CYahooQuote :
	public IQuote
{
public:
	CYahooQuote(void);
	virtual ~CYahooQuote(void);

	CStringArray& GetHistory(const CString& strSym, const CTime& timeStart, char ch = 'd');
	CStringArray& GetHistory(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch = 'd');
	BOOL GetTime(const CString& arrQuote, CString& strTime);
	BOOL GetHigh(const CString& arrQuote, CString& strHigh);
	BOOL GetLow(const CString& arrQuote, CString& strLow);
	BOOL GetOpen(const CString& arrQuote, CString& strOpen);
	BOOL GetClose(const CString& arrQuote, CString& strClose);
	BOOL GetVolumn(const CString& arrQuote, CString& strVolumn) {return TRUE;}
	BOOL GetAmount(const CString& arrQuote, CString& strAmount){ return TRUE;}

private:
	CStringArray m_strArray;
};

