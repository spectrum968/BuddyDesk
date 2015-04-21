#include "stdafx.h"
#include "CommonTool.h"

#include <fstream>
#include <windows.h>

CCommonTool::CCommonTool(void)
{
}


CCommonTool::~CCommonTool(void)
{
}

bool CCommonTool::GetTDXFiles(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote, vector<CString>& vecFiles)
{
	vector<CString> vecFilePaths = GetTDXDataFileName(strFolder, strId, eMarket, eQuote);

	for (vector<CString>::iterator itFile=vecFilePaths.begin(); itFile!=vecFilePaths.end(); itFile++)
	{
		try
		{
			CFileFind FileFinder;
			bool bFound = (FileFinder.FindFile(*itFile)==TRUE);
			if (!bFound)
				return false;
			while (bFound)  
			{  
				bFound = (FileFinder.FindNextFile()==TRUE); 
				CString strFilePath  = FileFinder.GetFilePath();  
				vecFiles.push_back(strFilePath);
			}  
			FileFinder.Close();
		}
		catch( ... )
		{
			OutputDebugString(_T("Fail to get file"));
			return false;
		}
	}
	

	return true;
}
vector<CString> CCommonTool::GetTDXDataFileName(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote)
{
	//C:\\\zd_zszq\\vipdoc\\sh\\minline\\*.day;
	CString strFilePath;
	vector<CString> vecFiles;
	vector<MarketType> vecMarket;
	vector<QuoteType> vecQuote;
	if (eMarket == MarketType::Unknown)
	{
		vecMarket.push_back(MarketType::Shanghai);
		vecMarket.push_back(MarketType::Shenzhen);
	}
	else
	{
		vecMarket.push_back(eMarket);
	}
	if (eQuote == QuoteType::Undefined)
	{
		vecQuote.push_back(QuoteType::Min1);
		vecQuote.push_back(QuoteType::Min5);
		vecQuote.push_back(QuoteType::Day);
	}
	else
	{
		vecQuote.push_back(eQuote);
	}
	for (vector<MarketType>::iterator itMarket=vecMarket.begin(); itMarket!=vecMarket.end(); itMarket++)
	{
		for (vector<QuoteType>::iterator itQuote = vecQuote.begin(); itQuote!=vecQuote.end(); itQuote++)
		{
			CString strMarketFolder, strQuoteFolder, strQuoteFile, strCode;
			strMarketFolder = GetTDXMarketFolderName(*itMarket);
			strQuoteFolder = GetTDXQuoteFolderName(*itQuote);
			strQuoteFile = GetTDXQuoteFileName(*itQuote);
			strCode = strId.IsEmpty() ? _T("*") : strId;
			if (strMarketFolder.IsEmpty()||strQuoteFolder.IsEmpty()||strQuoteFile.IsEmpty())
				continue;

			strFilePath.Format(_T("%s\\%s\\%s\\%s\\%s%s%s"), 
				strFolder, 
				cst_DATA_FOLDER, 
				strMarketFolder,
				strQuoteFolder,
				strMarketFolder,
				strCode,
				strQuoteFile);

			vecFiles.push_back(strFilePath);
		}
	}
	
	return vecFiles;
}
CString CCommonTool::GetTDXMarketFolderName(MarketType eMarket)
{
	CString strMarket(_T(""));
	switch(eMarket)
	{
	case MarketType::Shanghai:
		strMarket = cst_SH;
		break;
	case MarketType::Shenzhen:
		strMarket = cst_SZ;
		break;
	default:
		break;
	}

	return strMarket;
}
CString CCommonTool::GetTDXQuoteFolderName(QuoteType eQuote)
{
	CString strQuote(_T(""));
	switch (eQuote)
	{
	case QuoteType::Day:
		strQuote = cst_DAY_FOLDER;
		break;
	case Min1:
		strQuote = cst_1MIN_FOLDER;
		break;
	case Min5:
		strQuote = cst_5MIN_FOLDER;
		break;
	default:
		break;
	}

	return strQuote;
}
CString CCommonTool::GetTDXQuoteFileName(QuoteType eQuote)
{
	CString strFile(_T(""));
	switch (eQuote)
	{
	case Undefined:
		break;
	case ByTrade:
		break;
	case Min1:
		strFile = cst_1MIN_FILE;
		break;
	case Min5:
		strFile = cst_5MIN_FILE;
		break;
	case Min15:
		break;
	case Min30:
		break;
	case Min60:
		break;
	case Day:
		strFile = cst_DAY_FILE;
		break;
	case Day5:
		break;
	case Month:
		break;
	case HalfYear:
		break;
	case Year:
		break;
	default:
		break;
	}

	return strFile;
}
CString CCommonTool::GetCodeFromFileName(const CString& strFile)
{
	return _T("");
}