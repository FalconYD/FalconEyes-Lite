// ThresholdDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "FalconEyes.h"
#include "FalconEyesDoc.h"
#include "ThresholdDlg.h"
#include "afxdialogex.h"


// ThresholdDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(ThresholdDlg, CDialogEx)

ThresholdDlg::ThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_THRESHOLD, pParent)
{

}

ThresholdDlg::~ThresholdDlg()
{
}

void ThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_TYPE1, m_cbThType1);
	DDX_Control(pDX, IDC_CB_TYPE2, m_cbThType2);
	DDX_Control(pDX, IDC_SLIDER_THRESH, m_sliderThresh);
	DDX_Control(pDX, IDC_SLIDER_THMAX, m_sliderThMax);
	DDX_Control(pDX, IDC_CB_MODE, m_cbThresholdMode);
	DDX_Control(pDX, IDC_CB_ADAPTYPE, m_cbAdaptive);
}


BEGIN_MESSAGE_MAP(ThresholdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_THRESHOLD, &ThresholdDlg::OnBnClickedBnThreshold)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_THRESH, &ThresholdDlg::OnEnChangeEditThresh)
	ON_EN_CHANGE(IDC_EDIT_THMAX, &ThresholdDlg::OnEnChangeEditThmax)
	ON_CBN_SELCHANGE(IDC_CB_MODE, &ThresholdDlg::OnCbnSelchangeCbMode)
END_MESSAGE_MAP()


// ThresholdDlg 메시지 처리기입니다.
BOOL ThresholdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cbThresholdMode.AddString(_T("Threshold"));
	m_cbThresholdMode.AddString(_T("Adaptive"));
	m_cbThresholdMode.SetCurSel(0);

	m_cbAdaptive.AddString(_T("ADAPTIVE_THRESH_MEAN_C"));
	m_cbAdaptive.AddString(_T("ADAPTIVE_THRESH_GAUSSIAN_C"));
	m_cbAdaptive.SetCurSel(1);

	m_cbThType1.AddString(_T("BINARY"));
	m_cbThType1.AddString(_T("BINARY_INV"));
	m_cbThType1.AddString(_T("TRUNC"));
	m_cbThType1.AddString(_T("TOZERO"));
	m_cbThType1.AddString(_T("TOZERO_INV"));
	m_cbThType1.SetCurSel(0);

	m_cbThType2.AddString(_T("NONE"));
	m_cbThType2.AddString(_T("MASK"));
	m_cbThType2.AddString(_T("OTSU"));
	m_cbThType2.AddString(_T("TRIANGLE"));
	m_cbThType2.SetCurSel(2);

	m_sliderThresh.SetRange(SLIDER_MIN, SLIDER_MAX);
	m_sliderThresh.SetPos(SLIDER_MIN);
	int nSlide = m_sliderThresh.GetPos();
	CString strTmp;
	strTmp.Format(_T("%d"), nSlide);
	GetDlgItem(IDC_EDIT_THRESH)->SetWindowTextW(strTmp);

	m_sliderThMax.SetRange(SLIDER_MIN, SLIDER_MAX);
	m_sliderThMax.SetPos(SLIDER_MAX);
	nSlide = m_sliderThMax.GetPos();
	strTmp.Format(_T("%d"), nSlide);
	GetDlgItem(IDC_EDIT_THMAX)->SetWindowTextW(strTmp);

	
	GetDlgItem(IDC_EDIT_BLOCKSIZE)->SetWindowText(_T("99"));
	GetDlgItem(IDC_EDIT_DOUBLEC)->SetWindowText(_T("10"));

	m_fn_EnableControl(true);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL ThresholdDlg::PreTranslateMessage(MSG* pMsg)
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


void ThresholdDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void ThresholdDlg::OnBnClickedBnThreshold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	Mat matDst;
	Mat matSrc = pDoc->m_Dib.GetMatImg();

	CString strBlockSize;
	CString strDoubleC;
	GetDlgItem(IDC_EDIT_BLOCKSIZE)->GetWindowText(strBlockSize);
	GetDlgItem(IDC_EDIT_DOUBLEC)->GetWindowText(strDoubleC);

	THRESHOLD_MODE enMode = static_cast<THRESHOLD_MODE>(m_cbThresholdMode.GetCurSel());
	ThresholdTypes enThreshtype;
	AdaptiveThresholdTypes enAdaptivetype;
	int nThresh = m_sliderThresh.GetPos();
	int nThreshMax = m_sliderThMax.GetPos();
	int nAdaptiveType = m_cbAdaptive.GetCurSel();
	int nThresholdType1 = m_cbThType1.GetCurSel();
	int nThresholdType2 = m_cbThType2.GetCurSel() + (ThresholdTypes::THRESH_MASK - 1);
	int nBlockSize = _ttoi(strBlockSize);
	double dDoubleC = _ttof(strDoubleC);
	
	
	if (enMode == THRESHOLD_MODE::ETM_THRESHOLD)
	{
		if (nThresholdType2 == (ThresholdTypes::THRESH_MASK - 1))
		{
			enThreshtype = static_cast<ThresholdTypes>(nThresholdType1);
		}
		else
		{
			enThreshtype = static_cast<ThresholdTypes>(nThresholdType1 | nThresholdType2);
		}
		cv::threshold(matSrc, matDst, nThresh, nThreshMax, enThreshtype);
	}
	else if(enMode == THRESHOLD_MODE::ETM_ADAPTIVE)
	{
		enThreshtype = static_cast<ThresholdTypes>(nThresholdType1);
		enAdaptivetype = static_cast<AdaptiveThresholdTypes>(nAdaptiveType);
		cv::adaptiveThreshold(matSrc, matDst, nThreshMax, enAdaptivetype, enThreshtype, nBlockSize, dDoubleC);
	}
	else
	{
		return;
	}
	MatDib dibDst;
	CString str = pDoc->GetTitle();
	CString strTemp;
	dibDst.SetMatImg(matDst);
	AfxPrintInfo(_T("[Threshold] %s Thresh : %d, Thresh Max : %d, Type1 : %d, Type2 : %d"), str, nThresh, nThreshMax, nThresholdType1, nThresholdType2);
	str.Append(_T("_Threshold_"));
	m_cbThType1.GetLBText(m_cbThType1.GetCurSel(), strTemp);
	str.Append(strTemp);
	str.Append(_T(" | "));
	m_cbThType2.GetLBText(m_cbThType2.GetCurSel(), strTemp);
	str.Append(strTemp);
	AfxNewImage(dibDst, str);
	this->OnOK();
}

void ThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_sliderThresh)
		{
			int nSlide = m_sliderThresh.GetPos();
			CString strTmp;
			strTmp.Format(_T("%d"), nSlide);
			GetDlgItem(IDC_EDIT_THRESH)->SetWindowTextW(strTmp);
		}
		if (pScrollBar == (CScrollBar*)&m_sliderThMax)
		{
			int nSlide = m_sliderThMax.GetPos();
			CString strTmp;
			strTmp.Format(_T("%d"), nSlide);
			GetDlgItem(IDC_EDIT_THMAX)->SetWindowTextW(strTmp);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void ThresholdDlg::OnEnChangeEditThresh()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;
	GetDlgItem(IDC_EDIT_THRESH)->GetWindowText(strTemp);
	int nSlider = _ttoi(strTemp);
	if (nSlider > -1 && nSlider < 256)
		m_sliderThresh.SetPos(nSlider);
}


void ThresholdDlg::OnEnChangeEditThmax()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;
	GetDlgItem(IDC_EDIT_THMAX)->GetWindowText(strTemp);
	int nSlider = _ttoi(strTemp);
	if (nSlider >= SLIDER_MIN && nSlider <= SLIDER_MAX)
		m_sliderThMax.SetPos(nSlider);
}


void ThresholdDlg::OnCbnSelchangeCbMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nMode = m_cbThresholdMode.GetCurSel();

	switch (nMode)
	{
	case THRESHOLD_MODE::ETM_THRESHOLD:
		m_fn_EnableControl(true);
		break;
	case THRESHOLD_MODE::ETM_ADAPTIVE:
		m_fn_EnableControl(false);
		break;
	default:
		break;
	}
}

void ThresholdDlg::m_fn_EnableControl(bool bMod)
{
	m_cbAdaptive.EnableWindow(!bMod);
	m_cbThType2.EnableWindow(bMod);
	GetDlgItem(IDC_EDIT_THRESH)->EnableWindow(bMod);
	GetDlgItem(IDC_EDIT_DOUBLEC)->EnableWindow(!bMod);
	GetDlgItem(IDC_EDIT_BLOCKSIZE)->EnableWindow(!bMod);
	m_sliderThresh.EnableWindow(bMod);
}
