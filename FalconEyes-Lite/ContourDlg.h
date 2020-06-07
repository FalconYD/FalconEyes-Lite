#pragma once
#include "afxwin.h"


// ContourDlg 대화 상자입니다.

class ContourDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ContourDlg)

public:
	ContourDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ContourDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CONTOUR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	FalconTimer m_timer;
	afx_msg void OnBnClickedBnContour();
	void m_fn_Contour(Mat Src, Mat& Dst, std::vector<std::vector<cv::Point>>& vec, double dApprox, double dDis);
	std::vector<std::vector<cv::Point>> m_vecContours;
	int m_nObjCount;
	
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
