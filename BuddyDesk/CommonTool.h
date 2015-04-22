#pragma once
#include "stdafx.h"
#include <vector>
class CCommonTool
{
public:
	CCommonTool(void);
	~CCommonTool(void);

public:
	//TDX
	static bool GetTDXFiles(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote, vector<CString>& vecFiles);
	static vector<CString> GetTDXDataFileName(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote);
	static CString GetTDXMarketFolderName(MarketType eMarket);
	static CString GetTDXQuoteFolderName(QuoteType eQuote);
	static CString GetTDXQuoteFileName(QuoteType eQuote);
	static CString GetCodeFromTDXFileName(const CString& strFile);
	static MarketType GetMarketFromTDXFileName(const CString& strFile);
	static QuoteType GetQuoteTypeFromTDXFileName(const CString& strFile);
};

