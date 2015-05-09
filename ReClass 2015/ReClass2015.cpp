#include "stdafx.h"
#include "ReClass2015.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DialogEdit.h"
#include "DialogClasses.h"
#include "Parser.h"

#include "afxwinappex.h"
#include "afxdialogex.h"

#include "SDK.h"
ReclassSDK*	ReclassSDK::m_pReclassSDK;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only CReClass2015App object
CReClass2015App theApp;

// CReClass2015App
BEGIN_MESSAGE_MAP(CReClass2015App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CReClass2015App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CReClass2015App::OnFileNew)
	ON_COMMAND(ID_FILE_IMPORT, &CReClass2015App::OnFileImport)
	ON_COMMAND(ID_BUTTON_NEWCLASS, &CReClass2015App::OnButtonNewClass)
	ON_COMMAND(ID_BUTTON_NOTES, &CReClass2015App::OnButtonNotes)
	ON_COMMAND(ID_BUTTON_PARSER, &CReClass2015App::OnButtonParser)
	ON_COMMAND(ID_BUTTON_HEADER, &CReClass2015App::OnButtonHeader)
	ON_COMMAND(ID_BUTTON_FOOTER, &CReClass2015App::OnButtonFooter)
	ON_COMMAND(ID_FILE_SAVE, &CReClass2015App::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CReClass2015App::OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, &CReClass2015App::OnFileOpen)
	ON_COMMAND(ID_BUTTON_RESET, &CReClass2015App::OnButtonReset)
	ON_COMMAND(ID_BUTTON_PAUSE, &CReClass2015App::OnButtonPause)
	ON_COMMAND(ID_BUTTON_RESUME, &CReClass2015App::OnButtonResume)
	ON_COMMAND(ID_BUTTON_KILL, &CReClass2015App::OnButtonKill)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PAUSE, &CReClass2015App::OnUpdateButtonPause)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RESUME, &CReClass2015App::OnUpdateButtonResume)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_KILL, &CReClass2015App::OnUpdateButtonKill)
	ON_COMMAND(ID_BUTTON_GENERATE, &CReClass2015App::OnButtonGenerate)
	ON_COMMAND(ID_BUTTON_CLEAN, &CReClass2015App::OnButtonClean)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLEAN, &CReClass2015App::OnUpdateButtonClean)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CReClass2015App::OnUpdateFileSave)
END_MESSAGE_MAP()

CReClass2015App::CReClass2015App()
{
	m_bHiColorIcons = TRUE;
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetAppID(_T( "ReClass.2015.0.5.0.2"));

	#ifndef NDEBUG
	CreateConsole();
	#endif

	FontWidth = 12;
	FontHeight = 12;
}

void getDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof tkp, NULL, NULL );
	CloseHandle( hToken );
}

extern bool Scintilla_RegisterClasses(void *hInstance);
extern bool Scintilla_ReleaseResources();

BOOL CReClass2015App::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	SetRegistryKey(_T("ReClass 2015"));
	EnableTaskbarInteraction(FALSE);
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	crBackground	= GetProfileInt("Colors", "crBackground", crBackground);
	crSelect		= GetProfileInt("Colors", "crSelect", crSelect);
	crHidden		= GetProfileInt("Colors", "crHidden", crHidden);
					  
	crOffset		= GetProfileInt("Colors", "crOffset", crOffset);
	crAddress		= GetProfileInt("Colors", "crAddress", crAddress);
	crType			= GetProfileInt("Colors", "crType", crType);
	crName			= GetProfileInt("Colors", "crName", crName);
	crIndex			= GetProfileInt("Colors", "crIndex", crIndex);
	crValue			= GetProfileInt("Colors", "crValue", crValue);
	crComment		= GetProfileInt("Colors", "crComment", crComment);
					  
	crVTable		= GetProfileInt("Colors", "crVTable", crVTable);
	crFunction		= GetProfileInt("Colors", "crFunction", crFunction);
	crChar			= GetProfileInt("Colors", "crChar", crChar);
	crCustom		= GetProfileInt("Colors", "crCustom", crCustom);
	crHex			= GetProfileInt("Colors", "crHex", crHex);

	gbOffset		= GetProfileInt("Display", "gbOffset", gbOffset) > 0 ? true : false;
	gbAddress		= GetProfileInt("Display", "gbAddress", gbAddress) > 0 ? true : false;
	gbText			= GetProfileInt("Display", "gbText", gbText) > 0 ? true : false;

	gbFloat			= GetProfileInt("Display", "gbFloat", gbFloat) > 0 ? true : false;
	gbInt			= GetProfileInt("Display", "gbInt", gbInt) > 0 ? true : false;
	gbString		= GetProfileInt("Display", "gbString", gbString) > 0 ? true : false;
	gbPointers		= GetProfileInt("Display", "gbPointers", gbPointers) > 0 ? true : false;

	gbClassBrowser = GetProfileInt("Display", "gbClassBrowser", gbClassBrowser) > 0 ? true : false;
	gbFilterProcesses = GetProfileInt("Display", "gbFilterProcesses", gbFilterProcesses) > 0 ? true : false;


	// make toggle
	gbTop		= false; //GetProfileInt("Display","gbTop",gbTop) > 0 ? true : false;

	CMDIFrameWnd* pFrame = new CMainFrame();
	m_pMainWnd = pFrame;
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_ReClass2015TYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ReClass2015TYPE));

	HICON icon;
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_OPEN));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CLOSED));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CLASS));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_METHOD));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_VTABLE));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_DELETE));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_ADD));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_RANDOM));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_DROPARROW));	Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_POINTER));	Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_ARRAY));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CUSTOM));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_ENUM));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_FLOAT));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_LEFT));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_RIGHT));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_MATRIX));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_INTEGER));	Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_TEXT));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_UNSIGNED));	Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_VECTOR));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CHANGE));		Icons.push_back(icon);
	icon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_CAMERA));		Icons.push_back(icon);

	//Font.CreatePointFont(80,"Terminal");
	////Font.CreateFont(16, 8, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
	//FontWidth = 16;
	//FontHeight = 16;
	//FontWidth = 8;
	//FontHeight = 8;

	FontWidth = 8;
	FontHeight = 16;
	Font.CreateFont(FontHeight, FontWidth, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
	//SmallFont.CreateFont(12, 8, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, FIXED_PITCH, "Terminal");

	//hProcess = CheckForProcess("winmine.exe");
	//UpdateMemoryMap();
	//UpdateExports();

	// Initialize the editor
	if (!Scintilla_RegisterClasses(AfxGetApp()->m_hInstance))
	{	
		AfxMessageBox("Scintilla failed to initiailze");
		return FALSE;
	}

	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	getDebugPriv();

	return TRUE;
}

int CReClass2015App::ExitInstance()
{
	if (m_hMDIMenu != NULL)	
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	AfxOleTerm(FALSE);

	// Release Scintilla
	Scintilla_ReleaseResources();

	WriteProfileInt("Colors", "crBackground", crBackground);
	WriteProfileInt("Colors", "crSelect", crSelect);
	WriteProfileInt("Colors", "crHidden", crHidden);

	WriteProfileInt("Colors", "crOffset", crOffset);
	WriteProfileInt("Colors", "crAddress", crAddress);
	WriteProfileInt("Colors", "crType", crType);
	WriteProfileInt("Colors", "crName", crName);
	WriteProfileInt("Colors", "crIndex", crIndex);
	WriteProfileInt("Colors", "crValue", crValue);
	WriteProfileInt("Colors", "crComment", crComment);

	WriteProfileInt("Colors", "crVTable", crVTable);
	WriteProfileInt("Colors", "crFunction", crFunction);
	WriteProfileInt("Colors", "crChar", crChar);
	WriteProfileInt("Colors", "crCustom", crCustom);
	WriteProfileInt("Colors", "crHex", crHex);

	WriteProfileInt("Display", "gbOffset", gbOffset);
	WriteProfileInt("Display", "gbAddress", gbAddress);
	WriteProfileInt("Display", "gbText", gbText);

	WriteProfileInt("Display", "gbFloat", gbFloat);
	WriteProfileInt("Display", "gbInt", gbInt);
	WriteProfileInt("Display", "gbString", gbString);
	WriteProfileInt("Display", "gbPointers", gbPointers);

	WriteProfileInt("Display", "gbTop", gbTop);
	WriteProfileInt("Display", "gbClassBrowser", gbClassBrowser);
	WriteProfileInt("Display", "gbFilterProcesses", gbFilterProcesses);

	return CWinAppEx::ExitInstance();
}

void CReClass2015App::OnButtonReset()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	CMDIChildWnd* wnd = pFrame->MDIGetActive();
	while(wnd)
	{
		wnd->SendMessage(WM_CLOSE,0,0);
		wnd = pFrame->MDIGetActive();
	}

	Classes.clear();
	Header = "";
	Footer = "";
	Notes = "";

	tdHex="char";
	tdInt32="__int32";
	tdInt16="__int16";
	tdInt8="__int8";
	tdDWORD="DWORD";
	tdWORD="WORD";
	tdBYTE="unsigned char";
	tdVec2="Vector2";
	tdVec3="Vector3";
	tdQuat="Vector4";
	tdMatrix="matrix3x4_t";

	CurrentFilePath = "";
}

void CReClass2015App::OnButtonPause()
{
	PauseResumeThreadList(false);
}

void CReClass2015App::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((hProcess != NULL));
}

void CReClass2015App::OnButtonResume()
{
	PauseResumeThreadList(true);
}

void CReClass2015App::OnUpdateButtonResume(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((hProcess != NULL));
}

void CReClass2015App::OnButtonKill()
{
	TerminateProcess(hProcess,0);
	hProcess = NULL;
}

void CReClass2015App::OnUpdateButtonKill(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((hProcess != NULL));
}

void CReClass2015App::CalcOffsets(CNodeClass* pClass)
{
	DWORD offset = 0;
	for  (UINT i=0; i < pClass->Nodes.size();i++)
	{
		pClass->Nodes[i]->offset = offset;
		offset += pClass->Nodes[i]->GetMemorySize();
	}
}
void CReClass2015App::CalcAllOffsets()
{
	for (UINT i=0; i<Classes.size();i++)
	{
		CalcOffsets(Classes[i]);
	}
}

void CReClass2015App::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, m_hMDIMenu, m_hMDIAccel);

	//char name[64];
	//sprintf(name,"Class%0.8X",GetTickCount());

	printf( "[+] OnFileNew()\n" );

	CNodeClass* pClass = new CNodeClass;
	theApp.Classes.push_back(pClass);
	pChild->m_wndView.m_pClass = pClass;

	//CNodeCustom* pCust = new CNodeCustom;
	//pCust->memsize = 18;
	//pCust->pParent = pClass;
	//pClass->Nodes.push_back(pCust);

	#ifdef _WIN64
	for (int i = 0; i < 64/8; i++)
	{
		CNodeHex64* pNode = new CNodeHex64;
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	#else
	for (int i = 0; i < 64/4; i++)
	{
		CNodeHex32* pNode = new CNodeHex32;
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	#endif

	CalcOffsets(pClass);
	return;

	//CNodeIcon* pIcon = new CNodeIcon;
	//pClass->Nodes.push_back(pIcon);

	CNodeVTable* pVTable = new CNodeVTable;
	pClass->Nodes.push_back(pVTable);
	for (int i=0; i < 5; i++)
	{
		CNodeFunctionPtr* pNode = new CNodeFunctionPtr;
		pNode->offset = i * pNode->GetMemorySize();
		pVTable->Nodes.push_back(pNode);
	}

	pClass->Nodes.push_back( new CNodeHex64 );
	pClass->Nodes.push_back( new CNodeHex32 );
	pClass->Nodes.push_back( new CNodeHex16 );
	pClass->Nodes.push_back( new CNodeHex8 );
	pClass->Nodes.push_back( new CNodeInt64 );
	pClass->Nodes.push_back( new CNodeInt32 );
	pClass->Nodes.push_back( new CNodeInt64 );
	pClass->Nodes.push_back( new CNodeInt16 );
	pClass->Nodes.push_back( new CNodeInt8 );
	pClass->Nodes.push_back( new CNodeDWORD );
	pClass->Nodes.push_back( new CNodeWORD );
	pClass->Nodes.push_back( new CNodeBYTE );
	pClass->Nodes.push_back( new CNodeText );
	pClass->Nodes.push_back( new CNodeUnicode );
	pClass->Nodes.push_back( new CNodeFloat );
	pClass->Nodes.push_back( new CNodeDouble );
	pClass->Nodes.push_back( new CNodeCustom );
	pClass->Nodes.push_back( new CNodeVec2 );
	pClass->Nodes.push_back( new CNodeVec3 );
	pClass->Nodes.push_back( new CNodeQuat );
	pClass->Nodes.push_back( new CNodeMatrix );
	//pClass->Nodes.push_back( new CNodeHex32 );
	pClass->Nodes.push_back( new CNodePChar ); //why is this here

	//for (int i=0; i < 2; i++)
	{
		CNodePtr* pNode1 = new CNodePtr;
		CNodePtr* pNode2 = new CNodePtr;
		pNode1->pNode = pNode2;
		pNode2->pNode = pClass;

		pClass->Nodes.push_back(pNode1);
	}
	//for (int i=0; i < 2; i++)
	{
		// I don't know what the fuck this does ???
		CNodeArray* pNode = new CNodeArray;
		#ifdef _WIN64
		CNodeHex64* pNode2 = new CNodeHex64;
		#else
		CNodeHex32* pNode2 = new CNodeHex32;
		#endif
		pNode->pNode = pNode2;
		pClass->Nodes.push_back(pNode);
	}

	//Calc Offsets...
	DWORD offset = 0;
	for  (UINT i = 0; i < pClass->Nodes.size(); i++ )
	{
		pClass->Nodes[i]->offset = offset;
		offset += pClass->Nodes[i]->GetMemorySize( );
	}

	//CNodeClassPtr* pClass2 = new CNodeClassPtr;
	//pClass2->pClass = pClass;
	//pClass->Nodes.push_back(pClass2);

	//for (int i=0; i < 10; i++)
	//{
	//	CNodeHex32* pNode = new CNodeHex32;
	//	pNode->offset = i * 4;
	//	pClass->Nodes.push_back(pNode);
	//}

}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) { }
	enum { IDD = IDD_ABOUTBOX }; // Dialog Data
protected: // DDX/DDV support
	virtual void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); } 
	DECLARE_MESSAGE_MAP() // Implementation
};
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CReClass2015App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CReClass2015App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CReClass2015App::LoadCustomState()
{
}

void CReClass2015App::SaveCustomState()
{
}

class ImportNode
{
public:
	CString Name;
	CString Comment;
	enum NodeType type;
	int length;
	int ref;
	std::vector<ImportNode> Nodes;
};

class ImportLink
{
public:
	CNodeBase* pNode;
	CString Name;
	unsigned int ArraySize; // Added by ICY why is it not loading array sizes?
};

void CReClass2015App::OnFileImport()
{
	return;

	CWaitCursor wait;

	CString sql;
	CppSQLite3Table table;
	std::vector<ImportNode> Import;
	std::vector<ImportLink> Links;

	char szFilters[]= "ReClass (*.rdc)|*.rdc|All Files (*.*)|*.*||";
	CFileDialog fileDlg (TRUE, "rdc", "",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, NULL);
	if( fileDlg.DoModal ()==IDOK )
	{
		CString pathName = fileDlg.GetPathName();
		try
		{
			CppSQLite3DB db;
			db.open(pathName);
			table =	db.getTable("SELECT name, sql FROM sqlite_master WHERE type='table' ORDER BY name;");
			DWORD total = table.numRows()-1;

			table =	db.getTable("select * from info;");table.setRow(0);
			Notes = table.getStringField("notes","<ERROR>");
			Header = table.getStringField("header","<ERROR>");
			try{Footer = table.getStringField("footer","<ERROR>");}catch (...){}

			for (UINT i=0; i < total;i++)
			{
				sql.Format("select * from class%i;",i);
				table =	db.getTable(sql);

				table.setRow(0);
				ImportNode iNode;
				iNode.Name		= table.getStringField("variable","<ERROR>");
				iNode.Comment	= table.getStringField("comment","<ERROR>");
				iNode.type		= (NodeType)table.getIntField("type",0);
				iNode.length	= table.getIntField("length",0);
				iNode.ref		= table.getIntField("ref",0);
				
				for (int c=1; c < table.numRows();c++)
				{
					table.setRow(c);
					ImportNode sNode;
					sNode.Name		= table.getStringField("variable","<ERROR>");
					sNode.Comment	= table.getStringField("comment","<ERROR>");
					sNode.type		= (NodeType)table.getIntField("type",0);
					sNode.length	= table.getIntField("length",0);
					sNode.ref		= table.getIntField("ref",0);
					iNode.Nodes.push_back(sNode);
				}
				Import.push_back(iNode);
			}

			for (UINT i=0; i < Import.size();i++)
			{
				if (Import[i].Name == "VTABLE") continue;

				CNodeClass* pClass = new CNodeClass;
				pClass->Name = Import[i].Name;
				pClass->Comment = Import[i].Comment;

				Classes.push_back(pClass);

				//CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
				//CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2011TYPE, m_hMDIMenu, m_hMDIAccel);
				//
				//pFrame->UpdateFrameTitleForDocument(pClass->Name);
				//pChild->SetTitle(pClass->Name);
				//pChild->SetWindowTextA(pClass->Name);
				//pChild->m_wndView.m_pClass = pClass;

				CNodeBase* pNode;
				for (UINT n=0; n<Import[i].Nodes.size();n++)
				{
					NodeType t = Import[i].Nodes[n].type;
					if ( t == nt_hex64 ) pNode = new CNodeHex64;
					if ( t == nt_hex32 ) pNode = new CNodeHex32;
					if ( t == nt_hex16 ) pNode = new CNodeHex16;
					if ( t == nt_hex8  ) pNode = new CNodeHex8;

					if ( t == nt_int64 ) pNode = new CNodeInt64;
					if ( t == nt_int32 ) pNode = new CNodeInt32;
					if ( t == nt_int16 ) pNode = new CNodeInt16;
					if ( t == nt_int8  ) pNode = new CNodeInt8;

					if ( t == nt_uint32   ) pNode = new CNodeDWORD;
					if ( t == nt_uint16   ) pNode = new CNodeWORD;
					if ( t == nt_uint8    ) pNode = new CNodeBYTE;
					if ( t == nt_pointer  ) pNode = new CNodePtr;
					if ( t == nt_float    ) pNode = new CNodeFloat;
					if ( t == nt_double   ) pNode = new CNodeDouble;
					if ( t == nt_function ) pNode = new CNodeFunctionPtr;
					if ( t == nt_pointer  )
					{
						int r = Import[i].Nodes[n].ref;
						if (Import[ r ].Name == "VTABLE")
						{
							pNode = new CNodeVTable;
							for (UINT v=0; v<Import[r].Nodes.size();v++)
							{
								CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
								pFun->Name = Import[r].Nodes[v].Name;
								if (pFun->Name == "void function()") pFun->Name = "";
								pFun->Comment = Import[r].Nodes[v].Comment;
								pFun->pParent = pNode;
								((CNodeVTable*)pNode)->Nodes.push_back(pFun);
							}
						}
						else
						{
							pNode = new CNodePtr;
							ImportLink link;
							link.pNode = (CNodePtr*)pNode;
							link.Name = Import[r].Name;
							Links.push_back(link);
						}
					}
					if (t == nt_text)
					{
						pNode = new CNodeText;
						((CNodeText*)pNode)->memsize = Import[i].Nodes[n].length;
					}
					if (t == nt_unicode)
					{
						pNode = new CNodeUnicode;
						((CNodeUnicode*)pNode)->memsize = Import[i].Nodes[n].length;
					}
					if (t == nt_custom)
					{
						pNode = new CNodeCustom;
						((CNodeCustom*)pNode)->memsize = Import[i].Nodes[n].length;
					}
					if (t == nt_instance)
					{
						pNode = new CNodeClassInstance;

						int r = Import[i].Nodes[n].ref;
						ImportLink link;
						link.pNode = (CNodeClassInstance*)pNode;
						link.Name = Import[r].Name;
						Links.push_back(link);
					}

					pNode->Name		= Import[i].Nodes[n].Name;
					pNode->Comment	= Import[i].Nodes[n].Comment;
					pNode->pParent	= pClass;
					pClass->Nodes.push_back(pNode);
				}
			}
			//Fix Links... some real ghetto shit here
			for (UINT i=0; i < Links.size();i++)
			{
				for (UINT c=0; c < Classes.size();c++)
				{
					if (Links[i].Name == Classes[c]->Name)
					{
						CNodePtr* pPointer = (CNodePtr*)Links[i].pNode;
						pPointer->pNode = Classes[c];
					}
				}
			}

			CalcAllOffsets();
		}
		catch (CppSQLite3Exception& e)
		{
			MessageBox(NULL, e.errorMessage() ,"Error",MB_OK);
		}

	}
}

void CReClass2015App::ClearSelection()
{
	for (UINT i=0; i < Classes.size(); i++)
	{
		Classes[i]->bSelected = false;
		for (UINT n=0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			pNode->bSelected = false;
			
			NodeType t = pNode->GetType();
			if (t == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f=0; f < pVTable->Nodes.size(); f++) pVTable->Nodes[f]->bSelected = false;
			}
			if (t == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->bSelected = false;
			}
			if (t == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->bSelected = false;
			}

		}
	}
}
void CReClass2015App::ClearHidden()
{
	for (UINT i=0; i < Classes.size(); i++)
	{
		Classes[i]->bHidden = false;
		for (UINT n=0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			pNode->bHidden = false;

			NodeType t = pNode->GetType();
			if (t == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f=0; f < pVTable->Nodes.size(); f++) pVTable->Nodes[f]->bHidden = false;
			}
			if (t == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->bHidden = false;
			}
			if (t == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->bHidden = false;
			}
		}
	}
}
bool CReClass2015App::IsNodeValid(CNodeBase* pCheckNode)
{
	for (UINT i=0; i < Classes.size(); i++)
	{
		for (UINT n=0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			if (pNode == pCheckNode) return true;

			NodeType t = pNode->GetType();
			if (t == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f=0; f < pVTable->Nodes.size(); f++)
				{
					if (pVTable->Nodes[f] == pCheckNode) 
						return true;

				}
			}
			if (t == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				if (pArray->pNode == pCheckNode) 
					return true;
			}
			if (t == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				if (pPtr->pNode == pCheckNode) 
					return true;
			}
		}
	}
	return false;
}


//////////////// OnButtonNewClass /////////////////
void CReClass2015App::OnButtonNewClass()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, m_hMDIMenu, m_hMDIAccel);

	printf( "[+] OnButtonNewClass()\n");

	//pChild->SetTitle(name);
	//pChild->SetWindowTextA(name);
	//pFrame->UpdateFrameTitleForDocument(name);

	CNodeClass* pClass = new CNodeClass;
	theApp.Classes.push_back(pClass);
	pChild->m_wndView.m_pClass = pClass;

	// TODO Fix this
#ifdef _WIN64
	for ( int i=0; i < 64 / 8; i++ )
	{
		CNodeHex64* pNode = new CNodeHex64;
		pNode->pParent = pClass;
		pClass->Nodes.push_back( pNode );
	}
#else
	for ( int i = 0; i < 64 / 4; i++ )
	{
		CNodeHex32* pNode = new CNodeHex32;
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
#endif

	CalcOffsets(pClass);
	return;
}


void CReClass2015App::OnButtonNotes()
{
	CDialogEdit dlg;
	dlg.Title = "Notes";
	dlg.Text = Notes;
	dlg.DoModal();
	Notes = dlg.Text;
}

void CReClass2015App::OnButtonParser( )
{
	//LoadPlugin( GetFilePath( "\\Plugins\\TestPlugin.dll" ) );

	CDialogClasses dlg;
	dlg.DoModal( );

	//CDialogEdit dlg;
	//dlg.Title = "Parser";
	//dlg.Text = "";
	//dlg.DoModal( );

	//Parse( dlg.Text );
	//Notes = ;
}

void CReClass2015App::OnButtonHeader()
{
	CDialogEdit dlg;
	dlg.Title = "Header";
	dlg.Text = Header;
	dlg.DoModal();
	Header = dlg.Text;
}

void CReClass2015App::OnButtonFooter()
{
	CDialogEdit dlg;
	dlg.Title = "Footer";
	dlg.Text = Footer;
	dlg.DoModal();
	Footer = dlg.Text;
}

CNodeBase* CReClass2015App::CreateNewNode(NodeType Type)
{
	if (Type == nt_class) return new CNodeClass;

	if (Type == nt_hex64) return new CNodeHex64;
	if (Type == nt_hex32) return new CNodeHex32;
	if (Type == nt_hex16) return new CNodeHex16;
	if (Type == nt_hex8) return new CNodeHex8;

	if (Type == nt_int64) return new CNodeInt64;
	if (Type == nt_int32) return new CNodeInt32;
	if (Type == nt_int16) return new CNodeInt16;
	if (Type == nt_int8) return new CNodeInt8;

	if (Type == nt_uint32) return new CNodeDWORD;
	if (Type == nt_uint16) return new CNodeWORD;
	if (Type == nt_uint8) return new CNodeBYTE;

	if (Type == nt_vec2) return new CNodeVec2;
	if (Type == nt_vec3) return new CNodeVec3;
	if (Type == nt_quat) return new CNodeQuat;
	if (Type == nt_matrix) return new CNodeMatrix;

	if (Type == nt_float) return new CNodeFloat;
	if (Type == nt_double) return new CNodeDouble;

	if (Type == nt_custom) return new CNodeCustom;
	if (Type == nt_text) return new CNodeText;
	if (Type == nt_pchar) return new CNodePChar;
	if (Type == nt_unicode) return new CNodeUnicode;

	if (Type == nt_vtable) return new CNodeVTable;
	if (Type == nt_function) return new CNodeFunctionPtr;

	if (Type == nt_pointer) return new CNodePtr;
	if (Type == nt_array) return new CNodeArray;
	if (Type == nt_instance) return new CNodeClassInstance;

	return NULL;
}

void CReClass2015App::SaveXML(char* FileName)
{
	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement("ReClass");  
	doc.LinkEndChild( root );  

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue("reclass 2014");  
	root->LinkEndChild(comment);  
	//---------------------------------------------

	TiXmlElement* settings;
	settings = new TiXmlElement("TypeDef");
	settings->SetAttribute("tdHex", tdHex);
	settings->SetAttribute("tdInt64", tdInt64);
	settings->SetAttribute("tdInt32", tdInt32);
	settings->SetAttribute("tdInt16", tdInt16);
	settings->SetAttribute("tdInt8", tdInt8);
	settings->SetAttribute("tdDWORD", tdDWORD);
	settings->SetAttribute("tdWORD", tdWORD);
	settings->SetAttribute("tdBYTE", tdBYTE);
	settings->SetAttribute("tdVec2", tdVec2);
	settings->SetAttribute("tdVec3", tdVec3);
	settings->SetAttribute("tdQuat", tdQuat);
	settings->SetAttribute("tdMatrix", tdMatrix);
	root->LinkEndChild(settings);

	settings = new TiXmlElement("Header");
	settings->SetAttribute("Text",Header);
	root->LinkEndChild(settings);
	settings = new TiXmlElement("Footer");
	settings->SetAttribute("Text",Footer);
	root->LinkEndChild(settings);
	settings = new TiXmlElement("Notes");
	settings->SetAttribute("Text",Notes);
	root->LinkEndChild(settings);

	for (UINT i = 0; i < Classes.size(); i++)
	{
		CNodeClass* pClass = Classes[i];
		if (!pClass)
			continue;

		TiXmlElement* classnode = new TiXmlElement("Class");
		classnode->SetAttribute("Name", pClass->Name);
		classnode->SetAttribute("Type", pClass->GetType());
		classnode->SetAttribute("Comment", pClass->Comment);
		classnode->SetAttribute("Offset", (int)pClass->offset);
		classnode->SetAttribute("strOffset", pClass->strOffset);
		classnode->SetAttribute("Code", pClass->Code);
		root->LinkEndChild(classnode);

		for (UINT n = 0; n < pClass->Nodes.size(); n++)
		{
			CNodeBase* pNode = pClass->Nodes[n];
			if (!pNode)
				continue;

			TiXmlElement *node = new TiXmlElement("Node");
			node->SetAttribute("Name", pNode->Name);
			node->SetAttribute("Type", pNode->GetType());
			node->SetAttribute("Size", pNode->GetMemorySize());
			node->SetAttribute("bHidden", pNode->bHidden);
			node->SetAttribute("Comment", pNode->Comment);
			classnode->LinkEndChild(node);

			if (pNode->GetType() == nt_array)
			{
				CNodeArray* pptr = (CNodeArray*)pNode;
				node->SetAttribute("Total",pptr->Total);

				TiXmlElement *item = new TiXmlElement("Array");
				item->SetAttribute("Name", pptr->pNode->Name);
				item->SetAttribute("Type", pptr->pNode->GetType());
				item->SetAttribute("Size", pptr->pNode->GetMemorySize());
				item->SetAttribute("Comment", pptr->pNode->Comment);
				node->LinkEndChild(item);
			}			
			if (pNode->GetType() == nt_pointer)
			{
				CNodePtr* pptr = (CNodePtr*)pNode;
				node->SetAttribute("Pointer", pptr->pNode->Name);
			}
			if (pNode->GetType() == nt_instance)
			{
				CNodeClassInstance* pptr = (CNodeClassInstance*)pNode;
				node->SetAttribute("Instance",pptr->pNode->Name);
			}
			if (pNode->GetType() == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f=0; f < pVTable->Nodes.size();f++)
				{
					CNodeFunctionPtr* pNodefun = (CNodeFunctionPtr*)pVTable->Nodes[f];
					TiXmlElement *fun = new TiXmlElement("Function");
					fun->SetAttribute("Name", pNodefun->Name);
					fun->SetAttribute("Comment", pNodefun->Comment);
					fun->SetAttribute("bHidden", pNodefun->bHidden);
					node->LinkEndChild(fun);
					for (UINT as = 0; as < pNodefun->Assembly.size(); as++)
					{
						TiXmlElement *pCode = new TiXmlElement("Code");
						pCode->SetAttribute("Assembly", pNodefun->Assembly[as]);
						fun->LinkEndChild(pCode);
					}
				}
			}
		}
	}

	doc.SaveFile(FileName);
}

void CReClass2015App::OnFileSave()
{
	SaveXML(CurrentFilePath.GetBuffer());
}

void CReClass2015App::OnFileSaveAs()
{
	char szFilters[]= "ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||";
	CFileDialog fileDlg (FALSE, "reclass", "",OFN_HIDEREADONLY, szFilters, NULL);
	if(fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();
	CurrentFilePath = pathName;
	SaveXML(pathName.GetBuffer());
}

void CReClass2015App::OnFileOpen()
{
	char szFilters[]= "ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||";
	CFileDialog fileDlg (TRUE, "reclass", "", OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, NULL);
	if(fileDlg.DoModal() != IDOK) 
		return;

	CString pathName = fileDlg.GetPathName();

	OnButtonReset();

	TiXmlDocument doc(pathName);
	if (!doc.LoadFile()) 
		return;

	CurrentFilePath = pathName;

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);
	TiXmlElement* pElem;
	std::vector<ImportLink> Links;

	pElem = hDoc.FirstChildElement().Element();
	if (!pElem) return;
	const char* v = pElem->Value();
	if (stricmp(v, "ReClass") != 0) 
		return; //Not a Reclass file...
	hRoot = TiXmlHandle(pElem);

	pElem = hRoot.FirstChild( "TypeDef" ).Element();
	if (pElem)
	{
		tdHex	 = pElem->Attribute("tdHex");
		tdInt64  = pElem->Attribute("tdInt64");
		tdInt32	 = pElem->Attribute("tdInt32");
		tdInt16	 = pElem->Attribute("tdInt16");
		tdInt8	 = pElem->Attribute("tdInt8");
		tdDWORD	 = pElem->Attribute("tdDWORD");
		tdWORD	 = pElem->Attribute("tdWORD");
		tdBYTE	 = pElem->Attribute("tdBYTE");
		tdVec2	 = pElem->Attribute("tdVec2");
		tdVec3	 = pElem->Attribute("tdVec3");
		tdQuat	 = pElem->Attribute("tdQuat");
		tdMatrix = pElem->Attribute("tdMatrix");
		//todo: insert pchar
	}

	pElem = hRoot.FirstChild( "Header" ).Element();
	if (pElem)
		Header = pElem->Attribute("Text");

	pElem = hRoot.FirstChild( "Footer" ).Element();
	if (pElem)
		Footer = pElem->Attribute("Text");

	pElem = hRoot.FirstChild( "Notes" ).Element();
	if (pElem)
		Notes = pElem->Attribute("Text");

	pElem = hRoot.FirstChild( "Class" ).Element();
	while(pElem)
	{
		CNodeClass* pClass	= new CNodeClass;
		pClass->Name		= pElem->Attribute("Name");
		pClass->Comment		= pElem->Attribute("Comment");
		pClass->Code		= pElem->Attribute("Code");
		pClass->offset		= atoi(pElem->Attribute("Offset"));
		pClass->strOffset	= pElem->Attribute("strOffset");

		if (pClass->strOffset == "")
			pClass->strOffset = pElem->Attribute("Offset");
		
		TiXmlElement* pClassElem;
		pClassElem = pElem->FirstChildElement( );
		
		while( pClassElem )
		{
			int Type = -1;
			pClassElem->QueryIntAttribute("Type",&Type);

			if (Type != -1)
			{
				int Size=-1;
				CNodeBase* pNode = CreateNewNode((NodeType)Type);
				pNode->Name = pClassElem->Attribute("Name");
				pNode->Comment = pClassElem->Attribute("Comment");
				pNode->bHidden = atoi(pClassElem->Attribute("bHidden")) > 0 ? true : false;
				pClassElem->QueryIntAttribute("Size",&Size);
				pNode->pParent = pClass;
				pClass->Nodes.push_back(pNode);
		
				if (Type == nt_custom)	((CNodeCustom*)pNode)->memsize = Size;
				if (Type == nt_text)	((CNodeText*)pNode)->memsize = Size;
				if (Type == nt_unicode) ((CNodeText*)pNode)->memsize = Size;
				if (Type == nt_vtable)
				{
					TiXmlElement* pVTableElem;
					pVTableElem = pClassElem->FirstChildElement();
					while(pVTableElem)
					{
						CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
						pFun->Name		= pVTableElem->Attribute("Name");
						pFun->Comment	= pVTableElem->Attribute("Comment");
						pFun->bHidden	= atoi(pVTableElem->Attribute("bHidden")) > 0 ? true : false;
						pFun->pParent = pNode;
						((CNodeVTable*)pNode)->Nodes.push_back(pFun);

						TiXmlElement* pCode = pVTableElem->FirstChildElement();
						while(pCode)
						{
							CString d;
							d = pCode->Attribute("Assembly");
							pFun->Assembly.push_back(d);
							pCode = pCode->NextSiblingElement();
						}
						pVTableElem = pVTableElem->NextSiblingElement();
					}
				}
				//<Node Name="N4823" Type="23" Size="64" bHidden="0" Comment="" Total="1">
				//<Array Name="N12DB" Type="24" Size="64" Comment="" />
				if (Type == nt_array)
				{
					CNodeArray* pArray = (CNodeArray*)pNode;
					pArray->Total = atoi(pClassElem->Attribute("Total")) > 0 ? 1 : 0;

					TiXmlElement* pArrayElem;
					pArrayElem = pClassElem->FirstChildElement();
					if(pArrayElem)
					{
						CString Name	= pArrayElem->Attribute("Name");
						CString Comment	= pArrayElem->Attribute("Comment");
						int ArrayType = -1;
						pArrayElem->QueryIntAttribute("Type",&ArrayType);
						int ArraySize = -1;
						pClassElem->QueryIntAttribute("Size",&ArraySize);

						printf( "%d %d\n", ArraySize, ArrayType );

						if (ArrayType == nt_class)
						{
							ImportLink link;
							link.Name = Name;
							link.pNode = pNode;
							// what is going on here, ghetto rhigging
							if ( ArrayType > 0 )
								link.ArraySize = ArraySize;
							else
								link.ArraySize = 1;

							Links.push_back(link);
						}
						//Handle other type of arrays....
					}
				}

				if (Type == nt_pointer)
				{
					ImportLink link;
					link.Name = pClassElem->Attribute("Pointer");
					link.pNode = pNode;
					Links.push_back(link);
				}
				if (Type == nt_instance)
				{
					ImportLink link;
					link.Name = pClassElem->Attribute("Instance");
					link.pNode = pNode;
					Links.push_back(link);
				}	
			}

			pClassElem = pClassElem->NextSiblingElement( );
		}


		Classes.push_back(pClass);
		pElem = pElem->NextSiblingElement("Class");
	}

	//printf( "bad\n" );
	//return;

	//Fix Links...
	for (UINT i=0; i < Links.size();i++)
	{
		for (UINT c=0; c < Classes.size();c++)
		{
			if (Links[i].Name == Classes[c]->Name)
			{
				NodeType Type = Links[i].pNode->GetType();
				if (Type == nt_pointer)
				{
					CNodePtr* pPointer = (CNodePtr*)Links[i].pNode;
					pPointer->pNode = Classes[c];
				}
				if (Type == nt_instance)
				{
					CNodeClassInstance* pClassInstance = (CNodeClassInstance*)Links[i].pNode;
					pClassInstance->pNode = Classes[c];
				}
				if (Type == nt_array)
				{
					CNodeArray* pArray = (CNodeArray*)Links[i].pNode;
					pArray->pNode = Classes[c];

					if ( Links[i].ArraySize > 1 )
						pArray->Total = Links[i].ArraySize / Classes[c]->GetMemorySize( );
					else
						pArray->Total = 1;

					//printf( "Array of %s\n", Classes[c]->Name );
				}
			}
		}
	}

	CalcAllOffsets();
}

void CReClass2015App::OnButtonGenerate()
{
	CDialogEdit dlg;
	dlg.Title = "Headers";

	CString h,t;

	h += "// Generated using ReClass 2014\r\n\r\n";
	h += Header + "\r\n";

	for (UINT c = 0; c < Classes.size(); c++)
	{
		t.Format("class %s;\r\n", Classes[c]->Name);
		h += t;
	}

	h += "\r\n";

	std::vector<CString> vfun;
	std::vector<CString> var;

	CString ClassName;

	for (UINT c = 0; c < Classes.size();c++)
	{
		CNodeClass* pClass = Classes[c];
		CalcOffsets(pClass);//Just in case
		vfun.clear();
		var.clear();
		ClassName.Format("class %s",pClass->Name);

		int fill = 0;
		int fillStart = 0;

		for (UINT n = 0; n < pClass->Nodes.size( ); n++)
		{
			CNodeBase* pNode = (CNodeBase*)pClass->Nodes[n];
			NodeType Type = pNode->GetType();

			// TODO: tofix
			if ( ( Type == nt_hex64 ) ||  ( Type == nt_hex32 ) || ( Type == nt_hex16 ) || ( Type == nt_hex8 ) )
			{
				if (fill == 0)
					fillStart = (int)pNode->offset;
				fill += pNode->GetMemorySize();
			}
			else
			{
				if (fill>0)
				{
					t.Format("\t%s _0x%0.4X[%i];\r\n",tdHex,fillStart,fill);
					var.push_back(t);
				}
				fill = 0;
			}
			if (Type == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f=0; f<pVTable->Nodes.size();f++)
				{
					CString fn(pVTable->Nodes[f]->Name);
					if (fn.GetLength()==0) fn.Format("void Function%i()",f);
					t.Format("\tvirtual %s; //%s\r\n", fn, pVTable->Nodes[f]->Comment);
					vfun.push_back(t);
				}
			}

			if (Type == nt_int64)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n", tdInt64, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int32)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n", tdInt32, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int16)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n", tdInt16, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int8)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdInt8,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_uint32)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdDWORD,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_uint16)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdWORD,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_uint8)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdBYTE,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_vec2)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdVec2,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_vec3)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdVec3,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_quat)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdQuat,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_matrix)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n",tdMatrix,pNode->Name,pNode->offset,pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_pchar)
			{
				t.Format("\t%s %s; //0x%0.4X %s\r\n", tdPChar, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_text)
			{
				CNodeText* pText = (CNodeText*)pNode;
				t.Format("\tchar %s[%i]; //0x%0.4X %s\r\n", pText->Name, pText->memsize, pText->offset, pText->Comment);
				var.push_back(t);
			}
			if (Type == nt_unicode)
			{
				CNodeUnicode* pText = (CNodeUnicode*)pNode;
				t.Format("\twchar_t %s[%i]; //0x%0.4X %s\r\n", pText->Name, pText->memsize / sizeof( wchar_t ), pText->offset, pText->Comment);
				var.push_back(t);
			}

			if (Type == nt_float)
			{
				t.Format("\tfloat %s; //0x%0.4X %s\r\n", pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_double)
			{
				t.Format("\tdouble %s; //0x%0.4X %s\r\n", pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_custom)
			{
				t.Format("\t%s; //0x%0.4X %s\r\n", pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_function)
			{
				t.Format("\t%s; //0x%0.4X %s\r\n", pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_pointer)
			{
				CNodePtr* pPointer = (CNodePtr*)pNode;
				t.Format("\t%s* %s; //0x%0.4X %s\r\n", pPointer->pNode->Name ,pPointer->Name, pPointer->offset, pPointer->Comment);
				var.push_back(t);
			}

			if (Type == nt_instance)
			{
				CNodeClassInstance* pCls = (CNodeClassInstance*)pNode;
				if (pCls->offset == 0)
				{
					t.Format(" : public %s", pCls->pNode->Name); // Inheritance
					ClassName += t;
				}
				else
				{
					t.Format("\t%s %s; //0x%0.4X %s\r\n", pCls->pNode->Name, pCls->Name, pCls->offset, pCls->Comment);
					var.push_back(t);
				}
			}

			if (Type == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				t.Format("\t%s %s[%i]; //0x%0.4X %s\r\n", pArray->pNode->Name, pArray->Name, pArray->Total, pArray->offset, pArray->Comment);
				var.push_back(t);
			}

		}

		if (fill > 0)
		{
			t.Format("%s _0x%0.4X[%i];\r\n", tdHex, fillStart, fill);
			var.push_back(t);
		}

		t.Format("%s\r\n{\r\npublic:\r\n", ClassName);
		h += t;

		for (UINT i = 0; i < vfun.size(); i++) 
			h += vfun[i];
		if (vfun.size() > 0) 
			h += "\r\n";

		for (UINT i = 0; i < var.size() ;i++)	
			h += var[i];
		if (var.size() > 0) 
			h += "\r\n";

		if (pClass->Code.GetLength() > 0)
		{
			h += pClass->Code;
			h += "\r\n";
		}

		t.Format("};//Size=0x%0.4X\r\n",pClass->GetMemorySize());
		h += t;
		h += "\r\n";
	}

	h += Footer + "\r\n";

	dlg.Text = h;
	dlg.DoModal();
}

void CReClass2015App::DeleteClass(CNodeClass* pClass)
{
	CNodeBase* pNode = isNodeRef(pClass);
	if (pNode)
	{
		CString msg;
		printf("Class still has a reference in %s.%s\n", pNode->pParent->Name, pNode->Name);
		msg.Format("Class still has a reference in %s.%s", pNode->pParent->Name, pNode->Name);
		MessageBox(NULL, msg, "Error", MB_OK);
		return;
	}
	for (UINT i=0; i<Classes.size();i++)
	{
		if (Classes[i] == pClass)
		{
			Classes.erase(Classes.begin()+i);
			return;
		}
	}
}

CNodeBase* CReClass2015App::isNodeRef( CNodeBase* pTestNode )
{
	for (UINT c = 0; c < Classes.size(); c++)
	{
		CNodeClass* pClass = (CNodeClass*)Classes[c];
		for (UINT n = 0; n < pClass->Nodes.size(); n++)
		{
			CNodeBase* pNode = pClass->Nodes[n];
			if (!pNode)
				continue;

			if (pNode->GetType() == nt_instance)
			{
				CNodeClassInstance* pInstance = (CNodeClassInstance*)pNode;
				if (pInstance->pNode == pTestNode) return pInstance;
			}
			if (pNode->GetType() == nt_pointer)
			{
				CNodePtr* pInstance = (CNodePtr*)pNode;
				if (pInstance->pNode == pTestNode) return pInstance;
			}
			if (pNode->GetType() == nt_array)
			{
				CNodeArray* pInstance = (CNodeArray*)pNode;
				if (pInstance->pNode == pTestNode) return pInstance;
			}
		}
	}
	return NULL;
}
void CReClass2015App::OnButtonClean()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* wnd = pFrame->MDIGetActive();
	while(wnd)
	{
		wnd->SendMessage(WM_CLOSE,0,0);
		wnd = pFrame->MDIGetActive();
	}

	std::vector<CNodeClass*> toCheck;
	for (UINT i = 0; i < Classes.size(); i++)
	{
		if (isNodeRef(Classes[i]) == NULL) 
			toCheck.push_back(Classes[i]);
	}

	int count = 0;
	for (UINT i = 0; i < toCheck.size(); i++)
	{
		CNodeClass* pClass = toCheck[i];
		bool bCanDelete = true;
		for (UINT n = 0; n < pClass->Nodes.size(); n++)
		{
			CNodeBase* pNode = pClass->Nodes[n];
			NodeType Type = pNode->GetType();

			if (Type == nt_hex64 || Type == nt_hex32 || Type == nt_hex16 || Type == nt_hex8) 
				continue;

			bCanDelete = false;
			break;
		}
		if (bCanDelete) 
		{
			count++;
			DeleteClass(pClass);
		}
	}

	CString msg;
	// TODO: fix annoying behind reclass popup
	msg.Format("Unused Classes removed: %i", count);
	MessageBox(NULL, msg, "Cleaner", MB_OK);
}
void CReClass2015App::OnUpdateButtonClean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}


void CReClass2015App::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((CurrentFilePath.GetLength() > 0));
}
