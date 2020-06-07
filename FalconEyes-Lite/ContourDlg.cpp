// ContourDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "FalconTimer.h"
#include "FalconEyes.h"
#include "FalconEyesDoc.h"
#include "ContourDlg.h"
#include "afxdialogex.h"


// ContourDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(ContourDlg, CDialogEx)

ContourDlg::ContourDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_CONTOUR, pParent)
{

}

ContourDlg::~ContourDlg()
{
}

void ContourDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ContourDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_CONTOUR, &ContourDlg::OnBnClickedBnContour)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// ContourDlg 메시지 처리기입니다.


void ContourDlg::OnBnClickedBnContour()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	CString strVal;
	CString strDistance;
	GetDlgItem(IDC_EDIT_APPROXVAL)->GetWindowText(strVal);
	GetDlgItem(IDC_EDIT_DISTANCE)->GetWindowText(strDistance);
	double dApproxVal = _ttof(strVal);
	double dDistance = _ttof(strDistance);
	m_timer.Reset();
	Mat matDst;
	m_fn_Contour(pDoc->m_Dib.GetMatImg(), matDst, m_vecContours, dApproxVal, dDistance);

	int nIndex = 0;
	int nVal = 0;
	double dMax = 0;
	nVal = static_cast<int>(m_vecContours[0].size());
	for (int nStep = 0; nStep < m_vecContours.size(); nStep++)
	{
		double dArea = contourArea(m_vecContours[nStep], false);
		if (dMax < dArea)
		{
			dMax = dArea;
			nIndex = nStep;
		}
	}

	AfxPrintInfo(_T("[Contour] Title : %s, Object Count : %d, MaxArea : %.2f ( %d ms )"), pDoc->GetTitle(), m_nObjCount, dMax, m_timer.Elapse());

	CString str = pDoc->GetTitle();
	str.Append(_T("_Contour"));
	MatDib dib;
	dib.SetMatImg(matDst);
	AfxNewImage(dib, str);

	this->OnOK();
}

void ContourDlg::m_fn_Contour(Mat Src, Mat& Dst, std::vector<std::vector<cv::Point>>& vec, double dApprox, double dDis)
{
	Mat matSrc = Src;
	Mat matDst = Mat::zeros(cv::Size(matSrc.cols, matSrc.rows), CV_8UC3);
	std::vector<std::vector<cv::Point>> vecContour;
	if (matSrc.channels() != 1)
		cvtColor(matSrc, matSrc, ColorConversionCodes::COLOR_BGR2GRAY);
	findContours(matSrc, vecContour, RetrievalModes::RETR_EXTERNAL, ContourApproximationModes::CHAIN_APPROX_SIMPLE);
	
// 	if (m_vecContours.size() > 0)
// 	{
// 		for (int i = 0; i < m_vecContours.size(); i++)
// 		{
// 			m_listContour.AddString(_T(""));
// 		}
// 	}

	std::vector<cv::Point2f> vecApprox;
	double dArcLen;
	m_nObjCount = static_cast<int>(vecContour.size());
	for (int i = 0; i < vecContour.size(); i++)
	{
		dArcLen = arcLength(Mat(vecContour[i]), true)*dApprox;
		approxPolyDP(vecContour[i], vecApprox, dArcLen, true);

		for (int j = 1; j < vecApprox.size(); j++)
		{
			if (std::sqrt(std::pow(vecApprox[j].x - vecApprox[j - 1].x,2) + std::pow(vecApprox[j].y-vecApprox[j-1].y,2)) < dDis)
			{
				circle(matDst, vecApprox[j-1], 5, Scalar(0, 0, 255));
				circle(matDst, vecApprox[j], 5, Scalar(0, 0, 255));
			}
			else
				circle(matDst, vecApprox[j], 5, Scalar(0, 255, 0));
		}
	}
	
	drawContours(matDst, vecContour, -1, Scalar(200, 200, 200));
	Dst = matDst.clone();
	vec = vecContour;
	//m_vecContours.~vector();
	
}

BOOL ContourDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_EDIT_APPROXVAL)->SetWindowText(_T("0.005"));
	GetDlgItem(IDC_EDIT_DISTANCE)->SetWindowText(_T("10"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL ContourDlg::PreTranslateMessage(MSG* pMsg)
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


void ContourDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
