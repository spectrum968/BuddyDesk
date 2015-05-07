
// BuddyDeskDoc.cpp : CBuddyDeskDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "BuddyDesk.h"
#endif

#include "BuddyDeskDoc.h"

#include <propkey.h>

#include "GSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBuddyDeskDoc

IMPLEMENT_DYNCREATE(CBuddyDeskDoc, CDocument)

BEGIN_MESSAGE_MAP(CBuddyDeskDoc, CDocument)
END_MESSAGE_MAP()


// CBuddyDeskDoc ����/����

CBuddyDeskDoc::CBuddyDeskDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_pQuoteDB = NULL;
}

CBuddyDeskDoc::~CBuddyDeskDoc()
{
	if (m_pQuoteDB != NULL)
	{
		if (m_pQuoteDB->IsConnected())
			m_pQuoteDB->DisConnect();
		
		delete m_pQuoteDB;
		m_pQuoteDB;
	}
}

BOOL CBuddyDeskDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	if(m_pQuoteDB == NULL)
	{
		CString strServer = CGSetting::GetInstance()->GetString(cst_SQL_SERVER);
		CString strDB = CGSetting::GetInstance()->GetString(cst_SQL_DB);
		m_pQuoteDB = new CQuoteDB();
		m_pQuoteDB->Initial(strServer, strDB);
	}

	return TRUE;
}




// CBuddyDeskDoc ���л�

void CBuddyDeskDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

bool CBuddyDeskDoc::GetAllQuotes(map<int, CString>& mapQuotes)
{
	bool bRet(false);
	bRet = m_pQuoteDB->GetSecurityList(mapQuotes);

	return bRet;
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CBuddyDeskDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CBuddyDeskDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CBuddyDeskDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBuddyDeskDoc ���

#ifdef _DEBUG
void CBuddyDeskDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBuddyDeskDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBuddyDeskDoc ����
