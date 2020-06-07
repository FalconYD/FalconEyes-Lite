#pragma once


// CGaussianDlg ��ȭ �����Դϴ�.

class CGaussianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussianDlg)

public:
	CGaussianDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGaussianDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSSIAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctrlsliderGaussian;
	float m_fSigma;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditSigma();
	virtual BOOL OnInitDialog();
};
