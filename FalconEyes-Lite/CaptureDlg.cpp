// CaptureDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"


// CaptureDlg 대화 상자

IMPLEMENT_DYNAMIC(CaptureDlg, CDialogEx)

CaptureDlg::CaptureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CAPTURE, pParent)
{

}

CaptureDlg::~CaptureDlg()
{
}

void CaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CaptureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_CAPTURE, &CaptureDlg::OnBnClickedBnCapture)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CaptureDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHK_TOPMOST, &CaptureDlg::OnBnClickedChkTopmost)
END_MESSAGE_MAP()


// CaptureDlg 메시지 처리기


void CaptureDlg::OnBnClickedBnCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MatDib dib;
	Mat img = m_fn_GetMatImage();
	dib.SetMatImg(img);
	AfxNewImage(dib);
}

void CaptureDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LONG lResult = SetWindowLong(this->GetSafeHwnd()
		, GWL_EXSTYLE
		, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);

	bool bResult = SetLayeredWindowAttributes(RGB(255, 255, 255), 200, LWA_COLORKEY | LWA_ALPHA);

	if (!bResult) {

		AfxMessageBox(_T("Fail to SetLayeredWindowAttributes..."));
		return FALSE;
	}

	((CButton*)GetDlgItem(IDC_CHK_TOPMOST))->SetCheck(true);
	bool bChk = ((CButton*)GetDlgItem(IDC_CHK_TOPMOST))->GetCheck();
	m_fn_TopMost(bChk);

	SetTimer(0, 500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CaptureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rectPic;
	CStatic* pPic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	if (pPic != nullptr)
	{
		pPic->GetWindowRect(&rectPic);
		
		ScreenToClient(&rectPic);
		rectPic.right = cx - rectPic.left - 10;
		rectPic.bottom = cy - rectPic.top - 10;
		pPic->SetWindowPos(NULL, rectPic.left, rectPic.top, rectPic.right, rectPic.bottom, SWP_NOZORDER);
	}
}


void CaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	double dValue = 0.0;
	Mat temp;
	TCHAR strValue[MAX_PATH] = { 0, };
	switch (nIDEvent)
	{
	case 0:
		temp = m_fn_GetMatImage();
		dValue = m_fn_CalcCR(temp);
		_stprintf_s(strValue, _T("%.2lf"), dValue);
		GetDlgItem(IDC_STATIC_CR)->SetWindowText(strValue);

		dValue = m_fn_CalcStd(temp);
		_stprintf_s(strValue, _T("%.2lf"), dValue);
		GetDlgItem(IDC_STATIC_STD)->SetWindowText(strValue);

		dValue = m_fn_CalcEdge(temp);
		_stprintf_s(strValue, _T("%.2lf"), dValue);
		GetDlgItem(IDC_STATIC_Edge)->SetWindowText(strValue);
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}

double CaptureDlg::m_fn_CalcCR(Mat matInput)
{
	Mat Out = matInput.clone();
	double dMin, dMax;
	cv::minMaxLoc(Out, &dMin, &dMax);
	if (dMin == 0.0)
		dMin = 1;
	return dMax / dMin;
}

double CaptureDlg::m_fn_CalcStd(Mat matInput)
{
	Mat Out = matInput.clone();
	Mat matMean, matStd;
	cv::meanStdDev(Out, matMean, matStd);
	double temp = matStd.at<double>(0, 0);
	return temp;
}

double CaptureDlg::m_fn_CalcEdge(Mat matInput)
{
	Mat Out = matInput.clone();
	cvtColor(Out, Out, COLOR_BGR2GRAY);
	Canny(Out, Out, 128, 255);
	
	return (double)cv::countNonZero(Out);
}

Mat CaptureDlg::m_fn_GetMatImage()
{
	CWnd* hWnd = GetDesktopWindow();
	CDC* pDC = hWnd->GetWindowDC();
	CRect rect, rectStatic;

	pDC->GetWindow()->GetClientRect(rect);
	int cx = rect.right - rect.left;
	int cy = rect.bottom - rect.top;
	if (cx <= 0 || cy <= 0) return Mat::zeros(Size(1,1),CV_8UC1);
	HDC hMemDC;
	hMemDC = CreateCompatibleDC(pDC->m_hDC);
	BITMAPINFO    bmi;
	BITMAPINFOHEADER* bmih = &(bmi.bmiHeader);
	LPVOID        pBits;
	HBITMAP        hBitmap;
	ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biBitCount = 24;
	bmih->biWidth = cx;
	bmih->biHeight = cy;
	bmih->biPlanes = 1;
	hBitmap = CreateDIBSection(pDC->m_hDC, &bmi, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, cx, cy, pDC->m_hDC, rect.left, rect.top, SRCCOPY);
	DeleteDC(hMemDC);

	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rectStatic);
	if (rectStatic.left < 0)
		rectStatic.left = 0;
	if (rectStatic.top < 0)
		rectStatic.top = 0;
	if (rectStatic.right > rect.right)
		rectStatic.right = rect.right;
	if (rectStatic.bottom > rect.bottom)
		rectStatic.bottom = rect.bottom;
	hMemDC = NULL;
	bool bMustRelease = false;
	const int nDepth = CV_8UC3;
	const int nChannels = 3;

	if (hMemDC == NULL)
	{
		hMemDC = ::GetDC(NULL);
		bMustRelease = true;
	}
	Mat img;
	if (GetDIBits(hMemDC, hBitmap, 0, 0, NULL, &bmi, DIB_RGB_COLORS))
	{
		int nHeight = (bmih->biHeight > 0) ? bmih->biHeight : -bmih->biHeight;
		img = Mat::zeros(Size(bmih->biWidth, nHeight), nDepth);
		//img->origin = (bmih->biHeight > 0);
		bmih->biBitCount = (WORD)(img.channels() * 8);
		bmih->biCompression = BI_RGB;
		GetDIBits(hMemDC, hBitmap, 0, nHeight, img.data, &bmi, DIB_RGB_COLORS);
	}
	if (bMustRelease) ::ReleaseDC(NULL, hMemDC);
	DeleteObject(hBitmap);
	ReleaseDC(pDC);
	hWnd->DestroyWindow();
	flip(img, img, 0);
	Mat temp = img(Rect(rectStatic.left, rectStatic.top, rectStatic.Width(), rectStatic.Height())).clone();
	return temp;
}

void CaptureDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_fn_TopMost(false);
	CDialogEx::OnOK();
	
}

void CaptureDlg::m_fn_TopMost(bool bTopmost)
{
	if (bTopmost)
		::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	else
		::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
}

void CaptureDlg::OnBnClickedChkTopmost()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool bChk = ((CButton*)GetDlgItem(IDC_CHK_TOPMOST))->GetCheck();
	m_fn_TopMost(bChk);
}
