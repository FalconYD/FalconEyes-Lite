#pragma once

// AnalystHistogram 대화 상자

class AnalystHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(AnalystHistogram)

public:
	AnalystHistogram(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AnalystHistogram();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANALYST_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	enum enColor
	{
		GRAY_INDEX = 0,
		RED_INDEX = 1,
		GREEN_INDEX = 2,
		BLUE_INDEX = 3,
		TREND_SIZE = 4,
	};
	afx_msg void OnStnDblclickGraph();
	COScopeCtrl* m_pCtrlScope;
	virtual BOOL OnInitDialog();
	Rect m_rectBack;
	Mat m_matBack;
	Point m_pntHisto1, m_pntHisto2;
	BYTE* m_pGrayData, *m_pRedData, *m_pGreenData, *m_pBlueData;
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
