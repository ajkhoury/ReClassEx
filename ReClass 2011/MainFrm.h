// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CProcessMenuInfo
{
public:
	DWORD ProcessId;
	CBitmap* pBitmap;
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();

public:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	std::vector<CProcessMenuInfo> ProcMenuItems;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	//afx_msg void OnButtonShowclasses();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnButtonCbackground();
	afx_msg void OnButtonCselect();
	afx_msg void OnButtonChidden();
	afx_msg void OnButtonCoffset();
	afx_msg void OnButtonCaddress();
	afx_msg void OnButtonChex();
	afx_msg void OnButtonCtype();
	afx_msg void OnButtonCname();
	afx_msg void OnButtonCvalue();
	afx_msg void OnButtonCindex();
	afx_msg void OnButtonCcomment();
	afx_msg void OnButtonCtext();
	afx_msg void OnButtonCvtable();
	afx_msg void OnButtonCfunction();
	afx_msg void OnButtonCcustom();
	afx_msg void OnCheckCbaddress();
	afx_msg void OnUpdateCheckCbaddress(CCmdUI *pCmdUI);
	afx_msg void OnCheckCboffset();
	afx_msg void OnUpdateCheckCboffset(CCmdUI *pCmdUI);
	afx_msg void OnButtonTypedef();
	afx_msg void OnCheckCbtext();
	afx_msg void OnUpdateCheckCbtext(CCmdUI *pCmdUI);
	//afx_msg void OnButtonSelect();
	afx_msg void OnButtonSelectprocess();
	void ClearProcMenuItems();
	afx_msg void OnButtonEditclass();
	afx_msg void OnUpdateButtonEditclass(CCmdUI *pCmdUI);
	afx_msg void OnButtonDeleteclass();
	afx_msg void OnUpdateButtonDeleteclass(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCheckTopmost();
	afx_msg void OnUpdateCheckTopmost(CCmdUI *pCmdUI);
	afx_msg void OnCheckClassBrowser( );
	afx_msg void OnUpdateCheckClassBrowser(CCmdUI *pCmdUI);
	afx_msg void OnButtonLeft();
	afx_msg void OnButtonRight();
	afx_msg void OnCheckFloat();
	afx_msg void OnUpdateCheckFloat(CCmdUI *pCmdUI);
	afx_msg void OnCheckInteger();
	afx_msg void OnUpdateCheckInteger(CCmdUI *pCmdUI);
	afx_msg void OnCheckString();
	afx_msg void OnUpdateCheckString(CCmdUI *pCmdUI);
	afx_msg void OnCheckPointer();
	afx_msg void OnUpdateCheckPointer(CCmdUI *pCmdUI);
};


