// GammaCorrectionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "GammaCorrectionDlg.h"
#include "afxdialogex.h"


// CGammaCorrectionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGammaCorrectionDlg, CDialogEx)

CGammaCorrectionDlg::CGammaCorrectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAMMA_CORRECTION, pParent)
	, m_fGamma(2.2f)
{

}

CGammaCorrectionDlg::~CGammaCorrectionDlg()
{
}

void CGammaCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_ctrlsliderGamma);
	DDX_Text(pDX, IDC_EDIT_GAMMA, m_fGamma);
	DDV_MinMaxFloat(pDX, m_fGamma, static_cast<float>(0.20), static_cast<float>(5.00));
}


BEGIN_MESSAGE_MAP(CGammaCorrectionDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_GAMMA, &CGammaCorrectionDlg::OnEnChangeEditGamma)
END_MESSAGE_MAP()


// CGammaCorrectionDlg 메시지 처리기입니다.
BOOL CGammaCorrectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlsliderGamma.SetRange(10, 250);
	m_ctrlsliderGamma.SetTicFreq(20);
	m_ctrlsliderGamma.SetPageSize(200);
	m_ctrlsliderGamma.SetPos((int)(m_fGamma * 50));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CGammaCorrectionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (m_ctrlsliderGamma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_ctrlsliderGamma.GetPos();
		m_fGamma = (pos / 50.f);
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGammaCorrectionDlg::OnEnChangeEditGamma()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_ctrlsliderGamma.SetPos((int)(m_fGamma * 50));
}