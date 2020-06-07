// RotateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "RotateDlg.h"
#include "afxdialogex.h"


// CRotateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRotateDlg, CDialogEx)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROTATE, pParent)
	, m_nRotate(0)
	, m_fAngle(0)
{

}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ROTATE90, m_nRotate);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fAngle);
	DDV_MinMaxFloat(pDX, m_fAngle, 0, 360);
}


BEGIN_MESSAGE_MAP(CRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_ROTATEFREE, &CRotateDlg::OnBnClickedRadioRotatefree)
	ON_EN_SETFOCUS(IDC_EDIT_ANGLE, &CRotateDlg::OnEnSetfocusEditAngle)
END_MESSAGE_MAP()


// CRotateDlg 메시지 처리기입니다.


void CRotateDlg::OnBnClickedRadioRotatefree()
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ANGLE);
	pEdit->SetSel(0, -1);
	pEdit->SetFocus();
}


void CRotateDlg::OnEnSetfocusEditAngle()
{
	// TODO: Add your control notification handler code here
	m_nRotate = 3;
	UpdateData(FALSE);
}
