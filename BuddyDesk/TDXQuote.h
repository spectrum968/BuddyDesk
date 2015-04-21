#pragma once
#include "iquote.h"
#include <vector>

using namespace std;

struct TDXQuote
{
	CString m_strId;
	COleDateTime m_dtTime;
	
}

class CTDXQuote :
	public IQuoteLoader
{
public:
	CTDXQuote(void);
	virtual ~CTDXQuote(void);

	CString GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum = 90);
	CString GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum = 90);

private:
	void QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote);
	void QuoteAssembler(const CString& strId, const MarketType eMarket, const QuoteType eQuote, const vector<CString>& vecQuote, CString& strQuote);
	void ParseTDXFile(const vector<CString>& vecFiles, CString& strQuote);
};

