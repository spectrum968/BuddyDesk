
// BuddyDeskView.cpp : CBuddyDeskView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBuddyDeskView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBuddyDeskView ����/����

CBuddyDeskView::CBuddyDeskView()
{
	// TODO: �ڴ˴����ӹ������

}

CBuddyDeskView::~CBuddyDeskView()
{
}

BOOL CBuddyDeskView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBuddyDeskView ����

void CBuddyDeskView::OnDraw(CDC* /*pDC*/)
{
	CBuddyDeskDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
}


// CBuddyDeskView ��ӡ


void CBuddyDeskView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBuddyDeskView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBuddyDeskView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBuddyDeskView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
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


// CBuddyDeskView ���

#ifdef _DEBUG
void CBuddyDeskView::AssertValid() const
{
	CView::AssertValid();
}

void CBuddyDeskView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBuddyDeskDoc* CBuddyDeskView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBuddyDeskDoc)));
	return (CBuddyDeskDoc*)m_pDocument;
}
#endif //_DEBUG


// CBuddyDeskView ��Ϣ��������