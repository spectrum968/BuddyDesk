#pragma once
#include <map>

using namespace std;

class CGSetting
{
private:
	CGSetting(void);
public:
	~CGSetting(void);

public:
	static CGSetting* GetInstance();
public:
	bool LoadConfig(const CString& strConfigFilePath);
	void SetVariable(const CString& strName, const CString& strValue);
	CString GetString(const CString& strName);

private:
	bool Initial();

private:
	static CGSetting* m_pInstance;
	map<CString, CString> m_mapVariables;
};

