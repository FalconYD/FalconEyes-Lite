// CannyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "CannyDlg.h"
#include "afxdialogex.h"


// CCannyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCannyDlg, CDialogEx)

CCannyDlg::CCannyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CANNY_EDGE, pParent)
	, m_fSigma(1.4f)
	, m_fLowTh(30.f)
	, m_fHighTh(60.f)
{

}

CCannyDlg::~CCannyDlg()
{
}

void CCannyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CANNYSIGMA, m_fSigma);
	DDX_Text(pDX, IDC_EDIT_LOWTHRESHOLD, m_fLowTh);
	DDX_Text(pDX, IDC_EDIT_HIGHTHRESHOLD, m_fHighTh);
}


BEGIN_MESSAGE_MAP(CCannyDlg, CDialogEx)
END_MESSAGE_MAP()


// CCannyDlg 메시지 처리기입니다.
