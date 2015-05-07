
// BuddyDeskDoc.h : CBuddyDeskDoc ��Ľӿ�
//


#pragma once
#include "QuoteDB.h"
#include <map>
using namespace std;

class CBuddyDeskDoc : public CDocument
{
protected: // �������л�����
	CBuddyDeskDoc();
	DECLARE_DYNCREATE(CBuddyDeskDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CBuddyDeskDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

public:
	bool GetAllQuotes(map<int, CString>& mapQuotes);

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

private:
	CQuoteDB* m_pQuoteDB;
};
