#pragma once
#include "iquote.h"
#include <vector>

using namespace std;

class CYahooQuote :
	public IQuote
{
public:
	CYahooQuote(void);
	virtual ~CYahooQuote(void);

	CString GetHistory(const CString& strSym, const CTime& timeStart, char ch = 'd');
	CString GetHistory(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch = 'd');

private:
	void QuoteParser(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch, vector<CString>& vecQuote);
	void QuoteAssembler(const vector<CString>& vecQuote, CString& strQuote);

private:
	vector<CString> m_vecArray;
};

