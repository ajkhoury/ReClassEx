#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

class CReClass2015App : public CWinAppEx
{
public:
	CReClass2015App();

//App Globals
	CString Header;
	CString Footer;
	CString Notes;
	CString ParserInput;
	std::vector<CNodeClass*> Classes;

	CString CurrentFilePath;

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CNodeBase* CreateNewNode(NodeType Type);
	bool IsNodeValid(CNodeBase* pCheckNode);
	CNodeBase* isNodeRef(CNodeBase* pTestNode);
	void DeleteClass(CNodeClass* pClass);

	void CalcOffsets(CNodeClass* pClass);
	void CalcAllOffsets();
	void ClearSelection();
	void ClearHidden();

	void SaveXML(char* FileName);

// Implementation
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;

	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonReset();
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileImport();
	afx_msg void OnButtonNewClass();
	afx_msg void OnButtonNotes();
	afx_msg void OnButtonParser();
	afx_msg void OnButtonHeader();
	afx_msg void OnButtonFooter();

	afx_msg void OnButtonPause();
	afx_msg void OnUpdateButtonPause(CCmdUI *pCmdUI);
	afx_msg void OnButtonResume();
	afx_msg void OnUpdateButtonResume(CCmdUI *pCmdUI);
	afx_msg void OnButtonKill();
	afx_msg void OnUpdateButtonKill(CCmdUI *pCmdUI);
	afx_msg void OnButtonGenerate();
	afx_msg void OnButtonClean();
	afx_msg void OnUpdateButtonClean(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
};

extern CReClass2015App theApp;
