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

	CString GetHistory(const CString& strId, const CTime& timeStart, char ch = 'd', long lNum = 90);
	CString GetHistory(const CString& strId, const CTime& timeStart, const CTime& timeEnd, char ch = 'd', long lNum = 90);

private:
	void QuoteParser(const CString& strId, const CTime& timeStart, const CTime& timeEnd, char ch, long lNum, vector<CString>& vecQuote);
	void QuoteAssembler(const CString& strId, const vector<CString>& vecQuote, CString& strQuote);

private:
	vector<CString> m_vecArray;
};

