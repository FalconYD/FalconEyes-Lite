
// ChildFrm.cpp : CChildFrame Ŭ������ ����
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

// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame() noexcept
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ����

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

// CChildFrame �޽��� ó����


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
