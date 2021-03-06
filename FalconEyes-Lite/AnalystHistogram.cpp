// AnalystHistogram.cpp: 구현 파일
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "AnalystHistogram.h"
#include "MainFrm.h"
#include "FalconEyesDoc.h"
#include "FalconEyesView.h"
#include "afxdialogex.h"


// AnalystHistogram 대화 상자

IMPLEMENT_DYNAMIC(AnalystHistogram, CDialogEx)

AnalystHistogram::AnalystHistogram(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANALYST_HISTOGRAM, pParent)
{

}

AnalystHistogram::~AnalystHistogram()
{
}

void AnalystHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AnalystHistogram, CDialogEx)
	ON_STN_DBLCLK(IDC_GRAPH, &AnalystHistogram::OnStnDblclickGraph)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// AnalystHistogram 메시지 처리기

BOOL AnalystHistogram::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
 	CRect rect;
 	
 	GetDlgItem(IDC_GRAPH)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pCtrlScope = new COScopeCtrl(TREND_SIZE);
 	m_pCtrlScope->Create(WS_VISIBLE | WS_CHILD, rect, this, IDC_GRAPH);

	m_pCtrlScope->SetRange(0, 255, GRAY_INDEX);
	m_pCtrlScope->SetRange(0, 255, RED_INDEX);
	m_pCtrlScope->SetRange(0, 255, GREEN_INDEX);
	m_pCtrlScope->SetRange(0, 255, BLUE_INDEX);

	m_pCtrlScope->SetXUnits(_T("Histogram"));
	m_pCtrlScope->SetLegendLabel(_T("Gray"), GRAY_INDEX);
	m_pCtrlScope->SetLegendLabel(_T("Red"), RED_INDEX);
	m_pCtrlScope->SetLegendLabel(_T("Green"), GREEN_INDEX);
	m_pCtrlScope->SetLegendLabel(_T("Blue"), BLUE_INDEX);

	m_pGrayData = nullptr;
	m_pRedData = nullptr;
	m_pGreenData = nullptr;
	m_pBlueData = nullptr;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void AnalystHistogram::OnStnDblclickGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	CFalconEyesView* pView = (CFalconEyesView*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveView();

	unsigned char nMin[4], nMax[4];
	float fAvg[4];
	memset(nMin, 255, sizeof(nMin[0]) * 4);
	memset(nMax, 0x00, sizeof(nMax[0]) * 4);
	memset(fAvg, 0, sizeof(fAvg[0]) * 4);
	if (pDoc != NULL)
	{
		m_pCtrlScope->Reset();
		m_pntHisto1 = pView->m_pntHisto1;
		m_pntHisto2 = pView->m_pntHisto2;
		m_matBack = pDoc->m_matBuffer[0];
		int i;
		int nHistoLen;
		int nStartPos;
		int nW = m_matBack.cols;
		if (m_pGrayData != NULL)
		{
			delete m_pGrayData;
			m_pGrayData = NULL;
		}
		if (m_pRedData != nullptr)
		{
			delete m_pRedData;
			m_pRedData = NULL;
		}
		if (m_pGreenData != nullptr)
		{
			delete m_pGreenData;
			m_pGreenData = NULL;
		}
		if (m_pBlueData != nullptr)
		{
			delete m_pBlueData;
			m_pBlueData = NULL;
		}

		if (m_matBack.channels() == 1)
		{
			if (m_pntHisto1.y == m_pntHisto2.y)
			{
				nStartPos = m_pntHisto1.x < m_pntHisto2.x ? m_pntHisto1.x : m_pntHisto2.x;
				nHistoLen = abs(m_pntHisto1.x - m_pntHisto2.x);

				m_pGrayData = new BYTE[nHistoLen];
				m_pRedData = new BYTE[nHistoLen];
				m_pGreenData = new BYTE[nHistoLen];
				m_pBlueData = new BYTE[nHistoLen];

				memset(m_pGrayData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pRedData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pGreenData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pBlueData, 0x00, sizeof(BYTE)*nHistoLen);

				memcpy(m_pGrayData, &(m_matBack.data[m_pntHisto1.y * nW + nStartPos]), sizeof(BYTE)*nHistoLen);
			}
			else
			{
				nStartPos = m_pntHisto1.y < m_pntHisto2.y ? m_pntHisto1.y : m_pntHisto2.y;
				nHistoLen = abs(m_pntHisto1.y - m_pntHisto2.y);
				m_pGrayData = new BYTE[nHistoLen];
				m_pRedData = new BYTE[nHistoLen];
				m_pGreenData = new BYTE[nHistoLen];
				m_pBlueData = new BYTE[nHistoLen];

				memset(m_pGrayData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pRedData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pGreenData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pBlueData, 0x00, sizeof(BYTE)*nHistoLen);

				for (i = 0; i < nHistoLen; i++)
				{
					m_pGrayData[i] = m_matBack.data[(nStartPos + i) * nW + m_pntHisto1.x];
				}
			}
			for (i = 1; i < nHistoLen-1; ++i)
			{
				if (nMin[enColor::GRAY_INDEX] >= m_pGrayData[i])
					nMin[enColor::GRAY_INDEX] = m_pGrayData[i];
				if (nMax[enColor::GRAY_INDEX] <= m_pGrayData[i])
					nMax[enColor::GRAY_INDEX] = m_pGrayData[i];
				fAvg[enColor::GRAY_INDEX] += static_cast<float>(m_pGrayData[i] / (double)nHistoLen);
			}
			m_pCtrlScope->DrawGraph(m_pGrayData, nHistoLen, 0);
		}
		else
		{
			if (m_matBack.channels() == 4)
				cvtColor(m_matBack, m_matBack, ColorConversionCodes::COLOR_BGRA2BGR);
			Mat matR, matG, matB, matY, matU, matV;
			MatColorSplitRGB(m_matBack, matR, matG, matB);
			MatColorSplitYUV(m_matBack, matY, matU, matV);
			matU.release();
			matV.release();
			if (m_pntHisto1.y == m_pntHisto2.y)
			{
				nStartPos = m_pntHisto1.x < m_pntHisto2.x ? m_pntHisto1.x : m_pntHisto2.x;
				nHistoLen = abs(m_pntHisto1.x - m_pntHisto2.x);
				m_pGrayData = new BYTE[nHistoLen];
				m_pRedData = new BYTE[nHistoLen];
				m_pGreenData = new BYTE[nHistoLen];
				m_pBlueData = new BYTE[nHistoLen];

				memset(m_pGrayData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pRedData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pGreenData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pBlueData, 0x00, sizeof(BYTE)*nHistoLen);

				memcpy(m_pGrayData, &(matY.data[m_pntHisto1.y * nW + nStartPos]), sizeof(BYTE)*nHistoLen);
				memcpy(m_pRedData, &(matR.data[m_pntHisto1.y * nW + nStartPos]), sizeof(BYTE)*nHistoLen);
				memcpy(m_pGreenData, &(matG.data[m_pntHisto1.y * nW + nStartPos]), sizeof(BYTE)*nHistoLen);
				memcpy(m_pBlueData, &(matB.data[m_pntHisto1.y * nW + nStartPos]), sizeof(BYTE)*nHistoLen);
			}
			else
			{
				nStartPos = m_pntHisto1.y < m_pntHisto2.y ? m_pntHisto1.y : m_pntHisto2.y;
				nHistoLen = abs(m_pntHisto1.y - m_pntHisto2.y);
				m_pGrayData = new BYTE[nHistoLen];
				m_pRedData = new BYTE[nHistoLen];
				m_pGreenData = new BYTE[nHistoLen];
				m_pBlueData = new BYTE[nHistoLen];

				memset(m_pGrayData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pRedData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pGreenData, 0x00, sizeof(BYTE)*nHistoLen);
				memset(m_pBlueData, 0x00, sizeof(BYTE)*nHistoLen);

				for (i = 0; i < nHistoLen; i++)
				{
					m_pGrayData[i] = matY.data[(nStartPos + i) * nW + m_pntHisto1.x];
					m_pRedData[i] = matR.data[(nStartPos + i) * nW + m_pntHisto1.x];
					m_pGreenData[i] = matG.data[(nStartPos + i) * nW + m_pntHisto1.x];
					m_pBlueData[i] = matB.data[(nStartPos + i) * nW + m_pntHisto1.x];
				}
			}
			for (i = 1; i < nHistoLen-1; ++i)
			{
				if (nMin[enColor::GRAY_INDEX] >= m_pGrayData[i])
					nMin[enColor::GRAY_INDEX] = m_pGrayData[i];
				if (nMax[enColor::GRAY_INDEX] <= m_pGrayData[i])
					nMax[enColor::GRAY_INDEX] = m_pGrayData[i];
				fAvg[enColor::GRAY_INDEX] += static_cast<float>(m_pGrayData[i] / (double)nHistoLen);

				if (nMin[enColor::RED_INDEX] >= m_pRedData[i])
					nMin[enColor::RED_INDEX] = m_pRedData[i];
				if (nMax[enColor::RED_INDEX] <= m_pRedData[i])
					nMax[enColor::RED_INDEX] = m_pRedData[i];
				fAvg[enColor::RED_INDEX] += static_cast<float>(m_pRedData[i] / (double)nHistoLen);

				if (nMin[enColor::GREEN_INDEX] >= m_pGreenData[i])
					nMin[enColor::GREEN_INDEX] = m_pGreenData[i];
				if (nMax[enColor::GREEN_INDEX] <= m_pGreenData[i])
					nMax[enColor::GREEN_INDEX] = m_pGreenData[i];
				fAvg[enColor::GREEN_INDEX] += static_cast<float>(m_pGreenData[i] / (double)nHistoLen);

				if (nMin[enColor::BLUE_INDEX] >= m_pBlueData[i])
					nMin[enColor::BLUE_INDEX] = m_pBlueData[i];
				if (nMax[enColor::BLUE_INDEX] <= m_pBlueData[i])
					nMax[enColor::BLUE_INDEX] = m_pBlueData[i];
				fAvg[enColor::BLUE_INDEX] += static_cast<float>(m_pBlueData[i] / (double)nHistoLen);
			}
			m_pCtrlScope->DrawGraph(m_pGrayData, nHistoLen, GRAY_INDEX);
			m_pCtrlScope->DrawGraph(m_pRedData, nHistoLen, RED_INDEX);
			m_pCtrlScope->DrawGraph(m_pGreenData, nHistoLen, GREEN_INDEX);
			m_pCtrlScope->DrawGraph(m_pBlueData, nHistoLen, BLUE_INDEX);
			
		}
		AfxPrintInfo(_T("[Histogram 분석] 데이터길이 : %d, Min Lv(Grgb) : %d %d %d %d, Max Lv(Grgb) : %d %d %d %d, Avg Lv(Grgb) : %.2lf %.2lf %.2lf %.2lf"),
			nHistoLen, nMin[enColor::GRAY_INDEX], nMin[enColor::RED_INDEX], nMin[enColor::GREEN_INDEX], nMin[enColor::BLUE_INDEX],
			nMax[enColor::GRAY_INDEX], nMax[enColor::RED_INDEX], nMax[enColor::GREEN_INDEX], nMax[enColor::BLUE_INDEX],
			fAvg[enColor::GRAY_INDEX], fAvg[enColor::RED_INDEX], fAvg[enColor::GREEN_INDEX], fAvg[enColor::BLUE_INDEX]);
	}
}

void AnalystHistogram::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pCtrlScope != nullptr)
	{
		delete m_pCtrlScope;
		m_pCtrlScope = nullptr;
	}
	if (m_pGrayData != nullptr)
	{
		delete m_pGrayData;
		m_pGrayData = nullptr;
	}
	if (m_pRedData != nullptr)
	{
		delete m_pRedData;
		m_pRedData = nullptr;
	}
	if (m_pGreenData != nullptr)
	{
		delete m_pGreenData;
		m_pGreenData = nullptr;
	}
	if (m_pBlueData != nullptr)
	{
		delete m_pBlueData;
		m_pBlueData = nullptr;
	}
}


BOOL AnalystHistogram::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}