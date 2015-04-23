#pragma once
#include "stdafx.h"
#include <vector>
using namespace std;

struct Quote
{
	COleDateTime	dtTime;
	QuoteType		eQuote;
	double			dOpen;
	double			dHigh;
	double			dLow;
	double			dClose;
	double			dVolumn;
	double			dAmount;
};

class CQuotes
{
public:
	CQuotes(void);
	~CQuotes(void);

public:
	void Init(const CString& strXml);
private:


private:
	CString		m_strId;
	MarketType	m_eMarket;

	vector<Quote> m_vecQuote;
};

