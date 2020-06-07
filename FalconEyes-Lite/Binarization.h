#pragma once
#include "afxcmn.h"
#include "../IppImage/MatDibSegment.h"


// CBinarization 대화 상자입니다.

class CBinarization : public CDialogEx
{
	DECLARE_DYNAMIC(CBinarization)

public:
	CBinarization(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBinarization();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderThreshold;
	int m_nThreshold;
	MatDib m_dibSrc;
	MatDib m_dibRes;
	void SetImage(MatDib& dib);
	void MakePreviewImage();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditThreshold();
};
