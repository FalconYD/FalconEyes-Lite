
// FalconEyes.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "FalconEyes.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FalconEyesDoc.h"
#include "FalconEyesView.h"

#include "CAviChildFrame.h"
#include "CAviDoc.h"
#include "CAviView.h"

#include "../IppImage/MatDib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFalconEyesApp

BEGIN_MESSAGE_MAP(CFalconEyesApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFalconEyesApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_EDIT_PASTE, &CFalconEyesApp::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CFalconEyesApp::OnUpdateEditPaste)
	ON_COMMAND(ID_ANALYST_HISTOGRAM, &CFalconEyesApp::OnAnalystHistogram)
	ON_COMMAND(ID_FILE_NEW, &CFalconEyesApp::OnFileNew)
	ON_COMMAND(ID_FILTER_FREQUENCY_CV, &CFalconEyesApp::OnFilterFrequencyCv)
	ON_COMMAND(ID_SEGMENT_CONTOUR, &CFalconEyesApp::OnSegmentContour)
	ON_COMMAND(ID_SEGMENT_BIN_DLG, &CFalconEyesApp::OnSegmentBinDlg)
	ON_COMMAND(ID_FILE_CAPTURE, &CFalconEyesApp::OnFileCapture)
	ON_UPDATE_COMMAND_UI(ID_ANALYST_HISTOGRAM, &CFalconEyesApp::OnUpdateAnalystHistogram)
END_MESSAGE_MAP()


// CFalconEyesApp 생성

CFalconEyesApp::CFalconEyesApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("FalconEyes.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	mc_pDlgAnalystHisto = nullptr;
	mc_pDlgFrqProc = nullptr;
	mc_pDlgContour = nullptr;
	mc_pDlgThresh = nullptr;
	mc_pDlgFeature = nullptr;
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CFalconEyesApp 개체입니다.

CFalconEyesApp theApp;


// CFalconEyesApp 초기화

BOOL CFalconEyesApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	m_pImageDocTemplate = new CMultiDocTemplate(IDR_FalconEyesTYPE,
		RUNTIME_CLASS(CFalconEyesDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CFalconEyesView));
	if (!m_pImageDocTemplate)
		return FALSE;
	AddDocTemplate(m_pImageDocTemplate);

	m_pAviDocTemplate = new CMultiDocTemplate(IDR_AVI_TYPE,
		RUNTIME_CLASS(CAviDoc),
		RUNTIME_CLASS(CAviChildFrame),
		RUNTIME_CLASS(CAviView));
	if (!m_pAviDocTemplate)
		return FALSE;
	AddDocTemplate(m_pAviDocTemplate);
	
	// 레지스트리 초기화
	CleanState();

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

// 	// 프로그램 시작시 빈창을 띄우지 않습니다.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
// 
// 	// DDE Execute 열기를 활성화합니다.
// 	EnableShellOpen();
// 	RegisterShellFileTypes(TRUE);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFalconEyesApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CFalconEyesApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CFalconEyesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFalconEyesApp 사용자 지정 로드/저장 메서드

void CFalconEyesApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CFalconEyesApp::LoadCustomState()
{
}

void CFalconEyesApp::SaveCustomState()
{
}

void AfxNewImage(MatDib& dib)
{
	theApp.m_pNewDib = &dib;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);	
}

void AfxNewImage(MatDib& dib, CString title)
{
	AfxNewImage(dib);
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	pDoc->SetTitle(title);
}

void CFalconEyesApp::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	MatDib dib;
	if (dib.PasteFromClipboard())
		AfxNewImage(dib);
}


void CFalconEyesApp::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_DIB));
}

void AfxPrintInfo(CString message)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndOutput.AddString(message);
}

void AfxPrintInfo(LPCTSTR lpszFormat, ...)
{
	CString message;
	va_list argList;
	va_start(argList, lpszFormat);
	message.FormatV(lpszFormat, argList);
	va_end(argList);
	AfxPrintInfo(message);
		
}

void CFalconEyesApp::OnAnalystHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mc_pDlgAnalystHisto == nullptr)
	{
		mc_pDlgAnalystHisto = new AnalystHistogram(AfxGetMainWnd());
		mc_pDlgAnalystHisto->Create(IDD_ANALYST_HISTOGRAM);
	}
	mc_pDlgAnalystHisto->ShowWindow(SW_SHOW);
	
}


void CFalconEyesApp::OnSimulatorAlign()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	SHELLEXECUTEINFO m_sInfo;
	BOOL bResult = FALSE;
	PVOID pOldValue = NULL;
	Wow64DisableWow64FsRedirection(&pOldValue);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };

	//외부 어플리케이션 실행
	ZeroMemory(&m_sInfo, sizeof(SHELLEXECUTEINFO));
	m_sInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	m_sInfo.lpFile = _T("MFC_Align_Simulator.exe");
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


void CFalconEyesApp::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pImageDocTemplate->OpenDocumentFile(NULL);
}

void CFalconEyesApp::OnFilterFrequencyCv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mc_pDlgFrqProc == nullptr)
	{
		mc_pDlgFrqProc = new FrequencyImageProcssingDlg(AfxGetMainWnd());
		mc_pDlgFrqProc->Create(IDD_DLG_FREQUENCY);
	}
	mc_pDlgFrqProc->ShowWindow(SW_SHOW);
}


void CFalconEyesApp::OnSegmentContour()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if (mc_pDlgContour == nullptr)
	{
		mc_pDlgContour = new ContourDlg(AfxGetMainWnd());
		mc_pDlgContour->Create(IDD_DLG_CONTOUR);
	}
	mc_pDlgContour->ShowWindow(SW_SHOW);
}


void CFalconEyesApp::OnSegmentBinDlg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mc_pDlgThresh == nullptr)
	{
		mc_pDlgThresh = new ThresholdDlg(AfxGetMainWnd());
		mc_pDlgThresh->Create(IDD_DLG_THRESHOLD);
	}

	mc_pDlgThresh->ShowWindow(SW_SHOW);
}

void CFalconEyesApp::OnUpdateAlignMeca(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	pCmdUI->Enable(pDoc != nullptr);
}


void CFalconEyesApp::OnImageFeature()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mc_pDlgFeature == nullptr)
	{
		mc_pDlgFeature = new FeatureDlg(AfxGetMainWnd());
		mc_pDlgFeature->Create(IDD_DLG_FEATURE);
	}
	mc_pDlgFeature->m_fn_UpdateDoc();
	mc_pDlgFeature->ShowWindow(SW_SHOW);
}

void CFalconEyesApp::OnFileCapture()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (mc_pDlgCapture != nullptr)
	{
		mc_pDlgCapture->DestroyWindow();
		delete mc_pDlgCapture;
	}
	mc_pDlgCapture = new CaptureDlg(AfxGetMainWnd());
	mc_pDlgCapture->Create(IDD_DLG_CAPTURE);
	//mc_pDlgOGLProfile->m_fn_UpdateDoc();
	mc_pDlgCapture->ShowWindow(SW_SHOW);
}


void CFalconEyesApp::OnUpdateAnalystHistogram(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	pCmdUI->Enable(pDoc != nullptr);
}
