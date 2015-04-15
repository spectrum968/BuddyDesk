#include "stdafx.h"
#include "QuoteWapper.h"
#include <thread>
#include "QuoteSimpleFactory.h"

using namespace std;

void thread_task(const CString& strCode, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch, long lNum)
{
	IQuoteLoader* pQuoteLoader = CQuoteLoaderSimpleFactory::CreateQuote(_T("Yahoo"));
	CString strXml = pQuoteLoader->GetHistory(strCode, dtStart, dtEnd, ch, lNum);

	delete pQuoteLoader;
	pQuoteLoader = NULL;
}


CQuoteWapper::CQuoteWapper(void)
{
}


CQuoteWapper::~CQuoteWapper(void)
{
}

void CQuoteWapper::ParseQuote(const CString& strId, const COleDateTime& dtStart, const COleDateTime& dtEnd, char ch, long lNum)
{
	thread t(thread_task, strId, dtStart, dtEnd, ch, lNum);
	t.join();

	return;
}