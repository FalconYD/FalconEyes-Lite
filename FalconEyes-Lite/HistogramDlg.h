#pragma once


// CHistogramDlg ��ȭ �����Դϴ�.

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistogramDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void SetImage(MatDib* pDib);
	int m_Histogram[256];
	afx_msg void OnPaint();
};
