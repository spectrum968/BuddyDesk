#include "stdafx.h"
#include "TDXQuote.h"
#include "GSetting.h"
#include "CommonTool.h"
#include <fstream>

CTDXQuote::CTDXQuote(void)
{
}


CTDXQuote::~CTDXQuote(void)
{
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;

	COleDateTime dtEnd;
	dtEnd = COleDateTime::GetCurrentTime();
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, vecQuotes);
	CString strQuote;
	QuoteAssembler(strId, eMarket, eQuote, vecQuotes, strQuote);

	return strQuote;
}

void CTDXQuote::QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, vector<CString>& vecQuote)
{
	vector<CString> vecFiles;
	CString strFolder = CGSetting::GetInstance()->GetString(cst_TDX_PATH);
	CCommonTool::GetTDXFiles(strFolder, strId, eMarket, eQuote, vecFiles);
	map<CString, vector<TdxQuote>> mapQuotes;
	ParseTDXFile(vecFiles, mapQuotes);

	return;
}
void CTDXQuote::ParseTDXFile(const vector<CString>& vecFiles, map<CString, vector<TdxQuote>>& mapQuotes)
{
	ifstream fin;
	for (vector<CString>::const_iterator citFile=vecFiles.begin(); citFile!=vecFiles.end(); citFile++)
	{
		CString strFile = *citFile;
		USES_CONVERSION;
		LPSTR lpszFile = T2A(strFile);
		fin.open(lpszFile, ios::binary);
		int nError = GetLastError();
		if(!fin)
			break;
		vector<TdxQuote> vecQuotes;
		float fn;
		while (!fin.eof())
		{
			TdxQuote tdxQuote;
			fin.read(reinterpret_cast<char*>(&tdxQuote), sizeof(TdxQuote));
			vecQuotes.push_back(tdxQuote);
		}
		mapQuotes.insert(make_pair(_T("600000"), vecQuotes));
	}
	return;
}
void CTDXQuote::QuoteAssembler(const CString& strId, const MarketType eMarket, const QuoteType eQuote, const vector<CString>& vecQuote, CString& strQuote)
{
	return;
}
