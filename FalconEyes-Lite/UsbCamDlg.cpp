// UsbCamDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "FalconEyes.h"
#include "MainFrm.h"
#include "FalconEyesDoc.h"
#include "FalconEyesView.h"
#include "UsbCamDlg.h"
#include "afxdialogex.h"



// UsbCamDlg 대화 상자

IMPLEMENT_DYNAMIC(UsbCamDlg, CDialogEx)

UsbCamDlg::UsbCamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_USBCAM, pParent)
{

}

UsbCamDlg::~UsbCamDlg()
{
}

void UsbCamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UsbCamDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BN_RUNGRABBER, &UsbCamDlg::OnBnClickedBnRungrabber)
	ON_BN_CLICKED(IDC_BN_GETSIZE, &UsbCamDlg::OnBnClickedBnGetsize)
	ON_BN_CLICKED(IDC_BN_CREATECANVAS, &UsbCamDlg::OnBnClickedBnCreatecanvas)
	ON_BN_CLICKED(IDC_BN_GRABSTART, &UsbCamDlg::OnBnClickedBnGrabstart)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// UsbCamDlg 메시지 처리기


void UsbCamDlg::OnBnClickedBnRungrabber()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bResult = FALSE;
	PVOID pOldValue = NULL;
	Wow64DisableWow64FsRedirection(&pOldValue);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };

	//외부 어플리케이션 실행
	ZeroMemory(&m_sInfo, sizeof(SHELLEXECUTEINFO));
	m_sInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	m_sInfo.lpFile = _T("FalconGrab.exe");
	m_sInfo.lpParameters = _T(""); //전달 인자
	//m_sInfo.nShow = SW_SHOWMINIMIZED; //실행시 최소화
	m_sInfo.nShow = SW_SHOW;
	m_sInfo.lpVerb = _T("runas"); //관리자 권한 실행
	m_sInfo.fMask = SEE_MASK_NOCLOSEPROCESS; //프로세스 핸들값 사용

	if (ShellExecuteEx(&m_sInfo))
	{
		TRACE("ShellExecuteEx Success\n");
		bResult = TRUE;
	}
	else
	{
		TRACE("ShellExecuteEx Failed\n");
		bResult = FALSE;
	}
}

void UsbCamDlg::OnBnClickedBnGetsize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void UsbCamDlg::OnBnClickedBnCreatecanvas()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MatDib dibTmp;
	Mat matTmp;
	int nCVType = 0;

	switch (m_stSharedMem.nColor)
	{
	case CVTYPE::CVGRAY:
		nCVType = CV_8UC1;
		break;
	case CVTYPE::CVRGB:
		nCVType = CV_8UC3;
		break;
	default:
		AfxMessageBox(_T("Unknown Color Type."));
		return;
	}
	matTmp = Mat::zeros(Size(m_stSharedMem.nWidth, m_stSharedMem.nHeight), nCVType);
	dibTmp.SetMatImg(matTmp);
	AfxNewImage(dibTmp);
}

void UsbCamDlg::OnBnClickedBnGrabstart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_stViewUpdate.bThreadFlag)
	{
		m_stViewUpdate.bThreadFlag = false;
		while (m_stViewUpdate.enState == THREAD_STATUS::THREAD_STAT_RUNNING)
		{
			Sleep(10);
		}
		SetDlgItemText(IDC_BN_GRABSTART, L"Grab Start");
	}
	else
	{
		m_fn_ViewUpdaterStart();
		SetDlgItemText(IDC_BN_GRABSTART, L"Grab Stop");
	}
}

void UsbCamDlg::m_fn_ViewUpdaterStart()
{
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	m_stViewUpdate.bThreadFlag = true;
	m_stViewUpdate.cObjectPointer1 = this;
	m_stViewUpdate.cObjectPointer2 = pDoc;
	m_stViewUpdate.enState = THREAD_STATUS::THREAD_STAT_NONE;
	m_stViewUpdate.nScanInterval = 33;

	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, THREAD_VIEWUPDATE, &m_stViewUpdate, 0, NULL);
	CloseHandle(hThread);
}

unsigned int UsbCamDlg::THREAD_VIEWUPDATE(void* pParam)
{
	CFalconEyesView* pView;
	ST_THREAD_PARAM* stThreadparam = (ST_THREAD_PARAM*)pParam;
	UsbCamDlg* pDlg = (UsbCamDlg*)stThreadparam->cObjectPointer1;
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)stThreadparam->cObjectPointer2;

	POSITION pos = pDoc->GetFirstViewPosition();
	pView = (CFalconEyesView*)pDoc->GetNextView(pos);
	MatDib dibTemp;
	Mat matBuffer = Mat::zeros(Size(pDlg->m_stSharedMem.nWidth, pDlg->m_stSharedMem.nHeight), pDlg->m_stSharedMem.nColor==1?CV_8UC1:CV_8UC3);
	//SYSTEMTIME systimeprev, systimecurr;
	CString strFrame;

	stThreadparam->enState = THREAD_STATUS::THREAD_STAT_ACTIVE;
	while (pDlg->m_stViewUpdate.bThreadFlag)
	{
		/*GetLocalTime(&systimeprev);*/
		stThreadparam->enState = THREAD_STATUS::THREAD_STAT_RUNNING;

		CSingleLock lock(&(pDlg->m_MemberLock));
		memcpy(matBuffer.data, pDlg->m_pSharedMem + sizeof(SharedMemHeader), pDlg->m_stSharedMem.nMemsize);
		pDoc->m_Dib.SetMatImg(matBuffer);
		pView->Invalidate();
		lock.Unlock();
	}
	stThreadparam->enState = THREAD_STATUS::THREAD_STAT_COMPLETE;
	return 0;
}

BOOL UsbCamDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void UsbCamDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_stViewUpdate.bThreadFlag = false;
	while (m_stViewUpdate.enState == THREAD_STATUS::THREAD_STAT_RUNNING)
	{
		Sleep(10);
	}
}
