#pragma once

// CGammaCorrectionDlg ��ȭ �����Դϴ�.

class CGammaCorrectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGammaCorrectionDlg)

public:
	CGammaCorrectionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGammaCorrectionDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMMA_CORRECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CSliderCtrl m_ctrlsliderGamma;
	float m_fGamma;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditGamma();
};
