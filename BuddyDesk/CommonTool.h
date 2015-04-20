#pragma once
#include "stdafx.h"
#include <vector>
class CCommonTool
{
public:
	CCommonTool(void);
	~CCommonTool(void);

public:
	static bool GetFiles(const CString& strFolder, const CString& strId, vector<CString>& vecFiles, char ch= 'd');
	static vector<CString> GetDataFileName(const CString& strFolder, const CString& strId, char ch);
};

