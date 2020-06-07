#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// ThresholdDlg ��ȭ �����Դϴ�.

class ThresholdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ThresholdDlg)

public:
	ThresholdDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ThresholdDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnThreshold();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditThresh();
	afx_msg void OnEnChangeEditThmax();
	afx_msg void OnCbnSelchangeCbMode();

private:
	void m_fn_EnableControl(bool bEnable);

public:
	CSliderCtrl m_sliderThresh;
	CSliderCtrl m_sliderThMax;
	CComboBox m_cbThresholdMode;
	CComboBox m_cbAdaptive;
	CComboBox m_cbThType1;
	CComboBox m_cbThType2;
	CSliderCtrl m_sliderBlockSize;

private:
	const int SLIDER_MIN = 0;
	const int SLIDER_MAX = 255;
	typedef enum EN_THRESHOLD_MODE
	{
		ETM_THRESHOLD = 0,
		ETM_ADAPTIVE = 1
	}THRESHOLD_MODE;

};
