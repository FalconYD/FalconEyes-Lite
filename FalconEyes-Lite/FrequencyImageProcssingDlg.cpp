// FrequencyImageProcssingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "FalconEyesDoc.h"
#include "MainFrm.h"
#include "FrequencyImageProcssingDlg.h"
#include "afxdialogex.h"


// FrequencyImageProcssingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(FrequencyImageProcssingDlg, CDialogEx)

FrequencyImageProcssingDlg::FrequencyImageProcssingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FREQUENCY, pParent)
{
	m_pDoc = nullptr;
}

FrequencyImageProcssingDlg::~FrequencyImageProcssingDlg()
{
}

void FrequencyImageProcssingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FrequencyImageProcssingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_DFT, &FrequencyImageProcssingDlg::OnBnClickedBnDft)
	ON_BN_CLICKED(IDC_BN_IDFT, &FrequencyImageProcssingDlg::OnBnClickedBnIdft)
	ON_BN_CLICKED(IDC_BN_FILTER_MERGE, &FrequencyImageProcssingDlg::OnBnClickedBnFilter)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BN_CREATE_FILTER, &FrequencyImageProcssingDlg::OnBnClickedBnCreateFilter)
END_MESSAGE_MAP()


// FrequencyImageProcssingDlg 메시지 처리기입니다.

BOOL FrequencyImageProcssingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void FrequencyImageProcssingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL FrequencyImageProcssingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

void FrequencyImageProcssingDlg::OnBnClickedBnDft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MatDib dibDst;
	Mat matTemp;
	CString strTitle;
	m_pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	strTitle = m_pDoc->GetTitle();
	m_matSrc = m_pDoc->m_Dib.GetMatImg();
	if (m_matSrc.channels() != 1)
	{
		AfxPrintInfo(L"[Frequency Dlg] Source Image Channel : %d, Must Grayscale.", m_matSrc.channels());
		return;
	}

	if (m_matSrc.cols % 2 != 0 || m_matSrc.rows % 2 != 0)
	{
		AfxPrintInfo(L"[Frequency Dlg] Source Image Size Must Even Number.");
		return;
	}
	int cx = m_matSrc.cols / 2;
	int cy = m_matSrc.rows / 2;

	m_matDst = Mat::zeros(cv::Size(m_matSrc.cols, m_matSrc.rows), CV_8UC1);
	mc_timer.Reset();
	m_matSrc.convertTo(m_matFourier, CV_32F);
	dft(m_matFourier, m_matFourier, DFT_SCALE | DFT_COMPLEX_OUTPUT);
	split(m_matFourier, m_matPlane);
	
	Mat temp[2];
	merge(m_matPlane, 2, temp[0]);
	cartToPolar(m_matPlane[0], m_matPlane[1], temp[0], temp[1]);
	
	matTemp = temp[0].clone();
	log(matTemp, matTemp);

	Mat q0(matTemp, Rect(0, 0, cx, cy));
	Mat q1(matTemp, Rect(cx, 0, cx, cy));
	Mat q2(matTemp, Rect(0, cy, cx, cy));
	Mat q3(matTemp, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(matTemp, matTemp, 0, 1, NormTypes::NORM_MINMAX);

	matTemp.convertTo(m_matDst, CV_8U, 255);

	AfxPrintInfo(_T("[Frequency Dlg] DFT 입력 영상: %s (%d ms)"), strTitle, mc_timer.Elapse());
	dibDst.SetMatImg(m_matDst);
	strTitle.Append(_T("_DFT"));
	AfxNewImage(dibDst, strTitle);
}

void FrequencyImageProcssingDlg::OnBnClickedBnFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	m_matFilter = m_pDoc->m_Dib.GetMatImg();
	AfxPrintInfo(_T("[Frequency Dlg] Filter Set."));
}

void FrequencyImageProcssingDlg::OnBnClickedBnIdft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTitle = m_pDoc->GetTitle();
	MatDib dibDst;
	if (m_matFilter.empty())
		return;
 	m_matFilter.convertTo(m_matFilter, CV_32F, 1.0 / 255);
	int cx = m_matFilter.cols / 2;
	int cy = m_matFilter.rows / 2;
	
	Mat q0(m_matFilter, Rect(0, 0, cx, cy));
	Mat q1(m_matFilter, Rect(cx, 0, cx, cy));
	Mat q2(m_matFilter, Rect(0, cy, cx, cy));
	Mat q3(m_matFilter, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	Mat Fourier;
	split(m_matFourier, m_matPlane);
	multiply(m_matPlane[0], m_matFilter, m_matPlane[0]);
	multiply(m_matPlane[1], m_matFilter, m_matPlane[1]);
 	merge(m_matPlane, 2, Fourier);
	
	dft(Fourier, Fourier, DFT_INVERSE | DFT_REAL_OUTPUT);

	Fourier.convertTo(m_matDst, CV_8U);
	
	dibDst.SetMatImg(m_matDst);
	AfxPrintInfo(_T("[Frequency Dlg] IDFT 입력 영상: %s"), strTitle);
	strTitle.Append(_T("_IDFT"));
	AfxNewImage(dibDst, strTitle);
}



void FrequencyImageProcssingDlg::OnBnClickedBnCreateFilter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDoc != nullptr)
	{
		MatDib dibDst;
		dibDst.CreateGrayImage(m_pDoc->m_Dib.GetWidth(), m_pDoc->m_Dib.GetHeight());
		BYTE* pData = dibDst.GetDIBitsAddr();
		for (int nStep = 0; nStep < (dibDst.GetHeight() * dibDst.GetWidth()); nStep++)
		{
			pData[nStep] = 255;
		}
		AfxPrintInfo(_T("[Frequency Dlg] Create Filter"));
		AfxNewImage(dibDst);
	}
	else
		AfxPrintInfo(_T("[Frequency Dlg] Create Filter Fail."));
}

void FrequencyImageProcssingDlg::m_fn_EnableButton(bool bEnable)
{
	GetDlgItem(IDC_BN_CREATE_FILTER)->EnableWindow(bEnable);

}