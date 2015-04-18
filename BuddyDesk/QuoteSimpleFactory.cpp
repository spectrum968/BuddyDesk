#include "stdafx.h"
#include "QuoteSimpleFactory.h"


CQuoteLoaderSimpleFactory::CQuoteLoaderSimpleFactory(void)
{
}


CQuoteLoaderSimpleFactory::~CQuoteLoaderSimpleFactory(void)
{
}

IQuoteLoader* CQuoteLoaderSimpleFactory::CreateQuote(const CString& strSource)
{
	if(strSource.CompareNoCase(_T("Yahoo")) == 0)
	{
		return new CYahooQuote();
	}
	else if(strSource.CompareNoCase(_T("Tdx")) == 0)
	{
		return new CTDXQuote();
	}

	return NULL;
}
