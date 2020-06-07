// BrightnessConstrastDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "BrightnessConstrastDlg.h"
#include "afxdialogex.h"


// CBrightnessConstrastDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBrightnessConstrastDlg, CDialogEx)

CBrightnessConstrastDlg::CBrightnessConstrastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BRIGHTNESS_CONTRAST, pParent)
	, m_nBrightness(0)
	, m_nContrast(0)
{

}

CBrightnessConstrastDlg::~CBrightnessConstrastDlg()
{
}

void CBrightnessConstrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_ctrlsliderBrightness);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_ctrlsliderContrast);
	DDX_Text(pDX, IDC_EDIT_BRIGTNESS, m_nBrightness);
	DDV_MinMaxInt(pDX, m_nBrightness, -255, 255);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_nContrast);
	DDV_MinMaxInt(pDX, m_nContrast, -100, 100);
}


BEGIN_MESSAGE_MAP(CBrightnessConstrastDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_BRIGTNESS, &CBrightnessConstrastDlg::OnEnChangeEditBrigtness)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, &CBrightnessConstrastDlg::OnEnChangeEditContrast)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CBrightnessConstrastDlg 메시지 처리기입니다.

BOOL CBrightnessConstrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlsliderBrightness.SetRange(-255, 255);
	m_ctrlsliderBrightness.SetTicFreq(32);
	m_ctrlsliderBrightness.SetPageSize(32);

	m_ctrlsliderContrast.SetRange(-100, 100);
	m_ctrlsliderContrast.SetTicFreq(20);
	m_ctrlsliderContrast.SetPageSize(20);
	return TRUE;  
}

void CBrightnessConstrastDlg::OnEnChangeEditBrigtness()
{
	UpdateData(TRUE);
	m_ctrlsliderBrightness.SetPos(m_nBrightness);
}


void CBrightnessConstrastDlg::OnEnChangeEditContrast()
{

	UpdateData(TRUE);
	m_ctrlsliderContrast.SetPos(m_nContrast);
}


void CBrightnessConstrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar->GetSafeHwnd() == m_ctrlsliderBrightness.GetSafeHwnd())
	{
		m_nBrightness = m_ctrlsliderBrightness.GetPos();
		UpdateData(FALSE);
	}
	if (pScrollBar->GetSafeHwnd() == m_ctrlsliderContrast.GetSafeHwnd())
	{
		m_nContrast = m_ctrlsliderContrast.GetPos();
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
