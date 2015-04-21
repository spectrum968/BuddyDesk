#pragma once
#include "iquote.h"
#include <vector>
#include <map>

using namespace std;

struct TdxQuote
{
	int date;
	int open;
	int high;
	int low;
	int close;
	int amount;
	int volumn;
	int reservation;
};

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
	void ParseTDXFile(const vector<CString>& vecFiles, map<CString, vector<TdxQuote>>& mapQuotes);
};

