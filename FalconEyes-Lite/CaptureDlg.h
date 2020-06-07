#pragma once


// CaptureDlg 대화 상자

class CaptureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CaptureDlg)

public:
	CaptureDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CaptureDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CAPTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnCapture();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double m_fn_CalcCR(Mat matInput);
	double m_fn_CalcStd(Mat matInput);
	double m_fn_CalcEdge(Mat matInput);
	Mat m_fn_GetMatImage();
	afx_msg void OnBnClickedOk();
	void m_fn_TopMost(bool bTopmost);
	afx_msg void OnBnClickedChkTopmost();
};
