#pragma once


// CCannyDlg ��ȭ �����Դϴ�.

class CCannyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyDlg)

public:
	CCannyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCannyDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANNY_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_fSigma;
	float m_fLowTh;
	float m_fHighTh;
};
