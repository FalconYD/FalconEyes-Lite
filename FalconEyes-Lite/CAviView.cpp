// CAviView.cpp: 구현 파일
//

#include "stdafx.h"
#include "MainFrm.h"
#include "FalconEyes.h"
#include "CAviView.h"


// CAviView

IMPLEMENT_DYNCREATE(CAviView, CScrollView)

CAviView::CAviView()
{

}

CAviView::~CAviView()
{
}


BEGIN_MESSAGE_MAP(CAviView, CScrollView)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIDEO_PLAY, &CAviView::OnVideoPlay)
	ON_COMMAND(ID_VIDEO_PAUSE, &CAviView::OnVideoPause)
	ON_COMMAND(ID_VIDEO_STOP, &CAviView::OnVideoStop)
	ON_COMMAND(ID_VIDEO_START, &CAviView::OnVideoStart)
	ON_COMMAND(ID_VIDEO_END, &CAviView::OnVideoEnd)
	ON_COMMAND(ID_VIDEO_PREV, &CAviView::OnVideoPrev)
	ON_COMMAND(ID_VIDEO_NEXT, &CAviView::OnVideoNext)
	ON_COMMAND(ID_VIDEO_CAPTURE, &CAviView::OnVideoCapture)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PLAY, &CAviView::OnUpdateVideoPlay)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_PAUSE, &CAviView::OnUpdateVideoPause)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_STOP, &CAviView::OnUpdateVideoStop)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MODE_NORMAL, &CAviView::OnModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CAviView::OnUpdateModeNormal)
	ON_COMMAND(ID_MODE_DIFFERENCE, &CAviView::OnModeDifference)
	ON_UPDATE_COMMAND_UI(ID_MODE_DIFFERENCE, &CAviView::OnUpdateModeDifference)
	ON_COMMAND(ID_MODE_MOTION, &CAviView::OnModeMotion)
	ON_UPDATE_COMMAND_UI(ID_MODE_MOTION, &CAviView::OnUpdateModeMotion)
END_MESSAGE_MAP()


// CAviView 그리기

void CAviView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	if (m_Avi.IsValid())
	{
		sizeTotal.cx = m_Avi.GetWidth();
		sizeTotal.cy = m_Avi.GetHeight();
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);

	ResizeParentToFit(TRUE);
}

void CAviView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	if (m_Avi.IsValid())
	{
		switch (m_nMode)
		{
		case 0: // ID_MODE_NORMAL
			m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			break;
		case 1: // ID_MODE_DIFFERENCE
			if (m_nCurrentFrame > 0)
			{
				MatDib dib1, dib2;
				m_Avi.GetFrame(m_nCurrentFrame - 1, dib1);
				m_Avi.GetFrame(m_nCurrentFrame, dib2);

				MatDib dibGray1, dibGray2;
				DibToGray(dib1, dibGray1);
				DibToGray(dib2, dibGray2);

				MatDib dibDiff;
				DibDiff(dibDiff, dibGray1, dibGray2);

				dibDiff.Draw(pDC->m_hDC);
			}
			else
			{
				m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			}
			break;
		case 2:
			if (m_nCurrentFrame > 0)
			{
				MatDib dib1, dib2;
				m_Avi.GetFrame(m_nCurrentFrame - 1, dib1);
				m_Avi.GetFrame(m_nCurrentFrame, dib2);

				MatDib dibGray1, dibGray2;
				DibToGray(dib1, dibGray1);
				DibToGray(dib2, dibGray2);

				MatFullSearch fs;
				fs.SetImages(&dibGray1, &dibGray2);
				fs.FullSearch();

				MatDib dibMotion;
				fs.GetMotionImage(dibMotion);

				dibMotion.Draw(pDC->m_hDC);
			}
			else
			{
				m_Avi.DrawFrame(pDC->m_hDC, m_nCurrentFrame);
			}
			break;
		}
		
		// 메인 프레임 상태바에 프레임 번호 출력
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CString strText;
		strText.Format(_T("Frame: %d"), m_nCurrentFrame);
//		pFrame->m_wndStatusBar.SetPaneText(1, strText);
	}
}


// CAviView 진단

#ifdef _DEBUG
void CAviView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CAviView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAviView 메시지 처리기

BOOL CAviView::AviFileOpen(LPCTSTR lpszPathName)
{
	// AVI 파일을 열지 못한 경우, 메시지 출력하고 그냥 리턴
	if (!m_Avi.Open(lpszPathName))
	{
		AfxMessageBox(_T("Avi 파일을 열 수 없습니다."));
		return FALSE;
	}

	m_nCurrentFrame = 0;

	AfxPrintInfo(_T("[AVI 파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 전체 프레임 수: %d"), lpszPathName, m_Avi.GetWidth(), m_Avi.GetHeight(), m_Avi.GetTotalFrame());
	return TRUE;
}

void CAviView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		m_nCurrentFrame++;

		if (m_nCurrentFrame >= m_Avi.GetTotalFrame())
		{
			m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
			KillTimer(1);
			m_bPlay = FALSE;
		}

		Invalidate(FALSE);
	}
	else if (nIDEvent == 2)
	{
		
		Invalidate(FALSE);
	}
	CScrollView::OnTimer(nIDEvent);
}


void CAviView::OnVideoPlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_Avi.IsValid())
	{
		m_bPlay = TRUE;
		SetTimer(1, 1000 / m_Avi.GetFrameRate(), NULL);
	}
}

void CAviView::OnUpdateVideoPlay(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Avi.IsValid());
	pCmdUI->SetCheck(m_bPlay);
}

void CAviView::OnVideoPause()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPlay = FALSE;
	KillTimer(1);
}

void CAviView::OnUpdateVideoPause(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame != 0);
}

void CAviView::OnVideoStop()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bPlay = FALSE;
	KillTimer(1);
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}

void CAviView::OnUpdateVideoStop(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(!m_bPlay && m_nCurrentFrame == 0);
}

void CAviView::OnVideoStart()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nCurrentFrame = 0;
	Invalidate(FALSE);
}


void CAviView::OnVideoEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nCurrentFrame = m_Avi.GetTotalFrame() - 1;
	Invalidate(FALSE);
}

void CAviView::OnVideoPrev()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nCurrentFrame > 0)
		m_nCurrentFrame--;
	Invalidate(FALSE);
}

void CAviView::OnVideoNext()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nCurrentFrame < m_Avi.GetTotalFrame() - 1)
		m_nCurrentFrame++;
	Invalidate(FALSE);
}

void CAviView::OnVideoCapture()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	m_Avi.GetFrame(m_nCurrentFrame, dib);
	AfxPrintInfo(_T("[프레임 캡처] 프레임 번호: %d, 가로 크기: %d픽셀, 세로 크기: %d픽셀, 색상수: %d"), m_nCurrentFrame, dib.GetWidth(), dib.GetHeight(), 0x01 << dib.GetBitCount());
	AfxNewImage(dib);
}

BOOL CAviView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);
	return TRUE; // Erased
	//return CScrollView::OnEraseBkgnd(pDC);
}


void CAviView::OnModeNormal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nMode = 0;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nMode == 0);
}


void CAviView::OnModeDifference()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nMode = 1;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeDifference(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nMode == 1);
}


void CAviView::OnModeMotion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nMode = 2;
	Invalidate(FALSE);
}


void CAviView::OnUpdateModeMotion(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_nMode == 2);
}