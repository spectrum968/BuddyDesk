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

	//CString GetHistory(const CString& strSym, const CTime& timeStart, char ch = 'd');
	//CString GetHistory(const CString& strSym, const CTime& timeStart, const CTime& timeEnd, char ch = 'd');

private:
	vector<CString> m_vecArray;
};

