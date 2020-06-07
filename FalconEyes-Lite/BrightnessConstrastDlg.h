#pragma once
#include "afxcmn.h"

// CBrightnessConstrastDlg 대화 상자입니다.

class CBrightnessConstrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessConstrastDlg)

public:
	CBrightnessConstrastDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBrightnessConstrastDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
