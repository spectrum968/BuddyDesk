#pragma once
#include "IQuote.h"
#include "YahooQuote.h"
#include "TDXQuote.h"

class CQuoteSimpleFactory
{
public:
	CQuoteSimpleFactory(void);
	~CQuoteSimpleFactory(void);

public:
	static IQuote* CreateQuote(const CString& strSource);
};

