#include "stdafx.h"
#include "Quotes.h"


CQuotes::CQuotes(void)
{
	m_strId = _T("");
	m_eMarket = MarketType::Unknown;
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

		CXmlNodeList secList = doc.Select(_T("/buddy/s"));
		for (int i = 0; i < secList.GetLength(); i++)
		{
			CXmlNode ndSec = secList.GetItem(i);
			m_strId = ndSec.GetAttribute(_T("code"));
			m_eMarket = (MarketType)ndSec.GetAttributeLong(_T("market"));
			QuoteType eType = (QuoteType)ndSec.GetAttributeLong(_T("type"));

			CXmlNodeList quoteList = ndSec.Select(_T("q"));
			for (int j = 0; j < quoteList.GetLength(); j++)
			{
				Quote quote;
				CXmlNode ndQuote = quoteList.GetItem(i);
				CString strDate = ndQuote.GetAttribute(_T("t"));
				quote.dtTime.ParseDateTime((LPCTSTR)strDate.GetBuffer());
				quote.eQuote = eType;
				quote.dOpen = ndQuote.GetAttributeDouble(_T("o"));
				quote.dHigh = ndQuote.GetAttributeDouble(_T("h"));
				quote.dLow = ndQuote.GetAttributeDouble(_T("l"));
				quote.dClose = ndQuote.GetAttributeDouble(_T("c"));
				quote.dVolumn = ndQuote.GetAttributeDouble(_T("v"));
				quote.dAmount = ndQuote.GetAttributeDouble(_T("a"));

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
