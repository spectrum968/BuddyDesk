#pragma once
#include "stdafx.h"

interface IQuoteLoader
{
public:
	virtual ~IQuoteLoader(void)=0{};

	virtual CString GetHistory(const CString& strSecId, const COleDateTime& tStart, const MarketType eMarket, const QuoteType eQuote, long lNum = 90) = 0;
	virtual CString GetHistory(const CString& strSecId, const COleDateTime& tStart, const COleDateTime& tEnd, const MarketType eMarket, const QuoteType eQuote, long lNum = 90) = 0;
};

