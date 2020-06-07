#pragma once
#include "afxwin.h"


// FeatureDlg 대화 상자입니다.

class FeatureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FeatureDlg)

public:
	FeatureDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~FeatureDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FEATURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbSource1;
	CComboBox m_cbSource2;
	CComboBox m_cbMode;
	afx_msg void OnBnClickedBnExcute();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void m_fn_UpdateDoc();
private:
	void m_fn_SURF(Mat matSrc1, Mat matSrc2, Mat& matResult);
	void m_fn_SIFT(Mat matSrc1, Mat matSrc2, Mat& matResult);
};
