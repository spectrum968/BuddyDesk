#pragma once
#include "IQuote.h"
#include "YahooQuote.h"
#include "TDXQuote.h"

class CQuoteLoaderSimpleFactory
{
public:
	CQuoteLoaderSimpleFactory(void);
	~CQuoteLoaderSimpleFactory(void);

public:
	static IQuoteLoader* CreateQuote(const CString& strSource);
};

