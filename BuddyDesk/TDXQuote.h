#pragma once
#include "iquote.h"
#include <vector>

using namespace std;

class CTDXQuote :
	public IQuoteLoader
{
public:
	CTDXQuote(void);
	virtual ~CTDXQuote(void);

	CString GetHistory(const CString& strId, const COleDateTime& dtStart, char ch = 'd', long lNum = 90);
	CString GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch = 'd', long lNum = 90);

private:
	void QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch, long lNum, vector<CString>& vecQuote);
	void QuoteAssembler(const CString& strId, char ch, const vector<CString>& vecQuote, CString& strQuote);
};

