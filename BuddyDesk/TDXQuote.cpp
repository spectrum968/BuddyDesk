#include "stdafx.h"
#include "TDXQuote.h"
#include "GSetting.h"
#include "CommonTool.h"


CTDXQuote::CTDXQuote(void)
{
}


CTDXQuote::~CTDXQuote(void)
{
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;

	COleDateTime dtEnd;
	dtEnd = COleDateTime::GetCurrentTime();
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

void CTDXQuote::QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote)
{
	vector<CString> vecFiles;
	CString strFolder = CGSetting::GetInstance()->GetString(cst_TDX_PATH);
	CCommonTool::GetTDXFiles(strFolder, strId, eMarket, eQuote, vecFiles);

	return;
}

void CTDXQuote::QuoteAssembler(const CString& strId, const MarketType eMarket, const QuoteType eQuote, const vector<CString>& vecQuote, CString& strQuote)
{
	return;
}
