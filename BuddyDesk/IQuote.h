#pragma once
interface IQuote
{
public:
	virtual ~IQuote(void)=0{};

	virtual CString GetHistory(const CString& strSecId, const CTime& tStart, char ch='d') = 0;
	virtual CString GetHistory(const CString& strSecId, const CTime& tStart, const CTime& tEnd, char ch='d') = 0;
};

