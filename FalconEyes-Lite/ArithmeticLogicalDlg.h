#pragma once


// CArithmeticLogicalDlg ��ȭ �����Դϴ�.

class CArithmeticLogicalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CArithmeticLogicalDlg)

public:
	CArithmeticLogicalDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CArithmeticLogicalDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARITHMETIC_LOGICAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
