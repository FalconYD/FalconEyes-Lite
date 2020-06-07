// ResizeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "ResizeDlg.h"
#include "afxdialogex.h"


// CResizeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResizeDlg, CDialogEx)

CResizeDlg::CResizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESIZE, pParent)
	, m_nOldWidth(0)
	, m_nOldHeight(0)
	, m_nNewWidth(0)
	, m_nNewHeight(0)
	, m_bAspectRatio(FALSE)
	, m_nInterpolation(0)
{

}

CResizeDlg::~CResizeDlg()
{
}

void CResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_OLD_WIDTH, m_nOldWidth);
	DDX_Text(pDX, IDC_STATIC_OLD_HEIGHT, m_nOldHeight);
	DDX_Text(pDX, IDC_EDIT_NEW_WIDTH, m_nNewWidth);
	DDX_Text(pDX, IDC_EDIT_NEW_HEIGHT, m_nNewHeight);
	DDX_Check(pDX, IDC_CHECK_ASPECT_RATIO, m_bAspectRatio);
	DDV_MinMaxInt(pDX, m_nNewWidth, 1, 100000);
	DDV_MinMaxInt(pDX, m_nNewHeight, 1, 100000);
	DDX_CBIndex(pDX, IDC_COMBO_INTERPORATION, m_nInterpolation);
}


BEGIN_MESSAGE_MAP(CResizeDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NEW_WIDTH, &CResizeDlg::OnEnChangeEditNewWidth)
	ON_EN_CHANGE(IDC_EDIT_NEW_HEIGHT, &CResizeDlg::OnEnChangeEditNewHeight)
	ON_BN_CLICKED(IDC_CHECK_ASPECT_RATIO, &CResizeDlg::OnBnClickedCheckAspectRatio)
END_MESSAGE_MAP()


// CResizeDlg 메시지 처리기입니다.

BOOL CResizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WIDTH))->SetRange(1, 32767);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_HEIGHT))->SetRange(1, 32767);

	m_nNewWidth = m_nOldWidth;
	m_nNewHeight = m_nOldHeight;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CResizeDlg::OnEnChangeEditNewWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (GetDlgItem(IDC_COMBO_INTERPORATION) == NULL)
		return;

	if (m_bAspectRatio)
	{
		UpdateData(TRUE);
		m_nNewHeight = static_cast<int>(m_nNewWidth * m_nOldHeight / static_cast<double>(m_nOldWidth));
		UpdateData(FALSE);
	}
}


void CResizeDlg::OnEnChangeEditNewHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (GetDlgItem(IDC_COMBO_INTERPORATION) == NULL)
		return;

	if (m_bAspectRatio)
	{
		UpdateData(TRUE);
		m_nNewWidth = static_cast<int>(m_nNewHeight * m_nOldWidth / static_cast<double>(m_nOldHeight));
		UpdateData(FALSE);
	}
}


void CResizeDlg::OnBnClickedCheckAspectRatio()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bAspectRatio)
	{
		m_nNewHeight = m_nNewWidth * m_nOldHeight / m_nOldWidth;
		UpdateData(FALSE);
	}
}