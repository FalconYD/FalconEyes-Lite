
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "OutputWnd.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
// 	CMFCMenuBar       m_wndMenuBar;
// 	CMFCToolBar       m_wndToolBar;
// 	CMFCStatusBar     m_wndStatusBar;
// 	COutputWnd        m_wndOutput;

	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutputWnd        m_wndOutput;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	//afx_msg void OnViewCustomize();
	//afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnChangeribbonCategory(WPARAM wparam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	void EnableCategory(bool bEnable = false, int nMod = 0);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


