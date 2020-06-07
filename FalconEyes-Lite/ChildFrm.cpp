
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rectMain, rectChild, rectDock;
	((CMainFrame*)AfxGetMainWnd())->GetClientRect(&rectMain);
	((CMainFrame*)AfxGetMainWnd())->m_wndOutput.GetClientRect(&rectDock);
	this->GetClientRect(&rectChild);
	int nResizeWidth, nChkWidth,nResizeHeight, nChkHeight, nChildWidth, nChildHeight, nMainWidth, nMainHeight;
	nMainWidth = rectMain.Width();
	nMainHeight = rectMain.Height();
	nChildWidth = rectChild.Width();
	nChildHeight = rectChild.Height();

	nResizeHeight = nMainHeight - rectDock.Height() - 198;
	nChkHeight = nChildHeight + 20;
	nResizeWidth = nMainWidth;
	nChkWidth = nChildWidth;
		
	if (nResizeHeight < nChkHeight)
	{
		if(nChildWidth < nMainWidth)
			this->SetWindowPos(NULL, 0, 0, nChildWidth, nResizeHeight, SWP_NOZORDER);
		else
			this->SetWindowPos(NULL, 0, 0, nResizeWidth, nResizeHeight, SWP_NOZORDER);
	}
	if (nResizeWidth < nChkWidth)
	{
		if (nChildHeight < nMainHeight)
			this->SetWindowPos(NULL, 0, 0, nResizeWidth, nChildHeight, SWP_NOZORDER);
		else
			this->SetWindowPos(NULL, 0, 0, nResizeWidth, nResizeHeight, SWP_NOZORDER);
	}
	 	/*if (cx - x < rect.Width())
	 		TRACE("!!!!\n");*/
}
