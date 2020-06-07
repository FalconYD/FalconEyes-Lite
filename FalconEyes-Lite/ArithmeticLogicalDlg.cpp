// ArithmeticLogicalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "FalconEyesDoc.h"
#include "ArithmeticLogicalDlg.h"
#include "afxdialogex.h"


// CArithmeticLogicalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CArithmeticLogicalDlg, CDialogEx)

CArithmeticLogicalDlg::CArithmeticLogicalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ARITHMETIC_LOGICAL, pParent)
	, m_nFunc(0), m_pDoc1(NULL), m_pDoc2(NULL)
{

}

CArithmeticLogicalDlg::~CArithmeticLogicalDlg()
{
}

void CArithmeticLogicalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IMAGE1, m_ctrlcomboImage1);
	DDX_Control(pDX, IDC_COMBO_IMAGE2, m_ctrlcomboImage2);
	DDX_Radio(pDX, IDC_ADD, m_nFunc);
}


BEGIN_MESSAGE_MAP(CArithmeticLogicalDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CArithmeticLogicalDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CArithmeticLogicalDlg 메시지 처리기입니다.
BOOL CArithmeticLogicalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	int nIndex = 0;
	CString strTitle;

	CFalconEyesApp* pApp = (CFalconEyesApp*)AfxGetApp();
	POSITION pos = pApp->m_pImageDocTemplate->GetFirstDocPosition();

	while (pos != NULL)
	{
		CFalconEyesDoc* pDoc = (CFalconEyesDoc*)pApp->m_pImageDocTemplate->GetNextDoc(pos);
		//if (pDoc->m_Dib.GetBitCount() != 8)
		//	continue;

		strTitle = pDoc->GetTitle();

		m_ctrlcomboImage1.InsertString(nIndex, strTitle);
		m_ctrlcomboImage2.InsertString(nIndex, strTitle);

		m_ctrlcomboImage1.SetItemDataPtr(nIndex, (void*)pDoc);
		m_ctrlcomboImage2.SetItemDataPtr(nIndex, (void*)pDoc);

		nIndex++;
	}

	m_ctrlcomboImage1.SetCurSel(0);
	m_ctrlcomboImage2.SetCurSel(0);

	if (nIndex > 1) m_ctrlcomboImage2.SetCurSel(1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CArithmeticLogicalDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_pDoc1 = (CFalconEyesDoc*)m_ctrlcomboImage1.GetItemDataPtr(m_ctrlcomboImage1.GetCurSel());
	m_pDoc2 = (CFalconEyesDoc*)m_ctrlcomboImage2.GetItemDataPtr(m_ctrlcomboImage2.GetCurSel());
	CDialogEx::OnOK();
}
