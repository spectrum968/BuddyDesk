
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
	void UpdateHistory(const CString& strCode, bool bCandleStick, int nDays = 90);
	void AddData(CXTPChartSeries* pSeries, LPCTSTR arg, double dLow, double dHigh, double dOpen, double dClose);
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CBuddyDeskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CXTPChartControl m_wndChartControl;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnParseQuoteFinished();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bCandleStick;
	bool m_bThickLine;

};

#ifndef _DEBUG  // BuddyDeskView.cpp �еĵ��԰汾
inline CBuddyDeskDoc* CBuddyDeskView::GetDocument() const
   { return reinterpret_cast<CBuddyDeskDoc*>(m_pDocument); }
#endif

