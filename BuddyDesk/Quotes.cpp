#include "stdafx.h"
#include "Quotes.h"


CQuotes::CQuotes(void)
{
	m_strId = _T("");
	m_eStick = StickType::OriginStick;
}


CQuotes::~CQuotes(void)
{
}

void CQuotes::Init(const CString& strXml)
{
	try
	{
		CXmlDocument doc;
		doc.LoadXml(strXml);

		CXmlNodeList secList = doc.Find(_T("/root/sec"));
		for (int i = 0; i < secList.GetLength(); i++)
		{
			CXmlNode ndSec = secList.GetItem(i);
			m_strId = ndSec.GetAttribute(_T("id"));
			m_eStick = StickType::OriginStick;

			CXmlNodeList quoteList = ndSec.Find(_T("quote"));
			for (int j = 0; j < quoteList.GetLength(); j++)
			{
				Quote quote;
				CXmlNode ndQuote = quoteList.GetItem(i);
				quote.dtTime = ndQuote.GetAttributeDateTime(_T("time"), quote.dtTime);
				quote.eQuote = GetQuoteType(ndQuote.GetAttribute(_T("type")));
				quote.dOpen = ndQuote.GetAttributeDouble(_T("open"));
				quote.dHigh = ndQuote.GetAttributeDouble(_T("high"));
				quote.dLow = ndQuote.GetAttributeDouble(_T("low"));
				quote.dClose = ndQuote.GetAttributeDouble(_T("close"));
				quote.dVolumn = ndQuote.GetAttributeDouble(_T("volumn"));
				quote.dAmount = ndQuote.GetAttributeDouble(_T("amount"));

				m_vecQuote.push_back(quote);
			}
		}
	}
	catch( ... )
	{
		OutputDebugString(_T("parse quote failed."));
	}

	return;
	
}
QuoteType CQuotes::GetQuoteType(const CString& strType)
{
	if (strType.CompareNoCase(_T("d")) == 0)
		return QuoteType::Day;

	return QuoteType::Undefined;
}
