
// FalconEyesView.cpp : CFalconEyesView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FalconEyes.h"
#endif
#include "MainFrm.h"
#include "FalconEyesDoc.h"
#include "FalconEyesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFalconEyesView

IMPLEMENT_DYNCREATE(CFalconEyesView, CScrollView)

BEGIN_MESSAGE_MAP(CFalconEyesView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFalconEyesView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOMPLUS, &CFalconEyesView::OnViewZoomplus)
	ON_COMMAND(ID_VIEW_ZOOMMINUS, &CFalconEyesView::OnViewZoomminus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMPLUS, &CFalconEyesView::OnUpdateViewZoomplus)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMMINUS, &CFalconEyesView::OnUpdateViewZoomminus)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_GRAPHIC_SELECT, &CFalconEyesView::OnGraphicSelect)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_SELECT, &CFalconEyesView::OnUpdateGraphicSelect)
	ON_COMMAND(ID_GRAPHIC_LINE, &CFalconEyesView::OnGraphicDrawLine)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_LINE, &CFalconEyesView::OnUpdateGraphicDrawLine)
	ON_COMMAND(ID_GRAPHIC_RECT, &CFalconEyesView::OnGraphicDrawRect)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_RECT, &CFalconEyesView::OnUpdateGraphicDrawRect)
	ON_COMMAND(ID_GRAPHIC_CIRCLE, &CFalconEyesView::OnGraphicDrawCircle)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_CIRCLE, &CFalconEyesView::OnUpdateGraphicDrawCircle)
	ON_COMMAND(ID_GRAPHIC_ERASE, &CFalconEyesView::OnGraphicDrawErase)
	ON_COMMAND(ID_VIEW_ORIGIN, &CFalconEyesView::OnViewOrigin)
	ON_COMMAND(ID_GRAPHIC_HISTO, &CFalconEyesView::OnGraphicHisto)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_HISTO, &CFalconEyesView::OnUpdateGraphicHisto)
	ON_COMMAND(ID_GRAPH_MEALINE, &CFalconEyesView::OnGraphMealine)
	ON_COMMAND(ID_GRAPH_MEADIA, &CFalconEyesView::OnGraphMeadia)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_MEALINE, &CFalconEyesView::OnUpdateGraphMealine)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_MEADIA, &CFalconEyesView::OnUpdateGraphMeadia)
	ON_COMMAND(ID_GRAPHIC_FILL, &CFalconEyesView::OnGraphicFill)
	ON_UPDATE_COMMAND_UI(ID_GRAPHIC_FILL, &CFalconEyesView::OnUpdateGraphicFill)
	ON_COMMAND(ID_VIEW_HALFTONE, &CFalconEyesView::OnViewHalftone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HALFTONE, &CFalconEyesView::OnUpdateViewHalftone)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_GRAPHIC_COLOR, &CFalconEyesView::OnGraphicColor)
	ON_COMMAND(ID_VIEW_FITTOSCREEN, &CFalconEyesView::OnViewFittoscreen)
END_MESSAGE_MAP()

// CFalconEyesView 생성/소멸

CFalconEyesView::CFalconEyesView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	pntLT = CPoint(-1, -1);
	m_dZoom = 1.0;
	m_enDrawMode = DrawMode::_DRAW_NONE;
	m_bShift = false;
	m_pntGuide = CPoint(0, 0);
	m_scalar = Scalar(255, 255, 255);
	m_accelerators_key = ::LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_MAINFRAME));
	
}

CFalconEyesView::~CFalconEyesView()
{
}

BOOL CFalconEyesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CFalconEyesView 그리기

void CFalconEyesView::OnDraw(CDC* pDC)
{
	CFalconEyesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	if (pDoc->m_Dib.IsValid())
	{
		int w = pDoc->m_Dib.GetWidth();
		int h = pDoc->m_Dib.GetHeight();
		pDoc->m_Dib.Draw(pDC->m_hDC, 0, 0, static_cast<int>(m_dZoom * w), static_cast<int>(m_dZoom * h), SRCCOPY, m_bHalftone);

		// ----------- DC Overlay -----------
		
		if (m_bShift == true)
		{
			//CClientDC dc(this);
			CRect rect;
			pDC->GetClipBox(&rect);
			CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
			CPen *oldPen;
			oldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(rect.left, m_pntGuide.y);
			pDC->LineTo(rect.right, m_pntGuide.y);

			pDC->MoveTo(m_pntGuide.x, rect.top);
			pDC->LineTo(m_pntGuide.x, rect.bottom);

			pDC->SelectObject(oldPen);
		}
		// ----------- ~DC Overlay -----------
	}
}


// CFalconEyesView 인쇄


void CFalconEyesView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFalconEyesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CFalconEyesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CFalconEyesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CFalconEyesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFalconEyesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFalconEyesView 진단

#ifdef _DEBUG
void CFalconEyesView::AssertValid() const
{
	CView::AssertValid();
}

void CFalconEyesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFalconEyesDoc* CFalconEyesView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFalconEyesDoc)));
	return (CFalconEyesDoc*)m_pDocument;
}
#endif //_DEBUG


// CFalconEyesView 메시지 처리기
void CFalconEyesView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetScrollSizeToFit();
	m_bFilled = false;
	m_bHalftone = false;
}

void CFalconEyesView::SetScrollSizeToFit()
{
	CSize sizeTotal;
	CFalconEyesDoc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		sizeTotal.cx = static_cast<LONG>(m_dZoom * pDoc->m_Dib.GetWidth());
		sizeTotal.cy = static_cast<LONG>(m_dZoom * pDoc->m_Dib.GetHeight());
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 200;
	}
	
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(FALSE);
	
	GetScrollRange(SB_HORZ, &m_nScrollRangeHMin, &m_nScrollRangeHMax);
	GetScrollRange(SB_VERT, &m_nScrollRangeVMin, &m_nScrollRangeVMax);
  	m_nScrollPosH = static_cast<int>(m_nScrollPosH / (double)(m_nScrollRangeHPrevMax - m_nScrollRangeHPrevMin) * (m_nScrollRangeHMax - m_nScrollRangeHMin));
  	m_nScrollPosV = static_cast<int>(m_nScrollPosV / (double)(m_nScrollRangeVPrevMax - m_nScrollRangeVPrevMin) * (m_nScrollRangeVMax - m_nScrollRangeVMin));

 	SetScrollPos(SB_HORZ, m_nScrollPosH);
 	SetScrollPos(SB_VERT, m_nScrollPosV);
}

BOOL CFalconEyesView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);
	return TRUE;
}

void CFalconEyesView::OnViewOrigin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom = 1.00;
	
	m_nScrollPosH = GetScrollPos(SB_HORZ);
	m_nScrollPosV = GetScrollPos(SB_VERT);
	GetScrollRange(SB_HORZ, &m_nScrollRangeHPrevMin, &m_nScrollRangeHPrevMax);
	GetScrollRange(SB_VERT, &m_nScrollRangeVPrevMin, &m_nScrollRangeVPrevMax);

	SetScrollSizeToFit();
	Invalidate(TRUE);

	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strText;
	strText.Format(TEXT("배율 : x%.2f"), m_dZoom);
	pFrame->m_wndStatusBar.GetElement(0)->SetText(strText);
	pFrame->m_wndStatusBar.Invalidate(TRUE);
}

void CFalconEyesView::OnViewFittoscreen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CFalconEyesDoc* pDoc = GetDocument();
	CClientDC dc(this);

	CRect rectFrm, rectOut, rectRibon;
	pFrame->GetWindowRect(&rectFrm);
	pFrame->m_wndOutput.GetWindowRect(&rectOut);
	pFrame->m_wndRibbonBar.GetWindowRect(&rectRibon);
	int nChildWidth = rectFrm.Width();
	int nChildHeight = rectFrm.Height() - rectOut.Height() - rectRibon.Height() - 100;
	int nImgWidth = pDoc->m_Dib.GetWidth();
	int nImgHeight = pDoc->m_Dib.GetHeight();
	double dZoomH = 0.0, dZoomV = 0.0;
	m_nScrollPosH = GetScrollPos(SB_HORZ);
	m_nScrollPosV = GetScrollPos(SB_VERT);
	GetScrollRange(SB_HORZ, &m_nScrollRangeHPrevMin, &m_nScrollRangeHPrevMax);
	GetScrollRange(SB_VERT, &m_nScrollRangeVPrevMin, &m_nScrollRangeVPrevMax);

	dZoomH = static_cast<double>(nChildWidth) / static_cast<double>(nImgWidth);
	dZoomV = static_cast<double>(nChildHeight) / static_cast<double>(nImgHeight);
	m_dZoom = dZoomH > dZoomV ? dZoomV : dZoomV;

	SetScrollSizeToFit();
	Invalidate(TRUE);


	CString strText;
	strText.Format(TEXT("배율 : x%.2f"), m_dZoom);
	pFrame->m_wndStatusBar.GetElement(0)->SetText(strText);
	pFrame->m_wndStatusBar.Invalidate(TRUE);
}

void CFalconEyesView::OnViewZoomplus()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_dZoom < 0.05)
		m_dZoom = 0.05;
	else
		m_dZoom += 0.05;
	if (m_dZoom >= 100)
		m_dZoom = 100;
	
	m_nScrollPosH = GetScrollPos(SB_HORZ);
	m_nScrollPosV = GetScrollPos(SB_VERT);
	GetScrollRange(SB_HORZ, &m_nScrollRangeHPrevMin, &m_nScrollRangeHPrevMax);
	GetScrollRange(SB_VERT, &m_nScrollRangeVPrevMin, &m_nScrollRangeVPrevMax);

	SetScrollSizeToFit();
	Invalidate(TRUE);

	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strText;
	strText.Format(TEXT("배율 : x%.2f"), m_dZoom);
	pFrame->m_wndStatusBar.GetElement(0)->SetText(strText);
	pFrame->m_wndStatusBar.Invalidate(TRUE);
}

void CFalconEyesView::OnViewZoomminus()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_dZoom -= 0.05;
	if (m_dZoom <= 0.05)
		m_dZoom = 0.02;

	m_nScrollPosH = GetScrollPos(SB_HORZ);
	m_nScrollPosV = GetScrollPos(SB_VERT);
	GetScrollRange(SB_HORZ, &m_nScrollRangeHPrevMin, &m_nScrollRangeHPrevMax);
	GetScrollRange(SB_VERT, &m_nScrollRangeVPrevMin, &m_nScrollRangeVPrevMax);

	SetScrollSizeToFit();
	Invalidate(TRUE);

	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strText;
	strText.Format(TEXT("배율 : x%.2f"), m_dZoom);

	pFrame->m_wndStatusBar.GetElement(0)->SetText(strText);
	pFrame->m_wndStatusBar.Invalidate(TRUE);
}

void CFalconEyesView::OnUpdateViewZoomplus(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//pCmdUI->SetCheck(m_dZoom == 1.0);
}

void CFalconEyesView::OnUpdateViewZoomminus(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	//pCmdUI->SetCheck(m_dZoom == 1.0);
}

void CFalconEyesView::ShowImageInfo(CPoint point)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CFalconEyesDoc* pDoc = GetDocument();
	int w = pDoc->m_Dib.GetWidth();
	int h = pDoc->m_Dib.GetHeight();
	int c = pDoc->m_Dib.GetPaletteNums();

	CString strText, strByte;
	// 상태바에 마우스 좌표 표시;
	if (point.x >= 0 && point.y >= 0 && point.x < w && point.y < h)
	{
		if (m_enDrawMode == DrawMode::_DRAW_SELECT)
			strText.Format(TEXT("[ %d, %d ] ( %d*%d )"), point.x, point.y, m_rectSave.width, m_rectSave.height);
		else
			strText.Format(TEXT("[ %d, %d ]"), point.x, point.y);
		
		pFrame->m_wndStatusBar.GetElement(1)->SetText(strText);
		pFrame->m_wndStatusBar.Invalidate(TRUE);
	}
	// 상태바에 이미지 정보 표시;
	if (c == 0)
	{
		if (m_bShift)
			strByte.Format(_T("[%d %d %d]"), pDoc->m_Dib.GetMatPixel(point.x, point.y, 1), pDoc->m_Dib.GetMatPixel(point.x, point.y, 2), pDoc->m_Dib.GetMatPixel(point.x, point.y, 3));
		strText.Format(TEXT("가로 : %d  세로 : %d  채널 : 16M %s"), w, h, strByte);
	}
	else
	{
		if (m_bShift)
			strByte.Format(_T("[%d]"), pDoc->m_Dib.GetMatPixel(point.x, point.y, 0));
		strText.Format(TEXT("가로 : %d  세로 : %d  채널 : %d %s"), w, h, c, strByte);
	}
	pFrame->m_wndStatusBar.GetElement(2)->SetText(strText);
	pFrame->m_wndStatusBar.Invalidate(TRUE);

}

BOOL CFalconEyesView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
 	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SHIFT)
	{
		m_bShift = true;
	}
	if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_SHIFT)
	{
		m_bShift = false;
		m_pntGuide = CPoint(0, 0);
		Invalidate();
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_LEFT)
	{
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - 30);
		Invalidate();
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RIGHT)
	{
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + 30);
		Invalidate();
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP)
	{
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - 30);
		Invalidate();
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN)
	{
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + 30);
		Invalidate();
	}
	if (::TranslateAccelerator(this->m_hWnd, m_accelerators_key, pMsg)) return TRUE;
	return CScrollView::PreTranslateMessage(pMsg);
}

void CFalconEyesView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CString strText;
	CFalconEyesDoc* pDoc = GetDocument();
// ----------- DC Overlay -----------
	
// 	if (m_bShift = true)
// 	{
// 		CClientDC dc(this);
// 		CRect rect;
// 		dc.GetClipBox(&rect);
// 		CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
// 		CPen *oldPen;
// 		oldPen = dc.SelectObject(&pen);
// 		pDoc->m_Dib.Draw(dc.m_hDC, 0, 0, static_cast<int>(m_dZoom * pDoc->m_Dib.GetWidth()), static_cast<int>(m_dZoom * pDoc->m_Dib.GetHeight()));
// 
// 		dc.MoveTo(rect.left, m_pntGuide.y);
// 		dc.LineTo(rect.right, m_pntGuide.y);
// 
// 		dc.MoveTo(m_pntGuide.x, rect.top);
// 		dc.LineTo(m_pntGuide.x, rect.bottom);
// 
// 		dc.SelectObject(oldPen);
// 	}
// ----------- ~DC Overlay -----------

	CPoint pt = point + GetScrollPosition();
	m_pntGuide = pt;
	CPoint pt2 = pntLT + GetScrollPosition();
	pt.x = static_cast<LONG>(pt.x / m_dZoom);
	pt.y = static_cast<LONG>(pt.y / m_dZoom);
	pt2.x = static_cast<LONG>(pntLT.x / m_dZoom);
	pt2.y = static_cast<LONG>(pntLT.y / m_dZoom);
	int w = abs(pt.x - pt2.x);
	int h = abs(pt.y - pt2.y);
	int thick = 1;
	ShowImageInfo(pt);
	//TRACE("nFlags : %d\r\n", nFlags);
	if (nFlags == 1 || nFlags == 5)
	{
		switch (m_enDrawMode)
		{
		case DrawMode::_DRAW_SELECT:
			pDoc->m_Dib.DrawSelRect(pt2.x, pt2.y, pt.x, pt.y, m_scalar, thick);
			m_rectSave.x = pt2.x <= pt.x ? pt2.x : pt.x;
			m_rectSave.y = pt2.y <= pt.y ? pt2.y : pt.y;
			m_rectSave.width = abs(pt2.x - pt.x);
			m_rectSave.height = abs(pt2.y - pt.y);

			break;
		case DrawMode::_DRAW_GETHISTOGRAM:
			if (abs(pt2.x - pt.x) > abs(pt2.y - pt.y))
			{
				pDoc->m_Dib.DrawLine(pt2.x, pt2.y, pt.x, pt2.y, m_scalar, thick+1);
				m_pntHisto1.x = pt2.x;
				m_pntHisto1.y = pt2.y;
				m_pntHisto2.x = pt.x;
				m_pntHisto2.y = pt2.y;
			}
			else
			{
				pDoc->m_Dib.DrawLine(pt2.x, pt2.y, pt2.x, pt.y, m_scalar, thick+1);
				m_pntHisto1.x = pt2.x;
				m_pntHisto1.y = pt2.y;
				m_pntHisto2.x = pt2.x;
				m_pntHisto2.y = pt.y;
			}
			
			break;
		case DrawMode::_DRAW_LINE:
			pDoc->m_Dib.DrawLine(pt2.x, pt2.y, pt.x, pt.y, m_scalar, thick);
			break;
		case DrawMode::_DRAW_RECT:
			if (m_bFilled)
				thick = -1;
			pDoc->m_Dib.DrawRect(pt2.x, pt2.y, pt.x, pt.y, m_scalar, thick);
			
			break;
		case DrawMode::_DRAW_CIRCLE:
			//pDoc->m_Dib.DrawCircle(pt2.x, pt2.y, static_cast<int>(sqrt(pow(pt2.x - pt.x,2) + pow(pt2.y - pt.y,2))), Scalar(255, 255, 255), 1);
			if (m_bFilled)
				thick = -1;
			if (w < h)
				pDoc->m_Dib.DrawCircle(pt2.x + static_cast<int>(abs(pt.x - pt2.x) / 2.0), pt2.y + static_cast<int>(abs(pt.x - pt2.x) / 2.0), static_cast<int>(w / 2.0), m_scalar, thick);
			else
				pDoc->m_Dib.DrawCircle(pt2.x + static_cast<int>(abs(pt.y - pt2.y) / 2.0), pt2.y + static_cast<int>(abs(pt.y - pt2.y) / 2.0), static_cast<int>(h / 2.0), m_scalar, thick);
			break;
		case DrawMode::_DRAW_CLS:
			pDoc->m_Dib.DrawClearScreen();
			break;
		case DrawMode::_MEASURE_LINE:
			if(abs(pt2.x - pt.x) > abs(pt2.y - pt.y))
				pDoc->m_Dib.MeasureLine(pt2.x, pt2.y, pt.x, pt2.y, Scalar(0, 0, 255), 1);
			else
				pDoc->m_Dib.MeasureLine(pt2.x, pt2.y, pt2.x, pt.y, Scalar(0, 0, 255), 1);
			break;
		case DrawMode::_MEASURE_DIAGONAL:
			pDoc->m_Dib.MeasureDiagonal(pt2.x, pt2.y, pt.x, pt.y, Scalar(0, 0, 255), 1);
			break;
		default:
			break;
		}
	}
	Invalidate(TRUE);
	CScrollView::OnMouseMove(nFlags, point);
}
void CFalconEyesView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	pntLT.x = -1;
	pntLT.y = -1;
	
	CFalconEyesDoc* pDoc = GetDocument();
	
	if (m_enDrawMode != DrawMode::_DRAW_NONE && m_enDrawMode != DrawMode::_DRAW_SELECT && m_enDrawMode != DrawMode::_DRAW_GETHISTOGRAM)
	{
		pDoc->m_Dib.UpdateBack();
		pDoc->m_fn_UpdateUndoBuffer();
	}
	//pDoc->m_Dib.DrawUpdate();
	CScrollView::OnLButtonUp(nFlags, point);
}


void CFalconEyesView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if(pntLT.x == -1 && pntLT.y == -1)
		pntLT = point + GetScrollPosition();
	
	
	CScrollView::OnLButtonDown(nFlags, point);
}
void CFalconEyesView::OnGraphicSelect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = GetDocument();
	if (m_enDrawMode == DrawMode::_DRAW_SELECT)
	{
		m_enDrawMode = DrawMode::_DRAW_NONE;
		pDoc->m_Dib.DrawClearScreen();
		Invalidate();
	}
	else
		m_enDrawMode = DrawMode::_DRAW_SELECT;
}
void CFalconEyesView::OnUpdateGraphicSelect(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_DRAW_SELECT);
}

void CFalconEyesView::OnGraphicDrawLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_enDrawMode == DrawMode::_DRAW_LINE)
		m_enDrawMode = DrawMode::_DRAW_NONE;
	else
		m_enDrawMode = DrawMode::_DRAW_LINE;
}

void CFalconEyesView::OnUpdateGraphicDrawLine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_DRAW_LINE);
}

void CFalconEyesView::OnGraphicDrawRect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_enDrawMode == DrawMode::_DRAW_RECT)
		m_enDrawMode = DrawMode::_DRAW_NONE;
	else
		m_enDrawMode = DrawMode::_DRAW_RECT;
}

void CFalconEyesView::OnUpdateGraphicDrawRect(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_DRAW_RECT);
}

void CFalconEyesView::OnGraphicDrawCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_enDrawMode == DrawMode::_DRAW_CIRCLE)
		m_enDrawMode = DrawMode::_DRAW_NONE;
	else
		m_enDrawMode = DrawMode::_DRAW_CIRCLE;
}

void CFalconEyesView::OnUpdateGraphicDrawCircle(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_DRAW_CIRCLE);
}

void CFalconEyesView::OnGraphicDrawErase()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = GetDocument();
	pDoc->m_Dib.DrawClearScreen();
	Invalidate(TRUE);
}

void CFalconEyesView::OnGraphicHisto()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = GetDocument();
	if (m_enDrawMode == DrawMode::_DRAW_GETHISTOGRAM)
	{
		m_enDrawMode = DrawMode::_DRAW_NONE;
		pDoc->m_Dib.DrawClearScreen();
		Invalidate();
	}
	else
		m_enDrawMode = DrawMode::_DRAW_GETHISTOGRAM;
}


void CFalconEyesView::OnUpdateGraphicHisto(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_DRAW_GETHISTOGRAM);
}


void CFalconEyesView::OnGraphMealine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = GetDocument();
	if (m_enDrawMode == DrawMode::_MEASURE_LINE)
	{
		m_enDrawMode = DrawMode::_DRAW_NONE;
		pDoc->m_Dib.DrawClearScreen();
		Invalidate();
	}
	else
		m_enDrawMode = DrawMode::_MEASURE_LINE;
}


void CFalconEyesView::OnGraphMeadia()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = GetDocument();
	if (m_enDrawMode == DrawMode::_MEASURE_DIAGONAL)
	{
		m_enDrawMode = DrawMode::_DRAW_NONE;
		pDoc->m_Dib.DrawClearScreen();
		Invalidate();
	}
	else
		m_enDrawMode = DrawMode::_MEASURE_DIAGONAL;
}


void CFalconEyesView::OnUpdateGraphMealine(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_MEASURE_LINE);
}


void CFalconEyesView::OnUpdateGraphMeadia(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_enDrawMode == DrawMode::_MEASURE_DIAGONAL);
}


void CFalconEyesView::OnGraphicFill()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bFilled ^= true;
}


void CFalconEyesView::OnUpdateGraphicFill(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bFilled);
}


void CFalconEyesView::OnViewHalftone()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bHalftone ^= true;
}


void CFalconEyesView::OnUpdateViewHalftone(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bHalftone);
}


void CFalconEyesView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + zDelta);
	Invalidate();
	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CFalconEyesView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - zDelta);
	Invalidate();
	return TRUE;
	//return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CFalconEyesView::OnGraphicColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog cDlg;
	CFalconEyesDoc* pDoc = GetDocument();
	if (cDlg.DoModal() == IDOK)
	{
		COLORREF clr = cDlg.GetColor();
		uchar red = static_cast<uchar>(0x000000ff & clr);
		uchar green = static_cast<uchar>((0x0000ff00 & clr) >> 8);
		uchar blue = static_cast<uchar>((0x00ff0000 & clr) >> 16);
		if (pDoc->m_Dib.GetBitCount() != 8)
		{
			m_scalar = Scalar(blue, green, red);
		}
		else
		{
			m_scalar = Scalar((blue + green + red) / 3.0, (blue + green + red) / 3.0, (blue + green + red) / 3.0);
		}
	}
}
