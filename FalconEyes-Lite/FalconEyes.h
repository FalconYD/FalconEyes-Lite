
// FalconEyes.h : FalconEyes ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "AnalystHistogram.h"
#include "FrequencyImageProcssingDlg.h"
#include "ContourDlg.h"
#include "ThresholdDlg.h"
#include "FeatureDlg.h"
#include "CaptureDlg.h"

//#include "SimulatorAlignDlg.h"

// CFalconEyesApp:
// �� Ŭ������ ������ ���ؼ��� FalconEyes.cpp�� �����Ͻʽÿ�.
//
class MatDib;
class CFalconEyesApp : public CWinAppEx
{
public:
	CFalconEyesApp() noexcept;


public:
	MatDib* m_pNewDib;
	AnalystHistogram* mc_pDlgAnalystHisto;
	FrequencyImageProcssingDlg* mc_pDlgFrqProc;
	ContourDlg* mc_pDlgContour;
	ThresholdDlg* mc_pDlgThresh;
	FeatureDlg* mc_pDlgFeature;
	CaptureDlg* mc_pDlgCapture;
	CMultiDocTemplate* m_pImageDocTemplate;
	CMultiDocTemplate* m_pAviDocTemplate;

	SharedMemHeader m_stSharedMem;
// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnAnalystHistogram();
	afx_msg void OnSimulatorAlign();
	afx_msg void OnFileNew();
	afx_msg void OnFilterFrequencyCv();
	afx_msg void OnSegmentContour();
	afx_msg void OnSegmentBinDlg();
	afx_msg void OnUpdateAlignMeca(CCmdUI *pCmdUI);
private:
public:
	afx_msg void OnImageFeature();
	afx_msg void OnUpdateAppSysinfo(CCmdUI *pCmdUI);
	afx_msg void OnFileCapture();
	afx_msg void OnUpdateAnalystHistogram(CCmdUI* pCmdUI);
};

extern CFalconEyesApp theApp;

void AfxNewImage(MatDib& dib);
void AfxNewImage(MatDib& dib, CString title);
void AfxPrintInfo(CString message);
void AfxPrintInfo(LPCTSTR lpszFormat, ...);