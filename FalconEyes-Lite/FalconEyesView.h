
// FalconEyesView.h : CFalconEyesView 클래스의 인터페이스
//

#pragma once


class CFalconEyesView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CFalconEyesView() noexcept;
	DECLARE_DYNCREATE(CFalconEyesView)

// 특성입니다.
public:
	CFalconEyesDoc* GetDocument() const;

// 작업입니다.
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
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CFalconEyesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetScrollSizeToFit();
	// 이미지 확대,축소 배율
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

#ifndef _DEBUG  // FalconEyesView.cpp의 디버그 버전
inline CFalconEyesDoc* CFalconEyesView::GetDocument() const
   { return reinterpret_cast<CFalconEyesDoc*>(m_pDocument); }
#endif

