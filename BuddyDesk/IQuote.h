#pragma once
interface IQuoteLoader
{
public:
	virtual ~IQuoteLoader(void)=0{};

	virtual CString GetHistory(const CString& strSecId, const COleDateTime& tStart, char ch='d', long lNum = 90) = 0;
	virtual CString GetHistory(const CString& strSecId, const COleDateTime& tStart, const COleDateTime& tEnd, char ch='d', long lNum = 90) = 0;
};

