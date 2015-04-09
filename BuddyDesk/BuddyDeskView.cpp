
// BuddyDeskView.cpp : CBuddyDeskView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "BuddyDesk.h"
#endif

#include "BuddyDeskDoc.h"
#include "BuddyDeskView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBuddyDeskView

IMPLEMENT_DYNCREATE(CBuddyDeskView, CView)

BEGIN_MESSAGE_MAP(CBuddyDeskView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBuddyDeskView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBuddyDeskView 构造/析构

CBuddyDeskView::CBuddyDeskView()
{
	// TODO: 在此处添加构造代码

}

CBuddyDeskView::~CBuddyDeskView()
{
}

int CBuddyDeskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create the chart control.
	if (!m_wndChartControl.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0))
	{
		TRACE0("Error creating chart control.\n");
		return -1;
	}

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_wndChartControl.GetContent()->GetAppearance()->LoadAppearance(_T("CHART_APPEARANCE_GRAY"));

	// set chart title.
	CXTPChartTitleCollection* pTitles = m_wndChartControl.GetContent()->GetTitles();
	CXTPChartTitle* pTitle = pTitles->Add(new CXTPChartTitle());
	pTitle->SetText(_T("Historical Stock Prices"));

	// set chart subtitle.
	//CXTPChartTitle* pSubTitle = pTitles->Add(new CXTPChartTitle());
	//pSubTitle->SetText(_T("www.codejock.com"));
	//pSubTitle->SetDocking(xtpChartDockBottom);
	//pSubTitle->SetAlignment(xtpChartAlignFar);
	//pSubTitle->SetFont(CXTPChartFont::GetTahoma8());
	//pSubTitle->SetTextColor(CXTPChartColor::Gray);

	// turn off legend.
	m_wndChartControl.GetContent()->GetLegend()->SetVisible(FALSE);

	return 0;
}

void CBuddyDeskView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndChartControl.GetSafeHwnd())
	{
		m_wndChartControl.MoveWindow(0, 0, cx, cy);
	}
}

void CBuddyDeskView::OnSetFocus(CWnd* pOldWnd)
{
	m_wndChartControl.SetFocus();
}

BOOL CBuddyDeskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CBuddyDeskView 绘制

void CBuddyDeskView::OnDraw(CDC* /*pDC*/)
{
	CBuddyDeskDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CBuddyDeskView 打印


void CBuddyDeskView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBuddyDeskView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CBuddyDeskView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CBuddyDeskView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CBuddyDeskView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBuddyDeskView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBuddyDeskView 诊断

#ifdef _DEBUG
void CBuddyDeskView::AssertValid() const
{
	CView::AssertValid();
}

void CBuddyDeskView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBuddyDeskDoc* CBuddyDeskView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBuddyDeskDoc)));
	return (CBuddyDeskDoc*)m_pDocument;
}
#endif //_DEBUG


// CBuddyDeskView 消息处理程序
