#include "stdafx.h"
#include "QuoteWapper.h"
#include <thread>
#include "QuoteSimpleFactory.h"
#include "Quotes.h"

using namespace std;

struct ParserParam
{
	ParserParam(const CString& strCode, const COleDateTime& dtStart, const COleDateTime& dtEnd, 
		const MarketType eMarket, const QuoteType eQuote, long lNum)
	{
		m_strCode = strCode;
		m_dtStart = dtStart;
		m_dtEnd = dtEnd;
		m_eMarket = eMarket;
		m_eQuote = eQuote;
		m_lNum = lNum;
	}
	CString m_strCode;
	COleDateTime m_dtStart;
	COleDateTime m_dtEnd;
	MarketType m_eMarket;
	QuoteType m_eQuote;
	long m_lNum;
};

void thread_task(const ParserParam& param)
{
	IQuoteLoader* pQuoteLoader = CQuoteLoaderSimpleFactory::CreateQuote(_T("TDX"));
	pQuoteLoader->UpdateHistory(param.m_strCode);
	delete pQuoteLoader;
	pQuoteLoader = NULL;
}


CQuoteWapper::CQuoteWapper(void)
{
}


CQuoteWapper::~CQuoteWapper(void)
{
}

void CQuoteWapper::ParseQuote(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	ParserParam param = ParserParam(strId, dtStart, dtEnd, eMarket, eQuote, lNum);
	thread t(thread_task, param);
	t.join();

	return;
}
