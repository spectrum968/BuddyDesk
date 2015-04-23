#pragma once
#include "dboperator.h"
#include "Quotes.h"

class CQuote_DB :
	public CDBOperator
{
public:
	CQuote_DB(void);
	~CQuote_DB(void);
};

