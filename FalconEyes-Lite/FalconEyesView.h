
// FalconEyesView.h : CFalconEyesView Ŭ������ �������̽�
//

#pragma once


class CFalconEyesView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CFalconEyesView() noexcept;
	DECLARE_DYNCREATE(CFalconEyesView)

// Ư���Դϴ�.
public:
	CFalconEyesDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	enum DrawMode
	{ 
		_DRAW_NONE = 0, 
		_DRAW_SELECT, 
		_DRAW_GETHISTOGRAM, 
		_DRAW_LINE, 
		_DRAW_RECT, 
		_DRAW_CIRCLE, 
		_DRAW_CLS,
		_MEASURE_LINE,
		_MEASURE_DIAGONAL
	};
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CFalconEyesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetScrollSizeToFit();
	// �̹��� Ȯ��,��� ����
	double m_dZoom;
	afx_msg void OnViewZoomplus();
	afx_msg void OnViewZoomminus();
	afx_msg void OnUpdateViewZoomplus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewZoomminus(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void ShowImageInfo(CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL bDraw_Sel;
	
private:
	HACCEL m_accelerators_key;
	cv::Scalar m_scalar;
public:
	int m_nClickMode;
	CPoint pntLT;
	CPoint pntRB;
	DrawMode m_enDrawMode;
	Rect m_rectSave;
	Point m_pntHisto1, m_pntHisto2;
	bool m_bShift;
	bool m_bFilled;
	bool m_bHalftone;
	CPoint m_pntGuide;
	int m_nScrollPosH;
	int m_nScrollPosV;
	int m_nScrollRangeHPrevMin;
	int m_nScrollRangeHPrevMax;
	int m_nScrollRangeHMin;
	int m_nScrollRangeHMax;
	int m_nScrollRangeVPrevMin;
	int m_nScrollRangeVPrevMax;
	int m_nScrollRangeVMin;
	int m_nScrollRangeVMax;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGraphicSelect();
	afx_msg void OnUpdateGraphicSelect(CCmdUI *pCmdUI);
	afx_msg void OnGraphicDrawLine();
	afx_msg void OnUpdateGraphicDrawLine(CCmdUI *pCmdUI);
	afx_msg void OnGraphicDrawRect();
	afx_msg void OnUpdateGraphicDrawRect(CCmdUI *pCmdUI);
	afx_msg void OnGraphicDrawCircle();
	afx_msg void OnUpdateGraphicDrawCircle(CCmdUI *pCmdUI);
	afx_msg void OnGraphicDrawErase();
	afx_msg void OnViewOrigin();
	afx_msg void OnGraphicHisto();
	afx_msg void OnUpdateGraphicHisto(CCmdUI *pCmdUI);
	afx_msg void OnGraphMealine();
	afx_msg void OnGraphMeadia();
	afx_msg void OnUpdateGraphMealine(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGraphMeadia(CCmdUI *pCmdUI);
	afx_msg void OnGraphicFill();
	afx_msg void OnUpdateGraphicFill(CCmdUI *pCmdUI);
	afx_msg void OnViewHalftone();
	afx_msg void OnUpdateViewHalftone(CCmdUI *pCmdUI);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnGraphicColor();
	afx_msg void OnViewFittoscreen();
};

#ifndef _DEBUG  // FalconEyesView.cpp�� ����� ����
inline CFalconEyesDoc* CFalconEyesView::GetDocument() const
   { return reinterpret_cast<CFalconEyesDoc*>(m_pDocument); }
#endif
