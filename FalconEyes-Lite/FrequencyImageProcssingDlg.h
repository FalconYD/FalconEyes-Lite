#pragma once
#include "FalconEyesDoc.h"

// FrequencyImageProcssingDlg ��ȭ �����Դϴ�.

class FrequencyImageProcssingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FrequencyImageProcssingDlg)

public:
	FrequencyImageProcssingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~FrequencyImageProcssingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DLG_FREQUENCY};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
