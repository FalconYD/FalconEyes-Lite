// CFourierDescDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "CFourierDescDlg.h"
#include "afxdialogex.h"


// CFourierDescDlg 대화 상자

IMPLEMENT_DYNAMIC(CFourierDescDlg, CDialogEx)

CFourierDescDlg::CFourierDescDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOURIER_DESCRIPTOR, pParent)
	, m_nPercent(100)
{

}

CFourierDescDlg::~CFourierDescDlg()
{
}

void CFourierDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PERCENT, m_nPercent);
}


BEGIN_MESSAGE_MAP(CFourierDescDlg, CDialogEx)
END_MESSAGE_MAP()


// CFourierDescDlg 메시지 처리기
