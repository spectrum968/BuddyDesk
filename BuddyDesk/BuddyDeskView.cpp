
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

#include "QuoteWapper.h"
#include "Util_String.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Buddy::Util;


// CBuddyDeskView

IMPLEMENT_DYNCREATE(CBuddyDeskView, CView)

BEGIN_MESSAGE_MAP(CBuddyDeskView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBuddyDeskView::OnFilePrintPreview)
	ON_COMMAND(WM_PARSE_QUOTE_FINISHED, &OnParseQuoteFinished)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
    ON_COMMAND(ID_MENU_UPDATESECURITYINFO, &CBuddyDeskView::OnUpdateSecurityinfo)
END_MESSAGE_MAP()

// CBuddyDeskView 构造/析构

CBuddyDeskView::CBuddyDeskView()
{
	m_bCandleStick = true;
	m_bThickLine = false;
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

	// TODO:  在此添加您专用的创建代码

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
void CBuddyDeskView::AddData(CXTPChartSeries* pSeries, LPCTSTR arg, double dLow, double dHigh, double dOpen, double dClose)
{
	CXTPChartSeriesPoint* pPoint;	
	pPoint = pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(arg, dLow, dHigh, dOpen, dClose));	
}
void CBuddyDeskView::UpdateHistory(const CString& strCode, bool bCandleStick, int nDays)
{
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_wndChartControl.GetContent()->GetAppearance()->LoadAppearance(_T("CHART_APPEARANCE_GRAY"));

	// set chart title.
	CXTPChartTitleCollection* pTitles = m_wndChartControl.GetContent()->GetTitles();
	CXTPChartTitle* pTitle = pTitles->Add(new CXTPChartTitle());
	pTitle->SetText(_T(""));

	// set chart subtitle.
	CXTPChartTitle* pSubTitle = pTitles->Add(new CXTPChartTitle());
	pSubTitle->SetText(_T(""));
	pSubTitle->SetDocking(xtpChartDockBottom);
	pSubTitle->SetAlignment(xtpChartAlignFar);
	pSubTitle->SetFont(CXTPChartFont::GetTahoma8());
	pSubTitle->SetTextColor(CXTPChartColor::Gray);

	// turn off legend.
	m_wndChartControl.GetContent()->GetLegend()->SetVisible(FALSE);

	m_wndChartControl.GetContent()->GetSeries()->RemoveAll();

	CXTPChartSeries* pSeries = m_wndChartControl.GetContent()->GetSeries()->Add(new CXTPChartSeries());
	pSeries->SetArgumentScaleType(xtpChartScaleDateTime);
	
	COleDateTime dtEnd = COleDateTime::GetCurrentTime();

	COleDateTime dtStart = dtEnd;
	dtStart -= COleDateTimeSpan(nDays,0,0,0);
	
	HWND hWnd = GetSafeHwnd();
	CQuoteWapper::ParseQuote(hWnd, _T("600000"), dtStart, dtEnd, MarketType::Shanghai, QuoteType::Day);

	CStringArray arrQuote;

	
    CXTPChartHighLowSeriesStyle* pStyle = NULL;
    
    if (bCandleStick)
    {
        pStyle = (CXTPChartHighLowSeriesStyle*)pSeries->SetStyle(new CXTPChartCandleStickSeriesStyle());
    }
    else
    {
        pStyle = (CXTPChartHighLowSeriesStyle*)pSeries->SetStyle(new CXTPChartHighLowSeriesStyle());
    }
    
	pStyle->SetLineThickness(m_bThickLine ? 2 : 1);
	m_bCandleStick = bCandleStick;
	
	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pSeries->GetDiagram();
	
	pDiagram->GetAxisY()->GetGridLines()->SetMinorVisible(TRUE);
	pDiagram->GetAxisY()->GetGridLines()->GetMinorLineStyle()->SetDashStyle(xtpChartDashStyleDot);
	
	pDiagram->GetAxisY()->GetTitle()->SetText(_T("元"));
	pDiagram->GetAxisY()->GetTitle()->SetVisible(TRUE);

	pDiagram->GetAxisY()->GetRange()->SetShowZeroLevel(FALSE);
	
	pDiagram->GetAxisX()->GetLabel()->SetAngle(360-30);
	pDiagram->GetAxisX()->GetLabel()->SetAntialiasing(TRUE);


	pSeries->SetArgumentScaleType(xtpChartScaleQualitative);
	pDiagram->GetAxisX()->GetLabel()->SetVisible(TRUE);


	pDiagram->GetAxisX()->GetCustomLabels()->RemoveAll();

	for (int i = (int)arrQuote.GetSize() - 1; i > 0; --i)
	{
		CString strTime, strOpen, strHigh, strLow, strClose;
		//pQuote->GetTime(arrQuote[i], strTime);
		//pQuote->GetOpen(arrQuote[i], strOpen);
		//pQuote->GetHigh(arrQuote[i], strHigh);
		//pQuote->GetLow(arrQuote[i], strLow);
		//pQuote->GetClose(arrQuote[i], strClose);
		AddData(pSeries,strTime,CStrUtil::ToDouble(strOpen),
			CStrUtil::ToDouble(strHigh),CStrUtil::ToDouble(strLow),CStrUtil::ToDouble(strClose));
		
		if ((i % 10) == 0)
		{
			CXTPChartAxisCustomLabel* pLabel = new CXTPChartAxisCustomLabel();
			pLabel->SetAxisValue(strTime);
			pLabel->SetText(strTime);
			pDiagram->GetAxisX()->GetCustomLabels()->Add(pLabel);
		}
	}
}

void CBuddyDeskView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	//UpdateHistory(_T("MSFT"), true);
	//GetDocument()->UpdateAllViews(NULL);
	//if (GetDocument()!=NULL)
	//{
	//	GetDocument()->UpdateAllViews(NULL);
	//}
}

void CBuddyDeskView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_wndChartControl.PrintToDC(pDC->m_hDC, pInfo->m_rectDraw);
}

void CBuddyDeskView::OnParseQuoteFinished()
{
	AfxMessageBox(_T("文件解析完成"));
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


void CBuddyDeskView::OnUpdateSecurityinfo()
{
    // TODO: 在此添加命令处理程序代码
}
