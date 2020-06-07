#pragma once
#include "afxcmn.h"

// CBrightnessConstrastDlg ��ȭ �����Դϴ�.

class CBrightnessConstrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessConstrastDlg)

public:
	CBrightnessConstrastDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrightnessConstrastDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctrlsliderBrightness;
	CSliderCtrl m_ctrlsliderContrast;
	int m_nBrightness;
	int m_nContrast;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditBrigtness();
	afx_msg void OnEnChangeEditContrast();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
