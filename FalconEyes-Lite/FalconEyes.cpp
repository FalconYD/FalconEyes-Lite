
// FalconEyes.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
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


// CFalconEyesApp ����

CFalconEyesApp::CFalconEyesApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("FalconEyes.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	mc_pDlgAnalystHisto = nullptr;
	mc_pDlgFrqProc = nullptr;
	mc_pDlgContour = nullptr;
	mc_pDlgThresh = nullptr;
	mc_pDlgFeature = nullptr;
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CFalconEyesApp ��ü�Դϴ�.

CFalconEyesApp theApp;


// CFalconEyesApp �ʱ�ȭ

BOOL CFalconEyesApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�.  ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	m_pImageDocTemplate = new CMultiDocTemplate(IDR_FalconEyesTYPE,
		RUNTIME_CLASS(CFalconEyesDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
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
	
	// ������Ʈ�� �ʱ�ȭ
	CleanState();

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	// ��� ���⿡ ���� ���⸦ Ȱ��ȭ�մϴ�.
	m_pMainWnd->DragAcceptFiles();

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

// 	// ���α׷� ���۽� ��â�� ����� �ʽ��ϴ�.
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
// 
// 	// DDE Execute ���⸦ Ȱ��ȭ�մϴ�.
// 	EnableShellOpen();
// 	RegisterShellFileTypes(TRUE);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFalconEyesApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CFalconEyesApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CFalconEyesApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFalconEyesApp ����� ���� �ε�/���� �޼���

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	MatDib dib;
	if (dib.PasteFromClipboard())
		AfxNewImage(dib);
}


void CFalconEyesApp::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (mc_pDlgAnalystHisto == nullptr)
	{
		mc_pDlgAnalystHisto = new AnalystHistogram(AfxGetMainWnd());
		mc_pDlgAnalystHisto->Create(IDD_ANALYST_HISTOGRAM);
	}
	mc_pDlgAnalystHisto->ShowWindow(SW_SHOW);
	
}


void CFalconEyesApp::OnSimulatorAlign()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	SHELLEXECUTEINFO m_sInfo;
	BOOL bResult = FALSE;
	PVOID pOldValue = NULL;
	Wow64DisableWow64FsRedirection(&pOldValue);

	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };

	//�ܺ� ���ø����̼� ����
	ZeroMemory(&m_sInfo, sizeof(SHELLEXECUTEINFO));
	m_sInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	m_sInfo.lpFile = _T("MFC_Align_Simulator.exe");
	m_sInfo.lpParameters = _T(""); //���� ����
								   //m_sInfo.nShow = SW_SHOWMINIMIZED; //����� �ּ�ȭ
	m_sInfo.nShow = SW_SHOW;
	m_sInfo.lpVerb = _T("runas"); //������ ���� ����
	m_sInfo.fMask = SEE_MASK_NOCLOSEPROCESS; //���μ��� �ڵ鰪 ���

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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_pImageDocTemplate->OpenDocumentFile(NULL);
}

void CFalconEyesApp::OnFilterFrequencyCv()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (mc_pDlgFrqProc == nullptr)
	{
		mc_pDlgFrqProc = new FrequencyImageProcssingDlg(AfxGetMainWnd());
		mc_pDlgFrqProc->Create(IDD_DLG_FREQUENCY);
	}
	mc_pDlgFrqProc->ShowWindow(SW_SHOW);
}


void CFalconEyesApp::OnSegmentContour()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	if (mc_pDlgContour == nullptr)
	{
		mc_pDlgContour = new ContourDlg(AfxGetMainWnd());
		mc_pDlgContour->Create(IDD_DLG_CONTOUR);
	}
	mc_pDlgContour->ShowWindow(SW_SHOW);
}


void CFalconEyesApp::OnSegmentBinDlg()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (mc_pDlgThresh == nullptr)
	{
		mc_pDlgThresh = new ThresholdDlg(AfxGetMainWnd());
		mc_pDlgThresh->Create(IDD_DLG_THRESHOLD);
	}

	mc_pDlgThresh->ShowWindow(SW_SHOW);
}

void CFalconEyesApp::OnUpdateAlignMeca(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	pCmdUI->Enable(pDoc != nullptr);
}


void CFalconEyesApp::OnImageFeature()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CFalconEyesDoc* pDoc = (CFalconEyesDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();
	pCmdUI->Enable(pDoc != nullptr);
}
