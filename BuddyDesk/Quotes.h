#pragma once
#include <vector>

using namespace std;
enum QuoteType
{
	Undefined = -1,
	ByTrade,
	Min1,
	Min5,
	Min15,
	Min30,
	Min60,
	Day,
	Day5,
	Month,
	HalfYear,
	Year,
};

enum StickType
{
	OriginStick = 0,
	MergedStick = 1,
};

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
	QuoteType GetQuoteType(const CString& strType); 
private:
	CString		m_strId;
	StickType	m_eStick;

	vector<Quote> m_vecQuote;
};

