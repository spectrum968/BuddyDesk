#include "stdafx.h"
#include "GSetting.h"

CGSetting* CGSetting::m_pInstance = NULL;

CGSetting::CGSetting(void)
{
}


CGSetting::~CGSetting(void)
{
}

CGSetting* CGSetting::GetInstance()
{
	if ( m_pInstance == NULL )
		m_pInstance = new CGSetting();

	return m_pInstance;
}

bool CGSetting::Initial()
{
	bool bRtn(false);

	return !bRtn;
}

void CGSetting::SetVariable(const CString& strName, const CString& strValue)
{
	CLog::Log(_T("CGSetting"), _T("enter method SetVariable."));
	map<CString, CString>::iterator it= m_mapVariables.find(strName);
	if (it!=m_mapVariables.end())
	{
		CString strLog;
		strLog.Format(_T("update variable %s from %s to %s"), strName, it->second, strValue);
		CLog::Log(_T("CGSetting"), strLog);

		it->second = strValue;
	}
	else
		m_mapVariables.insert(make_pair(strName, strValue));

	CLog::Log(_T("CGSetting"), _T("quit method SetVariable"));

	return;
}
CString CGSetting::GetString(const CString& strName)
{
	CLog::Log(_T("CGSetting"), _T("enter method GetVariable."));
	CString strRet(_T(""));
	map<CString, CString>::iterator it = m_mapVariables.find(strName);
	if (it!=m_mapVariables.end())
		strRet = it->second;

	CLog::Log(_T("CGSetting"), _T("quit method GetString"));

	return strRet;
}
bool CGSetting::LoadConfig(const CString& strConfigFilePath)
{
	CLog::Log(_T("CDataDealer."), _T("enter method LoadConfig"));

	CXmlDocument xmlDoc;
	xmlDoc.Load(strConfigFilePath);
	if (!xmlDoc.IsValid())
		return false;

	CString strXPath = _T("//buddy/configuration");
	CXmlNode nodeCfg = xmlDoc.Find(strXPath);
	if (!nodeCfg.IsValid())
		return false;
	CString strCfg = nodeCfg.GetAttribute(_T("default"));

	strXPath.Format(_T("//buddy/settings [@mode='%s']"), strCfg);
	CXmlNode nodeSetting = xmlDoc.Find(strXPath);
	if (!nodeSetting.IsValid())
		return false;

	strXPath.Format(_T("//buddy/settings [@mode='%s']/setting"), strCfg);
	CXmlNodeList listSetting = xmlDoc.Select(strXPath);

	for (int i = 0; i < listSetting.GetLength(); i++)
	{
		CXmlNode setting = listSetting.GetItem(i);
		CString strName = setting.GetAttribute(_T("n"));
		CString strValue = setting.GetAttribute(_T("v"));

		CGSetting::GetInstance()->SetVariable(strName, strValue);
	}

	return true;
}