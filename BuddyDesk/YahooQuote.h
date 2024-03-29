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
	void	UpdateHistory(const CString& strId) {};
private:
	void QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote);
	void QuoteAssembler(const CString& strId, const MarketType eMarket, const QuoteType eQuote, const vector<CString>& vecQuote, CString& strQuote);

private:
	vector<CString> m_vecArray;
};

