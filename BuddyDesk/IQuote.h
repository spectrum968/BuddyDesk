#pragma once
interface IQuote
{
public:
	virtual ~IQuote(void)=0{};

	virtual CStringArray& GetHistory(const CString& strSecId, const CTime& tStart, char ch='d') = 0;
	virtual CStringArray& GetHistory(const CString& strSecId, const CTime& tStart, CTime& tEnd, char ch='d') = 0;
	virtual BOOL GetTime(const CString& arrQuote, CString& strTime) = 0;
	virtual BOOL GetOpen(const CString& arrQuote, CString& strOpen) = 0;
	virtual BOOL GetHigh(const CString& arrQuote, CString& strHigh) = 0;
	virtual BOOL GetLow(const CString& arrQuote, CString& strLow) = 0;
	virtual BOOL GetClose(const CString& arrQuote, CString& strClose) = 0;
	virtual BOOL GetVolumn(const CString& arrQuote, CString& strVolumn) = 0;
	virtual BOOL GetAmount(const CString& arrQuote, CString& strAmount) = 0;
};

