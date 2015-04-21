#pragma once
#include "iquote.h"
#include <vector>

using namespace std;

class CYahooQuote :
	public IQuoteLoader
{
public:
	CYahooQuote(void);
	virtual ~CYahooQuote(void);

	CString GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum = 90);
	CString GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum = 90);

private:
	void QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote);
	void QuoteAssembler(const CString& strId, char ch, const vector<CString>& vecQuote, CString& strQuote);

private:
	vector<CString> m_vecArray;
};

