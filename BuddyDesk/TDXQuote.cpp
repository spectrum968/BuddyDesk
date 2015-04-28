#include "stdafx.h"
#include "TDXQuote.h"
#include "GSetting.h"
#include <fstream>
#include <windows.h>

CTDXQuote::CTDXQuote(void)
{
	m_pQuoteDB = NULL;
}


CTDXQuote::~CTDXQuote(void)
{
}

bool CTDXQuote::Init()
{
	if (m_pQuoteDB)
		m_pQuoteDB = new CQuoteDB();

	return m_pQuoteDB->Initial(CGSetting::GetInstance()->GetString(cst_SQL_SERVER),
		CGSetting::GetInstance()->GetString(cst_SQL_DB));
}

void CTDXQuote::Destroy()
{
	if (m_pQuoteDB)
	{
		if (m_pQuoteDB->IsConnected())
		{
			m_pQuoteDB->DisConnect();
		}

		delete m_pQuoteDB;
		m_pQuoteDB = NULL;
	}
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	vector<CString> vecQuotes;

	COleDateTime dtEnd;
	dtEnd = COleDateTime::GetCurrentTime();
	CString strQuote = GetHistory(strId, dtStart, dtEnd, eMarket, eQuote, lNum);

	return strQuote;
}

CString CTDXQuote::GetHistory(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum)
{
	map<CString, vector<TdxQuote>> mapQuotes;
	QuoteParser(strId, dtStart, dtEnd, eMarket, eQuote, lNum, mapQuotes);
	CString strQuote;
	QuoteAssembler(mapQuotes, strQuote);

	return strQuote;
}

void CTDXQuote::QuoteParser(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, const MarketType eMarket, const QuoteType eQuote, long lNum, map<CString, vector<TdxQuote>>& mapQuotes)
{
	vector<CString> vecFiles;
	CString strFolder = CGSetting::GetInstance()->GetString(cst_TDX_PATH);
	
	GetTDXFiles(strFolder, strId, eMarket, eQuote, vecFiles);
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
		if(!fin)
			break;

		vector<TdxQuote> vecQuotes;
		while (!fin.eof())
		{
			TdxQuote tdxQuote;
			fin.read(reinterpret_cast<char*>(&tdxQuote), sizeof(TdxQuote));
			vecQuotes.push_back(tdxQuote);
		}
		mapQuotes.insert(make_pair(strFile, vecQuotes));
	}
	return;
}
void CTDXQuote::QuoteAssembler( const map<CString, vector<TdxQuote>>& mapQuote, CString& strQuote)
{
	CString strXml;
	strXml.Format(_T("<buddy>"));
	map<CString,vector<TdxQuote>>::const_iterator cit=mapQuote.begin();
	for ( ; cit!=mapQuote.end(); cit++)
	{
		CString strFilePath = cit->first;
		CString strCode = GetCodeFromTDXFileName(strFilePath);
		MarketType eMarket = GetMarketTypeFromTDXFileName(strFilePath);
		QuoteType eQuote = GetQuoteTypeFromTDXFileName(strFilePath);
		strXml.AppendFormat(_T("<s code=\"%s\" market=\"%d\" type=\"%d\">"), strCode, eMarket, eQuote);
		for (vector<TdxQuote>::const_iterator itQuote=cit->second.begin(); 
			itQuote!=cit->second.end(); itQuote++)
		{
			strXml.AppendFormat(_T("<q t=\"%s\" o=\"%.2f\" h=\"%.2f\" l=\"%.2f\" c=\"%.2f\" v=\"%.2f\" a=\"%.2f\" />"),
				GetDateFromTDXQuote(*itQuote).Format(),
				GetOpenFromTdxQuote(*itQuote),
				GetHighFromTdxQuote(*itQuote),
				GetLowFromTdxQuote(*itQuote),
				GetCloseFromTdxQuote(*itQuote),
				GetVolumnFromTdxQuote(*itQuote),
				GetAmountFromTdxQuote(*itQuote));
		}
		strXml.AppendFormat(_T("</s>"));

	}
	strXml.AppendFormat(_T("</buddy>"));

	strQuote = strXml;
	return;
}
bool CTDXQuote::GetTDXFiles(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote, vector<CString>& vecFiles)
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
vector<CString> CTDXQuote::GetTDXDataFileName(const CString& strFolder, const CString& strId, const MarketType eMarket, const QuoteType eQuote)
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
CString CTDXQuote::GetTDXMarketFolderName(MarketType eMarket)
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
CString CTDXQuote::GetTDXQuoteFolderName(QuoteType eQuote)
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
CString CTDXQuote::GetTDXQuoteFileName(QuoteType eQuote)
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
CString CTDXQuote::GetCodeFromTDXFileName(const CString& strFile)
{
	vector<CString> vecFullFilePath;
	CStrUtil::SplitString(strFile, vecFullFilePath, _T("\\"));
	CString strFileFullName = vecFullFilePath.at(5);
	CString strMarket = vecFullFilePath.at(3);

	vector<CString> vecFile;
	CStrUtil::SplitString(strFileFullName, vecFile, _T("."));
	CString strFileName = vecFile.at(0);
	CString strCode = strFileName.Right(strFileName.GetLength()-strMarket.GetLength());

	return strCode;
}
MarketType CTDXQuote::GetMarketTypeFromTDXFileName(const CString& strFile)
{
	vector<CString> vecFullFilePath;
	CStrUtil::SplitString(strFile, vecFullFilePath, _T("\\"));
	CString strMarket = vecFullFilePath.at(3);

	MarketType eMarket = MarketType::Unknown;
	if (strMarket.CompareNoCase(cst_SH) == 0)
		eMarket = MarketType::Shanghai;
	else if (strMarket.CompareNoCase(cst_SZ) == 0)
		eMarket = MarketType::Shenzhen;

	return eMarket;
}
QuoteType CTDXQuote::GetQuoteTypeFromTDXFileName(const CString& strFile)
{
	vector<CString> vecFullFilePath;
	CStrUtil::SplitString(strFile, vecFullFilePath, _T("\\"));
	CString strQuoteType = vecFullFilePath.at(4);

	QuoteType eQuote = QuoteType::Undefined;
	if (strQuoteType.CompareNoCase(cst_5MIN_FOLDER) == 0)
		eQuote = QuoteType::Min5;
	else if (strQuoteType.CompareNoCase(cst_1MIN_FOLDER) == 0)
		eQuote = QuoteType::Min1;
	else if (strQuoteType.CompareNoCase(cst_DAY_FOLDER) == 0)
		eQuote = QuoteType::Day;

	return eQuote;
}
COleDateTime CTDXQuote::GetDateFromTDXQuote(const TdxQuote& quote)
{
	CString strTemp;
	strTemp.Format(_T("%d"), quote.date);
	int nYear = CStrUtil::ToLong(strTemp.Mid(0,4));
	int nMonth = CStrUtil::ToLong(strTemp.Mid(4,2));
	int nDay = CStrUtil::ToLong(strTemp.Mid(6,2));
	COleDateTime dt(nYear, nMonth, nDay, 0, 0, 0);
	
	return dt;
}
double CTDXQuote::GetOpenFromTdxQuote(const TdxQuote& quote)
{
	double dOpen = ((double)quote.open)/100;
	return dOpen;
}
double CTDXQuote::GetHighFromTdxQuote(const TdxQuote& quote)
{
	double dHigh = ((double)quote.open)/100;
	return dHigh;
}
double CTDXQuote::GetLowFromTdxQuote(const TdxQuote& quote)
{
	double dLow = ((double)quote.open)/100;
	return dLow;
}
double CTDXQuote::GetCloseFromTdxQuote(const TdxQuote& quote)
{
	double dClose = ((double)quote.open)/100;
	return dClose;
}
double CTDXQuote::GetAmountFromTdxQuote(const TdxQuote& quote)
{
	double dAmount = ((double)quote.open)/100;
	return dAmount;
}
double CTDXQuote::GetVolumnFromTdxQuote(const TdxQuote& quote)
{
	double dVolumn = ((double)quote.open)/100;
	return dVolumn;
}
