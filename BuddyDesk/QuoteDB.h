#pragma once
#include "stdafx.h"
#include "dboperator.h"
#include "Quotes.h"

class CQuoteDB :
	public CDBOperator
{
public:
	CQuoteDB(void);
	~CQuoteDB(void);

public:
	bool WriteQuote(CQuotes quote);
	CQuotes GetQuote(const CString& strId, QuoteType eType=QuoteType::Day, StickType eStick=StickType::OriginStick);
	CString GetLastQuoteTime(const CString strId, QuoteType eType=QuoteType::Day);
};

