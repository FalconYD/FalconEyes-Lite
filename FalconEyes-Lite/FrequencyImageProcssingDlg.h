#pragma once
#include "FalconEyesDoc.h"

// FrequencyImageProcssingDlg 대화 상자입니다.

class FrequencyImageProcssingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FrequencyImageProcssingDlg)

public:
	FrequencyImageProcssingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~FrequencyImageProcssingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DLG_FREQUENCY};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnDft();
	afx_msg void OnBnClickedBnIdft();
	afx_msg void OnBnClickedBnFilter();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBnCreateFilter();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CFalconEyesDoc* m_pDoc;
	FalconTimer mc_timer;
	Mat m_matSrc;
	Mat m_matDst;
	Mat m_matFourier;
	Mat m_matFilter;
	Mat m_matPlane[2];
private :
	void m_fn_EnableButton(bool bEnable);

	
};
