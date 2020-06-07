#pragma once


// UsbCamDlg 대화 상자

class UsbCamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UsbCamDlg)

public:
	UsbCamDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~UsbCamDlg();

	enum CVTYPE
	{
		TYPENULL = 0,
		CVGRAY = 1,
		CVRGB = 3
	};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_USBCAM };
#endif

protected:
	mutable CCriticalSection m_MemberLock;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBnRungrabber();
	SHELLEXECUTEINFO m_sInfo;
	afx_msg void OnBnClickedBnGetsize();
	afx_msg void OnBnClickedBnCreatecanvas();
	afx_msg void OnBnClickedBnGrabstart();
	void m_fn_ViewUpdaterStart();

	SharedMemHeader m_stSharedMem;
	char* m_pSharedMem;
	
	ST_THREAD_PARAM m_stViewUpdate;
	static unsigned int _stdcall THREAD_VIEWUPDATE(void* pParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
