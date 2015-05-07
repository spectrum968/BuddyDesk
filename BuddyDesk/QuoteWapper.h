#pragma once
#include "stdafx.h"
class CQuoteWapper
{
public:
	CQuoteWapper(void);
	~CQuoteWapper(void);
public:
	static void ParseQuote(HWND hWnd, const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum = 90);
};

