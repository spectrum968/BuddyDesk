
// BuddyDeskView.h : CBuddyDeskView ��Ľӿ�
//

#pragma once


class CBuddyDeskView : public CView
{
protected: // �������л�����
	CBuddyDeskView();
	DECLARE_DYNCREATE(CBuddyDeskView)

// ����
public:
	CBuddyDeskDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CBuddyDeskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BuddyDeskView.cpp �еĵ��԰汾
inline CBuddyDeskDoc* CBuddyDeskView::GetDocument() const
   { return reinterpret_cast<CBuddyDeskDoc*>(m_pDocument); }
#endif

