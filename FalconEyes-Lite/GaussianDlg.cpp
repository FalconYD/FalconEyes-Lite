// GaussianDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "GaussianDlg.h"
#include "afxdialogex.h"


// CGaussianDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAUSSIAN, pParent)
	, m_fSigma(1.40f)
{

}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SIGMA, m_ctrlsliderGaussian);
	DDX_Text(pDX, IDC_EDIT_SIGMA, m_fSigma);
	DDV_MinMaxFloat(pDX, m_fSigma, 0.20f, 5.00f);
}


BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_SIGMA, &CGaussianDlg::OnEnChangeEditSigma)
END_MESSAGE_MAP()


// CGaussianDlg 메시지 처리기입니다.

void CGaussianDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if (m_ctrlsliderGaussian.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_ctrlsliderGaussian.GetPos();
		m_fSigma = (pos / 50.0f);
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CGaussianDlg::OnEnChangeEditSigma()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	m_ctrlsliderGaussian.SetPos(static_cast<int>(m_fSigma * 50));
}


BOOL CGaussianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlsliderGaussian.SetRange(10, 250);
	m_ctrlsliderGaussian.SetTicFreq(20);
	m_ctrlsliderGaussian.SetPageSize(20);
	m_ctrlsliderGaussian.SetPos(static_cast<int>(m_fSigma * 50));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
