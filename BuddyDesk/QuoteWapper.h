#pragma once
class CQuoteWapper
{
public:
	CQuoteWapper(void);
	~CQuoteWapper(void);
public:
	static void ParseQuote(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch = 'd', long lNum = 90);
};

