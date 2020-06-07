#pragma once


// CArithmeticLogicalDlg 대화 상자입니다.

class CArithmeticLogicalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArithmeticLogicalDlg)

public:
	CArithmeticLogicalDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CArithmeticLogicalDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARITHMETIC_LOGICAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	CComboBox m_ctrlcomboImage1;
	CComboBox m_ctrlcomboImage2;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
public:
	void* m_pDoc1;
	void* m_pDoc2;
	int m_nFunc;
};
