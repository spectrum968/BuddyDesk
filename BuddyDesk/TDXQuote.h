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
	void QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, map<CString, vector<TdxQuote>>& vecQuote);
	void QuoteAssembler(const map<CString, vector<TdxQuote>>& mapQuote, CString& strQuote);
	void ParseTDXFile(const vector<CString>& vecFiles, map<CString, vector<TdxQuote>>& mapQuotes);

	bool GetTDXFiles(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote, vector<CString>& vecFiles);
	vector<CString> GetTDXDataFileName(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote);
	CString GetTDXMarketFolderName(MarketType eMarket);
	CString GetTDXQuoteFolderName(QuoteType eQuote);
	CString GetTDXQuoteFileName(QuoteType eQuote);
	CString GetCodeFromTDXFileName(const CString& strFile);
	MarketType GetMarketTypeFromTDXFileName(const CString& strFile);
	QuoteType GetQuoteTypeFromTDXFileName(const CString& strFile);
	COleDateTime GetDateFromTDXQuote(const TdxQuote& quote);
	double GetOpenFromTdxQuote(const TdxQuote& quote);
	double GetHighFromTdxQuote(const TdxQuote& quote);
	double GetLowFromTdxQuote(const TdxQuote& quote);
	double GetCloseFromTdxQuote(const TdxQuote& quote);
	double GetAmountFromTdxQuote(const TdxQuote& quote);
	double GetVolumnFromTdxQuote(const TdxQuote& quote);
};

