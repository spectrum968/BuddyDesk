#include "stdafx.h"
#include "QuoteSimpleFactory.h"


CQuoteSimpleFactory::CQuoteSimpleFactory(void)
{
}


CQuoteSimpleFactory::~CQuoteSimpleFactory(void)
{
}

IQuote* CQuoteSimpleFactory::CreateQuote(const CString& strSource)
{
	if(strSource.CompareNoCase(_T("Yahoo")) == 0)
	{
		return new CYahooQuote();
	}
	//else if(strSource.CompareNoCase(_T("Tdx")) == 0)
	//{
	//	return new CTDXQuote();
	//}
}
