
// BuddyDeskDoc.cpp : CBuddyDeskDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CBuddyDeskDoc 构造/析构

CBuddyDeskDoc::CBuddyDeskDoc()
{
	// TODO: 在此添加一次性构造代码
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	if(m_pQuoteDB == NULL)
	{
		CString strServer = CGSetting::GetInstance()->GetString(cst_SQL_SERVER);
		CString strDB = CGSetting::GetInstance()->GetString(cst_SQL_DB);
		m_pQuoteDB = new CQuoteDB();
		m_pQuoteDB->Initial(strServer, strDB);
	}

	return TRUE;
}




// CBuddyDeskDoc 序列化

void CBuddyDeskDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

bool CBuddyDeskDoc::GetAllQuotes(map<int, CString>& mapQuotes)
{
	bool bRet(false);
	bRet = m_pQuoteDB->GetSecurityList(mapQuotes);

	return bRet;
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CBuddyDeskDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CBuddyDeskDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CBuddyDeskDoc 诊断

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


// CBuddyDeskDoc 命令
