#pragma once
#include "stdafx.h"
#include <vector>
class CCommonTool
{
public:
	CCommonTool(void);
	~CCommonTool(void);

public:
	static bool GetFiles(const CString& strFolder, vector<CString>& vecFiles);
};

