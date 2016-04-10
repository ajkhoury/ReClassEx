#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ReClass2015.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DialogEdit.h"
#include "DialogClasses.h"
#include "DialogModules.h"
#include "DialogPlugins.h"
#include "DialogAbout.h"
#include "Parser.h"

//#include "SDK.h"
//ReclassSDK* ReclassSDK::m_pReclassSDK;

// The one and only CReClass2015App object
CReClass2015App theApp;

// CReClass2015App
BEGIN_MESSAGE_MAP(CReClass2015App, CWinAppEx) 
	ON_COMMAND(ID_APP_ABOUT, &CReClass2015App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CReClass2015App::OnFileNew)
	//ON_COMMAND(ID_FILE_IMPORT, &CReClass2015App::OnFileImport)
	ON_COMMAND(ID_RECLASS_PLUGINS, &CReClass2015App::OnButtonPlugins)
	ON_COMMAND(ID_BUTTON_NEWCLASS, &CReClass2015App::OnButtonNewClass)
	ON_COMMAND(ID_BUTTON_NOTES, &CReClass2015App::OnButtonNotes)
	ON_COMMAND(ID_BUTTON_SEARCH, &CReClass2015App::OnButtonSearch)
	ON_COMMAND(ID_BUTTON_CONSOLE, &CReClass2015App::OnButtonConsole) 
	ON_COMMAND(ID_BUTTON_MODULES, &CReClass2015App::OnButtonModules)
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
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SEARCH, &CReClass2015App::OnUpdateButtonSearch)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MODULES, &CReClass2015App::OnUpdateButtonModules)
	ON_COMMAND(ID_BUTTON_GENERATE, &CReClass2015App::OnButtonGenerate)
	ON_COMMAND(ID_BUTTON_CLEAN, &CReClass2015App::OnButtonClean)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLEAN, &CReClass2015App::OnUpdateButtonClean)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CReClass2015App::OnUpdateFileSave)
END_MESSAGE_MAP()

CReClass2015App::CReClass2015App()
{
	m_bHiColorIcons = TRUE;
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetAppID(_T("ReClass 2015"));

	FontWidth = 12;
	FontHeight = 12;
}

BOOL CReClass2015App::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_STANDARD_CLASSES;
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

	crBackground = GetProfileInt(_T("Colors"), _T("crBackground"), crBackground);
	crSelect = GetProfileInt(_T("Colors"), _T("crSelect"), crSelect);
	crHidden = GetProfileInt(_T("Colors"), _T("crHidden"), crHidden);
	crOffset = GetProfileInt(_T("Colors"), _T("crOffset"), crOffset);
	crAddress = GetProfileInt(_T("Colors"), _T("crAddress"), crAddress);
	crType = GetProfileInt(_T("Colors"), _T("crType"), crType);
	crName = GetProfileInt(_T("Colors"), _T("crName"), crName);
	crIndex = GetProfileInt(_T("Colors"), _T("crIndex"), crIndex);
	crValue = GetProfileInt(_T("Colors"), _T("crValue"), crValue);
	crComment = GetProfileInt(_T("Colors"), _T("crComment"), crComment);
	crVTable = GetProfileInt(_T("Colors"), _T("crVTable"), crVTable);
	crFunction = GetProfileInt(_T("Colors"), _T("crFunction"), crFunction);
	crChar = GetProfileInt(_T("Colors"), _T("crChar"), crChar);
	crCustom = GetProfileInt(_T("Colors"), _T("crCustom"), crCustom);
	crHex = GetProfileInt(_T("Colors"), _T("crHex"), crHex);
	gbOffset = GetProfileInt(_T("Display"), _T("gbOffset"), gbOffset) > 0 ? true : false;
	gbAddress = GetProfileInt(_T("Display"), _T("gbAddress"), gbAddress) > 0 ? true : false;
	gbText = GetProfileInt(_T("Display"), _T("gbText"), gbText) > 0 ? true : false;
	gbFloat = GetProfileInt(_T("Display"), _T("gbFloat"), gbFloat) > 0 ? true : false;
	gbInt = GetProfileInt(_T("Display"), _T("gbInt"), gbInt) > 0 ? true : false;
	gbString = GetProfileInt(_T("Display"), _T("gbString"), gbString) > 0 ? true : false;
	gbPointers = GetProfileInt(_T("Display"), _T("gbPointers"), gbPointers) > 0 ? true : false;
	gbClassBrowser = GetProfileInt(_T("Display"), _T("gbClassBrowser"), gbClassBrowser) > 0 ? true : false;
	gbFilterProcesses = GetProfileInt(_T("Display"), _T("gbFilterProcesses"), gbFilterProcesses) > 0 ? true : false;

	// make toggle
	gbTop = false; //GetProfileInt("Display","gbTop",gbTop) > 0 ? true : false;

	CMDIFrameWnd* pFrame = new CMainFrame();
	m_pMainWnd = pFrame;
	if (!pFrame)
		return FALSE;
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_ReClass2015TYPE));
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

	Font.CreateFont(16, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, _T("Terminal"));
	FontWidth = 8;
	FontHeight = 14;

	//FontWidth = 8;
	//FontHeight = 16;
	//Font.CreateFont(FontHeight, FontWidth, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, FIXED_PITCH, "Fixedsys");
	//FontHeight = 14;

	//SmallFont.CreateFont(12, 8, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY, FIXED_PITCH, "Terminal");

	//UpdateMemoryMap();
	//UpdateExports();

	// Initialize the editor
	if (!Scintilla_RegisterClasses(AfxGetApp()->m_hInstance))
	{
		AfxMessageBox(_T("Scintilla failed to initiailze"));
		return FALSE;
	}

	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	Utils::SetDebugPrivilege(TRUE);

	Console = new CDialogConsole( _T( "Console" ) );
	
	if (Console->Create(CDialogConsole::IDD, CWnd::GetDesktopWindow()))
		Console->ShowWindow(SW_HIDE);

	CreateDirectory( _T( "plugins" ), NULL );

	WIN32_FIND_DATA file_data;
	ZeroMemory( &file_data, sizeof( WIN32_FIND_DATA ) );
	
#ifdef _WIN64
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin64" ), &file_data );
#else
	HANDLE findfile_tree = FindFirstFile( _T( "plugins\\*.rc-plugin" ), &file_data );
#endif

	if ( findfile_tree != INVALID_HANDLE_VALUE )
	{
		do
		{
			HMODULE plugin_base = LoadLibrary( CString( _T( "plugins\\" ) ) + file_data.cFileName );

			if ( plugin_base == NULL )
			{
				CString message;
				message.Format( _T( "plugin %s was not able to be loaded!" ), file_data.cFileName );
				GetMainWnd( )->MessageBox( message );
				continue;
			}

			auto pfnPluginInit = reinterpret_cast<decltype( &PluginInit )>( GetProcAddress( plugin_base, "PluginInit" ) );
			if ( pfnPluginInit == nullptr )
			{
				CString message;
				message.Format( _T( "plugin %s is not a reclass plugin!" ), file_data.cFileName );
				GetMainWnd( )->MessageBox( message );
				FreeLibrary( plugin_base );
				continue;
			}
			
			RECLASS_PLUGIN_INFO plugin_info;
			ZeroMemory( &plugin_info, sizeof RECLASS_PLUGIN_INFO );
			if ( pfnPluginInit( &plugin_info ) )
			{
				LoadedPlugins.emplace( plugin_base, plugin_info );
				//TODO: More stuff with the plugin info E.G: Getting callbacks
			} else FreeLibrary( plugin_base );
		} while ( FindNextFile( findfile_tree, &file_data ) );
	}
	return TRUE;
}

int CReClass2015App::ExitInstance()
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	if (Console) {
		Console->EndDialog(0);
		delete Console;
	}

	AfxOleTerm(FALSE);

	// Release Scintilla
	Scintilla_ReleaseResources();

	WriteProfileInt(_T("Colors"), _T("crBackground"), crBackground);
	WriteProfileInt(_T("Colors"), _T("crSelect"), crSelect);
	WriteProfileInt(_T("Colors"), _T("crHidden"), crHidden);
	WriteProfileInt(_T("Colors"), _T("crOffset"), crOffset);
	WriteProfileInt(_T("Colors"), _T("crAddress"), crAddress);
	WriteProfileInt(_T("Colors"), _T("crType"), crType);
	WriteProfileInt(_T("Colors"), _T("crName"), crName);
	WriteProfileInt(_T("Colors"), _T("crIndex"), crIndex);
	WriteProfileInt(_T("Colors"), _T("crValue"), crValue);
	WriteProfileInt(_T("Colors"), _T("crComment"), crComment);
	WriteProfileInt(_T("Colors"), _T("crVTable"), crVTable);
	WriteProfileInt(_T("Colors"), _T("crFunction"), crFunction);
	WriteProfileInt(_T("Colors"), _T("crChar"), crChar);
	WriteProfileInt(_T("Colors"), _T("crCustom"), crCustom);
	WriteProfileInt(_T("Colors"), _T("crHex"), crHex);
	WriteProfileInt(_T("Display"), _T("gbOffset"), gbOffset);
	WriteProfileInt(_T("Display"), _T("gbAddress"), gbAddress);
	WriteProfileInt(_T("Display"), _T("gbText"), gbText);
	WriteProfileInt(_T("Display"), _T("gbFloat"), gbFloat);
	WriteProfileInt(_T("Display"), _T("gbInt"), gbInt);
	WriteProfileInt(_T("Display"), _T("gbString"), gbString);
	WriteProfileInt(_T("Display"), _T("gbPointers"), gbPointers);
	WriteProfileInt(_T("Display"), _T("gbTop"), gbTop);
	WriteProfileInt(_T("Display"), _T("gbClassBrowser"), gbClassBrowser);
	WriteProfileInt(_T("Display"), _T("gbFilterProcesses"), gbFilterProcesses);

	for ( auto plugin : LoadedPlugins )
		FreeLibrary( plugin.first );

	return CWinAppEx::ExitInstance();
}

void CReClass2015App::OnButtonReset()
{
	g_hProcess = NULL;
	g_ProcessID = 0;
	g_AttachedProcessAddress = NULL;

	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* wnd = pFrame->MDIGetActive();

	while (wnd)
	{
		wnd->SendMessage(WM_CLOSE, 0, 0);
		wnd = pFrame->MDIGetActive();
	}

	Classes.clear();
	Header = _T("");
	Footer = _T("");
	Notes = _T("");

	tdHex = _T("char");
	tdInt32 = _T("__int32");
	tdInt16 = _T("__int16");
	tdInt8 = _T("__int8");
	tdDWORD = _T("DWORD");
	tdWORD = _T("WORD");
	tdBYTE = _T("BYTE");
	tdVec2 = _T("D3DVECTOR2");
	tdVec3 = _T("D3DVECTOR3");
	tdQuat = _T("D3DXQUATERNION");
	tdMatrix = _T("D3DMATRIX");
	tdPChar = _T("PCHAR");

	CurrentFilePath = "";
}

void CReClass2015App::OnButtonPause()
{
	PauseResumeThreadList(false);
}

void CReClass2015App::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2015App::OnButtonResume()
{
	PauseResumeThreadList(true);
}

void CReClass2015App::OnUpdateButtonResume(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2015App::OnButtonKill()
{
	TerminateProcess(g_hProcess, 0);
	g_hProcess = NULL;
}

void CReClass2015App::OnUpdateButtonKill(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2015App::CalcOffsets(CNodeClass* pClass)
{
	DWORD offset = 0;
	for (UINT i = 0; i < pClass->Nodes.size(); i++)
	{
		pClass->Nodes[i]->offset = offset;
		offset += pClass->Nodes[i]->GetMemorySize();
	}
}
void CReClass2015App::CalcAllOffsets()
{
	for (UINT i = 0; i < Classes.size(); i++)
		CalcOffsets(Classes[i]);
}

void CReClass2015App::OnFileNew()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, m_hMDIMenu, m_hMDIAccel);

	CNodeClass* pClass = new CNodeClass;
	pClass->pChildWindow = pChild;
	theApp.Classes.push_back(pClass);
	pChild->m_wndView.m_pClass = pClass;

	//CNodeCustom* pCust = new CNodeCustom;
	//pCust->memsize = 18;
	//pCust->pParent = pClass;
	//pClass->Nodes.push_back(pCust);

	for (int i = 0; i < 64 / sizeof(size_t); i++)
	{
		CNodeHex* pNode = new CNodeHex;
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}

	CalcOffsets(pClass);
	return;

	//CNodeIcon* pIcon = new CNodeIcon;
	//pClass->Nodes.push_back(pIcon);

	CNodeVTable* pVTable = new CNodeVTable;
	pClass->Nodes.push_back(pVTable);
	for (int i = 0; i < 5; i++)
	{
		CNodeFunctionPtr* pNode = new CNodeFunctionPtr;
		pNode->offset = i * pNode->GetMemorySize();
		pVTable->Nodes.push_back(pNode);
	}

	pClass->Nodes.push_back(new CNodeHex64);
	pClass->Nodes.push_back(new CNodeHex32);
	pClass->Nodes.push_back(new CNodeHex16);
	pClass->Nodes.push_back(new CNodeHex8);
	pClass->Nodes.push_back(new CNodeInt64);
	pClass->Nodes.push_back(new CNodeInt32);
	pClass->Nodes.push_back(new CNodeInt64);
	pClass->Nodes.push_back(new CNodeInt16);
	pClass->Nodes.push_back(new CNodeInt8);
	pClass->Nodes.push_back(new CNodeDWORD);
	pClass->Nodes.push_back(new CNodeWORD);
	pClass->Nodes.push_back(new CNodeByte);
	pClass->Nodes.push_back(new CNodeText);
	pClass->Nodes.push_back(new CNodeUnicode);
	pClass->Nodes.push_back(new CNodeFloat);
	pClass->Nodes.push_back(new CNodeDouble);
	pClass->Nodes.push_back(new CNodeCustom);
	pClass->Nodes.push_back(new CNodeVec2);
	pClass->Nodes.push_back(new CNodeVec3);
	pClass->Nodes.push_back(new CNodeQuat);
	pClass->Nodes.push_back(new CNodeMatrix);
	pClass->Nodes.push_back(new CNodeCharPtr);

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
		// 
		CNodeArray* pNode = new CNodeArray;
		CNodeHex* pNode2 = new CNodeHex;
		pNode->pNode = pNode2;
		pClass->Nodes.push_back(pNode);
	}

	// Calc Offsets
	DWORD offset = 0;
	for (UINT i = 0; i < pClass->Nodes.size(); i++)
	{
		pClass->Nodes[i]->offset = offset;
		offset += pClass->Nodes[i]->GetMemorySize();
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
};

// TODO: Fix this
void CReClass2015App::OnFileImport()
{
	return;
	//CWaitCursor wait;
	//CString sql;
	//CppSQLite3Table table;
	//std::vector<ImportNode> Import;
	//std::vector<ImportLink> Links;
	//
	//char szFilters[] = "ReClass (*.rdc)|*.rdc|All Files (*.*)|*.*||";
	//CFileDialog fileDlg(TRUE, "rdc", "",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, NULL);
	//if( fileDlg.DoModal() == IDOK )
	//{
	//	CString pathName = fileDlg.GetPathName();
	//	try
	//	{
	//		CppSQLite3DB db;
	//		db.open(pathName);
	//		table =	db.getTable("SELECT name, sql FROM sqlite_master WHERE type='table' ORDER BY name;");
	//		DWORD total = table.numRows()-1;
	//
	//		table =	db.getTable("select * from info;");table.setRow(0);
	//		Notes = table.getStringField("notes","<ERROR>");
	//		Header = table.getStringField("header","<ERROR>");
	//		try{Footer = table.getStringField("footer","<ERROR>");}catch (...){}
	//
	//		for (UINT i=0; i < total;i++)
	//		{
	//			sql.Format("select * from class%i;",i);
	//			table =	db.getTable(sql);
	//
	//			table.setRow(0);
	//			ImportNode iNode;
	//			iNode.Name		= table.getStringField("variable","<ERROR>");
	//			iNode.Comment	= table.getStringField("comment","<ERROR>");
	//			iNode.type		= (NodeType)table.getIntField("type",0);
	//			iNode.length	= table.getIntField("length",0);
	//			iNode.ref		= table.getIntField("ref",0);
	//			
	//			for (int c=1; c < table.numRows();c++)
	//			{
	//				table.setRow(c);
	//
	//				ImportNode sNode;
	//				sNode.Name		= table.getStringField("variable","<ERROR>");
	//				sNode.Comment	= table.getStringField("comment","<ERROR>");
	//				sNode.type		= (NodeType)table.getIntField("type",0);
	//				sNode.length	= table.getIntField("length",0);
	//				sNode.ref		= table.getIntField("ref",0);
	//
	//				iNode.Nodes.push_back(sNode);
	//			}
	//			Import.push_back(iNode);
	//		}
	//
	//		for (UINT i=0; i < Import.size();i++)
	//		{
	//			if (Import[i].Name == "VTABLE") continue;
	//
	//			CNodeClass* pClass = new CNodeClass;
	//			pClass->Name = Import[i].Name;
	//			pClass->Comment = Import[i].Comment;
	//
	//			Classes.push_back(pClass);
	//
	//			//CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	//			//CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, m_hMDIMenu, m_hMDIAccel);
	//			//
	//			//pFrame->UpdateFrameTitleForDocument(pClass->Name);
	//			//pChild->SetTitle(pClass->Name);
	//			//pChild->SetWindowTextA(pClass->Name);
	//			//pChild->m_wndView.m_pClass = pClass;
	//
	//			CNodeBase* pNode;
	//			for (UINT n=0; n<Import[i].Nodes.size();n++)
	//			{
	//				NodeType t = Import[i].Nodes[n].type;
	//				if ( t == nt_hex64 ) pNode = new CNodeHex64;
	//				if ( t == nt_hex32 ) pNode = new CNodeHex32;
	//				if ( t == nt_hex16 ) pNode = new CNodeHex16;
	//				if ( t == nt_hex8  ) pNode = new CNodeHex8;
	//
	//				if ( t == nt_int64 ) pNode = new CNodeInt64;
	//				if ( t == nt_int32 ) pNode = new CNodeInt32;
	//				if ( t == nt_int16 ) pNode = new CNodeInt16;
	//				if ( t == nt_int8  ) pNode = new CNodeInt8;
	//
	//				if ( t == nt_uint32   ) pNode = new CNodeDWORD;
	//				if ( t == nt_uint16   ) pNode = new CNodeWORD;
	//				if ( t == nt_uint8    ) pNode = new CNodeByte;
	//				if ( t == nt_pointer  ) pNode = new CNodePtr;
	//				if ( t == nt_float    ) pNode = new CNodeFloat;
	//				if ( t == nt_double   ) pNode = new CNodeDouble;
	//				if ( t == nt_function ) pNode = new CNodeFunctionPtr;
	//				if ( t == nt_pointer  )
	//				{
	//					int r = Import[i].Nodes[n].ref;
	//					if (Import[ r ].Name == "VTABLE")
	//					{
	//						pNode = new CNodeVTable;
	//						for (UINT v=0; v<Import[r].Nodes.size();v++)
	//						{
	//							CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
	//							pFun->Name = Import[r].Nodes[v].Name;
	//							if (pFun->Name == "void function()") pFun->Name = "";
	//							pFun->Comment = Import[r].Nodes[v].Comment;
	//							pFun->pParent = pNode;
	//							((CNodeVTable*)pNode)->Nodes.push_back(pFun);
	//						}
	//					}
	//					else
	//					{
	//						pNode = new CNodePtr;
	//						ImportLink link;
	//						link.pNode = (CNodePtr*)pNode;
	//						link.Name = Import[r].Name;
	//						Links.push_back(link);
	//					}
	//				}
	//				if (t == nt_text)
	//				{
	//					pNode = new CNodeText;
	//					((CNodeText*)pNode)->memsize = Import[i].Nodes[n].length;
	//				}
	//				if (t == nt_unicode)
	//				{
	//					pNode = new CNodeUnicode;
	//					((CNodeUnicode*)pNode)->memsize = Import[i].Nodes[n].length;
	//				}
	//				if (t == nt_custom)
	//				{
	//					pNode = new CNodeCustom;
	//					((CNodeCustom*)pNode)->memsize = Import[i].Nodes[n].length;
	//				}
	//				if (t == nt_instance)
	//				{
	//					pNode = new CNodeClassInstance;
	//
	//					int r = Import[i].Nodes[n].ref;
	//					ImportLink link;
	//					link.pNode = (CNodeClassInstance*)pNode;
	//					link.Name = Import[r].Name;
	//					Links.push_back(link);
	//				}
	//
	//				pNode->Name		= Import[i].Nodes[n].Name;
	//				pNode->Comment	= Import[i].Nodes[n].Comment;
	//				pNode->pParent	= pClass;
	//				pClass->Nodes.push_back(pNode);
	//			}
	//		}
	//		//Fix Links... some real ghetto code here
	//		for (UINT i = 0; i < Links.size(); i++)
	//		{
	//			for (UINT c = 0; c < Classes.size(); c++)
	//			{
	//				if (Links[i].Name == Classes[c]->Name)
	//				{
	//					CNodePtr* pPointer = (CNodePtr*)Links[i].pNode;
	//					pPointer->pNode = Classes[c];
	//				}
	//			}
	//		}
	//
	//		CalcAllOffsets();
	//	}
	//	catch (CppSQLite3Exception& e)
	//	{
	//		MessageBox(NULL, e.errorMessage() ,"Error",MB_OK);
	//	}
	//
	//}
}

void CReClass2015App::ClearSelection()
{
	for (UINT i = 0; i < Classes.size(); i++)
	{
		Classes[i]->bSelected = false;
		for (UINT n = 0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			pNode->bSelected = false;

			NodeType nt = pNode->GetType();
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->Nodes.size(); f++)
					pVTable->Nodes[f]->bSelected = false;
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->bSelected = false;
			}
			if (nt == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->bSelected = false;
			}
		}
	}
}

void CReClass2015App::ClearHidden()
{
	for (UINT i = 0; i < Classes.size(); i++)
	{
		Classes[i]->bHidden = false;
		for (UINT n = 0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			pNode->bHidden = false;

			NodeType nt = pNode->GetType();
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->Nodes.size(); f++) pVTable->Nodes[f]->bHidden = false;
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				pArray->bHidden = false;
			}
			if (nt == nt_pointer)
			{
				CNodePtr* pPtr = (CNodePtr*)pNode;
				pPtr->bHidden = false;
			}
		}
	}
}

bool CReClass2015App::IsNodeValid(CNodeBase* pCheckNode)
{
	for (UINT i = 0; i < Classes.size(); i++)
	{
		for (UINT n = 0; n < Classes[i]->Nodes.size(); n++)
		{
			CNodeBase* pNode = Classes[i]->Nodes[n];
			if (pNode == pCheckNode) 
				return true;

			NodeType nt = pNode->GetType();
			if (nt == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->Nodes.size(); f++)
				{
					if (pVTable->Nodes[f] == pCheckNode)
						return true;

				}
			}
			if (nt == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				if (pArray->pNode == pCheckNode)
					return true;
			}
			if (nt == nt_pointer)
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

	CNodeClass* pClass = new CNodeClass;
	pClass->pChildWindow = pChild;
	pClass->idx = (int)theApp.Classes.size();
	theApp.Classes.push_back(pClass);
	pChild->m_wndView.m_pClass = pClass;

	for (int i = 0; i < 64 / sizeof(size_t); i++)
	{
		CNodeHex* pNode = new CNodeHex;
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}

	CalcOffsets(pClass);
}

void CReClass2015App::OnButtonSearch()
{
	GetMainWnd( )->MessageBoxW( _T( "Coming Soon!" ), _T( "ReClass2015" ) );
}

void CReClass2015App::OnUpdateButtonSearch(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2015App::OnButtonConsole()
{
	Console->ShowWindow(SW_SHOW);
	Console->SetForegroundWindow();
}

void CReClass2015App::OnButtonModules()
{
	PrintOut(_T("OnButtonModules called"));
	CDialogModules dlg;
	dlg.DoModal();
}

void CReClass2015App::OnUpdateButtonModules( CCmdUI * pCmdU )
{ 
	pCmdU->Enable( g_hProcess != NULL );
}

void CReClass2015App::OnButtonNotes()
{
	PrintOut(_T("OnButtonNotes called"));
	CDialogEdit dlg;
	dlg.Title = _T("Notes");
	dlg.Text = Notes;
	dlg.DoModal();
	Notes = dlg.Text;
}

void CReClass2015App::OnButtonParser()
{
	//LoadPlugin(GetFilePath("\\Plugins\\TestPlugin.dll" ));
	CDialogClasses dlg;
	dlg.DoModal();

	//CDialogEdit dlg;
	//dlg.Title = "Parser";
	//dlg.Text = "";
	//dlg.DoModal( );

	//Parse( dlg.Text );
	//Notes = ;
}

void CReClass2015App::OnButtonHeader()
{
	PrintOut(_T("OnButtonHeader called"));
	CDialogEdit dlg;
	dlg.Title = _T("Header");
	dlg.Text = Header;
	dlg.DoModal();
	Header = dlg.Text;
}

void CReClass2015App::OnButtonFooter()
{
	PrintOut(_T("OnButtonFooter called"));
	CDialogEdit dlg;
	dlg.Title = _T("Footer");
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

	if (Type == nt_bits) return new CNodeBits;

	if (Type == nt_int64) return new CNodeInt64;
	if (Type == nt_int32) return new CNodeInt32;
	if (Type == nt_int16) return new CNodeInt16;
	if (Type == nt_int8) return new CNodeInt8;

	if (Type == nt_uint32) return new CNodeDWORD;
	if (Type == nt_uint16) return new CNodeWORD;
	if (Type == nt_uint8) return new CNodeByte;

	if (Type == nt_vec2) return new CNodeVec2;
	if (Type == nt_vec3) return new CNodeVec3;
	if (Type == nt_quat) return new CNodeQuat;
	if (Type == nt_matrix) return new CNodeMatrix;

	if (Type == nt_float) return new CNodeFloat;
	if (Type == nt_double) return new CNodeDouble;

	if (Type == nt_custom) return new CNodeCustom;
	if (Type == nt_text) return new CNodeText;
	if (Type == nt_pchar) return new CNodeCharPtr;
	if (Type == nt_unicode) return new CNodeUnicode;

	if (Type == nt_vtable) return new CNodeVTable;
	if (Type == nt_function) return new CNodeFunctionPtr;

	if (Type == nt_pointer) return new CNodePtr;
	if (Type == nt_array) return new CNodeArray;
	if (Type == nt_instance) return new CNodeClassInstance;

	return NULL;
}

void CReClass2015App::SaveXML(TCHAR* FileName)
{
	PrintOut(_T("SaveXML(\"%s\") called"), FileName);

	TiXMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(/*"xml version = \"1.0\" encoding=\"UTF-8\""*/);
	doc.LinkEndChild(decl);

	XMLElement* root = doc.NewElement("ReClass");
	doc.LinkEndChild(root);

	XMLComment* comment = doc.NewComment("Reclass 2015");
	root->LinkEndChild(comment);
	//---------------------------------------------
	XMLElement* settings = doc.NewElement("TypeDef");
#ifdef UNICODE
	settings->SetAttribute("tdHex", CW2A(tdHex));
	settings->SetAttribute("tdInt64", CW2A(tdInt64));
	settings->SetAttribute("tdInt32", CW2A(tdInt32));
	settings->SetAttribute("tdInt16", CW2A(tdInt16));
	settings->SetAttribute("tdInt8", CW2A(tdInt8));
	settings->SetAttribute("tdDWORD", CW2A(tdDWORD));
	settings->SetAttribute("tdWORD", CW2A(tdWORD));
	settings->SetAttribute("tdBYTE", CW2A(tdBYTE));
	settings->SetAttribute("tdFloat", CW2A(tdFloat));
	settings->SetAttribute("tdDouble", CW2A(tdDouble));
	settings->SetAttribute("tdVec2", CW2A(tdVec2));
	settings->SetAttribute("tdVec3", CW2A(tdVec3));
	settings->SetAttribute("tdQuat", CW2A(tdQuat));
	settings->SetAttribute("tdMatrix", CW2A(tdMatrix));
	settings->SetAttribute("tdPChar", CW2A(tdPChar));
#else
	settings->SetAttribute("tdHex",	  tdHex);
	settings->SetAttribute("tdInt64", tdInt64);
	settings->SetAttribute("tdInt32", tdInt32);
	settings->SetAttribute("tdInt16", tdInt16);
	settings->SetAttribute("tdInt8",  tdInt8);
	settings->SetAttribute("tdDWORD", tdDWORD);
	settings->SetAttribute("tdWORD",  tdWORD);
	settings->SetAttribute("tdBYTE",  tdBYTE);
	settings->SetAttribute("tdFloat", tdFloat);
	settings->SetAttribute("tdDouble",tdDouble);
	settings->SetAttribute("tdVec2",  tdVec2);
	settings->SetAttribute("tdVec3",  tdVec3);
	settings->SetAttribute("tdQuat",  tdQuat);
	settings->SetAttribute("tdMatrix",tdMatrix);
	settings->SetAttribute("tdPChar", tdPChar);
#endif
	root->LinkEndChild(settings);

	settings = doc.NewElement("Header");
#ifdef UNICODE
	settings->SetAttribute("Text", CW2A(Header));
	root->LinkEndChild(settings);

	settings = doc.NewElement("Footer");
	settings->SetAttribute("Text", CW2A(Footer));
	root->LinkEndChild(settings);

	settings = doc.NewElement("Notes");
	settings->SetAttribute("Text", CW2A(Notes));
	root->LinkEndChild(settings);
#else
	settings->SetAttribute("Text", Header);
	root->LinkEndChild(settings);

	settings = doc.NewElement("Footer");
	settings->SetAttribute("Text", Footer);
	root->LinkEndChild(settings);

	settings = doc.NewElement("Notes");
	settings->SetAttribute("Text", Notes);
	root->LinkEndChild(settings);
#endif

	for (UINT i = 0; i < Classes.size(); i++)
	{
		CNodeClass* pClass = Classes[i];

#ifdef UNICODE
		CStringA strClassName = CW2A(pClass->Name);
		CStringA strClassComment = CW2A(pClass->Comment);
		CStringA strClassOffset = CW2A(pClass->strOffset);
		CStringA strClassCode = CW2A(pClass->Code);
#else
		CStringA strClassName = pClass->Name;
		CStringA strClassComment = pClass->Comment;
		CStringA strClassOffset = pClass->strOffset;
		CStringA strClassCode = pClass->Code;
#endif

		XMLElement* classNode = doc.NewElement("Class");
		classNode->SetAttribute("Name", strClassName);
		classNode->SetAttribute("Type", pClass->GetType());
		classNode->SetAttribute("Comment", strClassComment);
		classNode->SetAttribute("Offset", (int)pClass->offset);
		classNode->SetAttribute("strOffset", strClassOffset);
		classNode->SetAttribute("Code", strClassCode);
		root->LinkEndChild(classNode);

		for (UINT n = 0; n < pClass->Nodes.size(); n++)
		{
			CNodeBase* pNode = pClass->Nodes[n];
			if (!pNode)
				continue;

#ifdef UNICODE
			CStringA strNodeName = CW2A(pNode->Name);
			CStringA strNodeComment = CW2A(pNode->Comment);
#else
			CStringA strNodeName = pNode->Name;
			CStringA strNodeComment = pNode->Comment;
#endif

			XMLElement* node = doc.NewElement("Node");
			node->SetAttribute("Name", strNodeName);
			node->SetAttribute("Type", pNode->GetType());
			node->SetAttribute("Size", pNode->GetMemorySize());
			node->SetAttribute("bHidden", pNode->bHidden);
			node->SetAttribute("Comment", strNodeComment);

			classNode->LinkEndChild(node);

			if (pNode->GetType() == nt_array)
			{
				CNodeArray* pptr = (CNodeArray*)pNode;
				node->SetAttribute("Total", (UINT)pptr->Total);

#ifdef UNICODE
				CStringA strArrayNodeName = CW2A(pptr->pNode->Name);
				CStringA strArrayNodeComment = CW2A(pptr->pNode->Comment);
#else
				CStringA strArrayNodeName = pptr->pNode->Name;
				CStringA strArrayNodeComment = pptr->pNode->Comment;
#endif

				XMLElement *item = doc.NewElement("Array");
				item->SetAttribute("Name", strArrayNodeName);
				item->SetAttribute("Type", pptr->pNode->GetType());
				item->SetAttribute("Size", pptr->pNode->GetMemorySize());
				item->SetAttribute("Comment", strArrayNodeComment);
				node->LinkEndChild(item);
			}
			else if (pNode->GetType() == nt_pointer)
			{
				CNodePtr* pptr = (CNodePtr*)pNode;
#ifdef UNICODE
				CStringA strPtrNodeName = CW2A(pptr->pNode->Name);
#else
				CStringA strPtrNodeName = pptr->pNode->Name;
#endif

				node->SetAttribute("Pointer", strPtrNodeName);
			}
			else if (pNode->GetType() == nt_instance)
			{
				CNodeClassInstance* pptr = (CNodeClassInstance*)pNode;
#ifdef UNICODE
				CStringA strInstanceNodeName = CW2A(pptr->pNode->Name);
#else
				CStringA strInstanceNodeName = pptr->pNode->Name;
#endif
				node->SetAttribute("Instance", strInstanceNodeName);
			}
			else if (pNode->GetType() == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->Nodes.size(); f++)
				{
					CNodeFunctionPtr* pNodefun = (CNodeFunctionPtr*)pVTable->Nodes[f];
#ifdef UNICODE
					CStringA strFunctionNodeName = CW2A(pNodefun->Name);
					CStringA strFunctionNodeComment = CW2A(pNodefun->Comment);
#else
					CStringA strFunctionNodeName = pNodefun->Name;
					CStringA strFunctionNodeComment = pNodefun->Comment;
#endif

					XMLElement *fun = doc.NewElement("Function");			
					fun->SetAttribute("Name", strFunctionNodeName);
					fun->SetAttribute("Comment", strFunctionNodeComment);
					fun->SetAttribute("bHidden", pNodefun->bHidden);
					node->LinkEndChild(fun);
					for (UINT as = 0; as < pNodefun->Assembly.size(); as++)
					{
						XMLElement *pCode = doc.NewElement("Code");
						CStringA strFunctionNodeAssembler = pNodefun->Assembly[as];
						pCode->SetAttribute("Assembly", strFunctionNodeAssembler);
						fun->LinkEndChild(pCode);
					}
				}
			}
		}
	}


	char szFilename[MAX_PATH] = { 0 };
#ifdef UNICODE
	// Convert path to mbs in unicode mode
	size_t converted = 0;
	wcstombs_s(&converted, szFilename, FileName, MAX_PATH);
#else
	strcpy_s(szFilename, FileName);
#endif

	XMLError err = doc.SaveFile(szFilename);
	if (err == XML_NO_ERROR)
	{
		PrintOut(_T("ReClass files saved successfully to \"%s\""), FileName);
		return;
	}

	PrintOut(_T("Failed to save file to \"%s\". Error %d"), FileName, err);
}

void CReClass2015App::OnFileSave()
{
	SaveXML(CurrentFilePath.GetBuffer());
}

void CReClass2015App::OnFileSaveAs()
{
	TCHAR Filters[] = _T("ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("reclass"), _T(""), OFN_HIDEREADONLY, Filters, NULL);
	if (fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();
	CurrentFilePath = pathName;
	SaveXML(pathName.GetBuffer());
}

void CReClass2015App::OnFileOpen()
{
	PrintOut(_T("OnFileOpen() called"));

	TCHAR Filters[] = _T("ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("reclass"), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, Filters, NULL);
	if (fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();

	OnButtonReset();

	TiXMLDocument doc;

#ifdef UNICODE
	// Convert path to mbs in unicode mode
	char szFilename[MAX_PATH] = { 0 };
	size_t converted = 0;
	wcstombs_s(&converted, szFilename, pathName, MAX_PATH);
#else
	char* szFilename = pathName.GetBuffer();
#endif

	XMLError ret = doc.LoadFile(szFilename);
	if (ret != XML_NO_ERROR)
		return;

	CurrentFilePath = pathName;

	XMLHandle hDoc(&doc);
	XMLHandle hRoot(0);
	XMLElement* pElem;
	std::vector<ImportLink> Links;

	pElem = hDoc.FirstChildElement().ToElement();
	if (!pElem)
		return;

	const char* v = pElem->Value();
	if (_stricmp(v, "ReClass") != 0) // the root element value is 'ReClass'
		return; // Not a Reclass file

	hRoot = XMLHandle(pElem);

	pElem = hRoot.FirstChildElement("TypeDef").ToElement();
	if (pElem)
	{
		tdHex	 = CA2W(pElem->Attribute("tdHex"));
		tdInt64  = CA2W(pElem->Attribute("tdInt64"));
		tdInt32  = CA2W(pElem->Attribute("tdInt32"));
		tdInt16  = CA2W(pElem->Attribute("tdInt16"));
		tdInt8	 = CA2W(pElem->Attribute("tdInt8"));
		tdDWORD  = CA2W(pElem->Attribute("tdDWORD"));
		tdWORD	 = CA2W(pElem->Attribute("tdWORD"));
		tdBYTE	 = CA2W(pElem->Attribute("tdBYTE"));
		tdVec2	 = CA2W(pElem->Attribute("tdVec2"));
		tdVec3	 = CA2W(pElem->Attribute("tdVec3"));
		tdQuat	 = CA2W(pElem->Attribute("tdQuat"));
		tdMatrix = CA2W(pElem->Attribute("tdMatrix"));
		tdPChar  = CA2W(pElem->Attribute("tdPChar"));
	}

	pElem = hRoot.FirstChildElement("Header").ToElement();
	if (pElem)
		Header = pElem->Attribute("Text");

	pElem = hRoot.FirstChildElement("Footer").ToElement();
	if (pElem)
		Footer = pElem->Attribute("Text");

	pElem = hRoot.FirstChildElement("Notes").ToElement();
	if (pElem)
		Notes = pElem->Attribute("Text");

	pElem = hRoot.FirstChildElement("Class").ToElement();
	while (pElem)
	{
		CNodeClass* pClass = new CNodeClass;
		pClass->Name = CA2W(pElem->Attribute("Name"));
		pClass->Comment = CA2W(pElem->Attribute("Comment"));
		pClass->Code = CA2W(pElem->Attribute("Code"));
		pClass->offset = atoi(pElem->Attribute("Offset"));
		pClass->strOffset = CA2W(pElem->Attribute("strOffset"));

		if (pClass->strOffset == "")
			pClass->strOffset = CA2W(pElem->Attribute("Offset"));

		XMLElement* pClassElem = pElem->FirstChildElement();
		while (pClassElem)
		{
			int Type = nt_none;
			pClassElem->QueryIntAttribute("Type", &Type);

			if (Type != nt_none)
			{
				int Size = -1;
				CNodeBase* pNode = CreateNewNode((NodeType)Type);
				pNode->Name = CA2W(pClassElem->Attribute("Name"));
				pNode->Comment = CA2W(pClassElem->Attribute("Comment"));
				pNode->bHidden = atoi(pClassElem->Attribute("bHidden")) > 0 ? true : false;
				pClassElem->QueryIntAttribute("Size", &Size);
				pNode->pParent = pClass;
				pClass->Nodes.push_back(pNode);

				if (Type == nt_custom)
					((CNodeCustom*)pNode)->memsize = Size;
				if (Type == nt_text)
					((CNodeText*)pNode)->memsize = Size;
				if (Type == nt_unicode)
					((CNodeText*)pNode)->memsize = Size;
				if (Type == nt_vtable)
				{
					XMLElement* pVTableElem = pClassElem->FirstChildElement();
					while (pVTableElem)
					{
						CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
						pFun->Name = CA2W(pVTableElem->Attribute("Name"));
						pFun->Comment = CA2W(pVTableElem->Attribute("Comment"));
						pFun->bHidden = atoi(pVTableElem->Attribute("bHidden")) > 0 ? true : false;
						pFun->pParent = pNode;
						((CNodeVTable*)pNode)->Nodes.push_back(pFun);

						XMLElement* pCode = pVTableElem->FirstChildElement();
						while (pCode)
						{
							CStringA disassembly = pCode->Attribute("Assembly");
							pFun->Assembly.push_back(disassembly);
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
					pArray->Total = atoi(pClassElem->Attribute("Total"));

					XMLElement* pArrayElem = pClassElem->FirstChildElement();
					if (pArrayElem)
					{
						CString Name = CA2W(pArrayElem->Attribute("Name"));
						CString Comment = CA2W(pArrayElem->Attribute("Comment"));
						int ArrayType = -1;
						pArrayElem->QueryIntAttribute("Type", &ArrayType);
						int ArraySize = -1;
						pClassElem->QueryIntAttribute("Size", &ArraySize);

						if (ArrayType == nt_class)
						{
							ImportLink link;
							link.Name = Name;
							link.pNode = pNode;
							Links.push_back(link);
						}
						//Handle other type of arrays....
					}
				}

				if (Type == nt_pointer)
				{
					ImportLink link;
					link.Name = CA2W(pClassElem->Attribute("Pointer"));
					link.pNode = pNode;
					Links.push_back(link);
				}
				if (Type == nt_instance)
				{
					ImportLink link;
					link.Name = CA2W(pClassElem->Attribute("Instance"));
					link.pNode = pNode;
					Links.push_back(link);
				}
			}

			pClassElem = pClassElem->NextSiblingElement();
		}


		Classes.push_back(pClass);
		pElem = pElem->NextSiblingElement("Class");
	}


	//Fix Links... very ghetto this whole thing is just fucked
	for (UINT i = 0; i < Links.size(); i++)
	{
		for (UINT c = 0; c < Classes.size(); c++)
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
				}
			}
		}
	}

	CalcAllOffsets();
}

void CReClass2015App::OnButtonGenerate()
{
	PrintOut(_T("OnButtonGenerate() called"));

	CDialogEdit dlg;
	dlg.Title = _T("Headers");

	CString h, t;

	h += _T("// Generated using ReClass 2015\r\n\r\n");
	h += Header + _T("\r\n\r\n");

	for (UINT c = 0; c < Classes.size(); c++)
	{
		t.Format(_T("class %s;\r\n"), Classes[c]->Name);
		h += t;
	}

	h += _T("\r\n");

	std::vector<CString> vfun;
	std::vector<CString> var;

	CString ClassName;

	for (UINT c = 0; c < Classes.size(); c++)
	{
		CNodeClass* pClass = Classes[c];

		CalcOffsets(pClass);

		vfun.clear();
		var.clear();

		ClassName.Format(_T("class %s"), pClass->Name);

		int fill = 0;
		int fillStart = 0;

		for (UINT n = 0; n < pClass->Nodes.size(); n++)
		{
			CNodeBase* pNode = (CNodeBase*)pClass->Nodes[n];
			NodeType Type = pNode->GetType();

			if ((Type == nt_hex64) || (Type == nt_hex32) || (Type == nt_hex16) || (Type == nt_hex8))
			{
				if (fill == 0)
					fillStart = (int)pNode->offset;
				fill += pNode->GetMemorySize();
			}
			else
			{
				if (fill > 0)
				{
					t.Format(_T("\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n"), tdHex, fillStart, fill, fillStart);
					var.push_back(t);
				}
				fill = 0;
			}

			if (Type == nt_vtable)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNode;
				for (UINT f = 0; f < pVTable->Nodes.size(); f++)
				{
					CString fn(pVTable->Nodes[f]->Name);
					if (fn.GetLength() == 0)
						fn.Format(_T("void Function%i()"), f);
					t.Format(_T("\tvirtual %s; //%s\r\n"), fn, pVTable->Nodes[f]->Comment);
					vfun.push_back(t);
				}
			}

			if (Type == nt_int64)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdInt64, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int32)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdInt32, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int16)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdInt16, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_int8)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdInt8, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_uint32)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdDWORD, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_uint16)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdWORD, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_uint8)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdBYTE, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_vec2)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdVec2, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_vec3)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdVec3, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_quat)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdQuat, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_matrix)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdMatrix, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_pchar)
			{
				t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), tdPChar, pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_text)
			{
				CNodeText* pText = (CNodeText*)pNode;
				t.Format(_T("\tchar %s[%i]; //0x%0.4X %s\r\n"), pText->Name, pText->memsize, pText->offset, pText->Comment);
				var.push_back(t);
			}
			if (Type == nt_unicode)
			{
				CNodeUnicode* pText = (CNodeUnicode*)pNode;
				t.Format(_T("\twchar_t %s[%i]; //0x%0.4X %s\r\n"), pText->Name, pText->memsize / sizeof(wchar_t), pText->offset, pText->Comment);
				var.push_back(t);
			}

			if (Type == nt_float)
			{
				t.Format(_T("\tfloat %s; //0x%0.4X %s\r\n"), pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}
			if (Type == nt_double)
			{
				t.Format(_T("\tdouble %s; //0x%0.4X %s\r\n"), pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_custom)
			{
				t.Format(_T("\t%s; //0x%0.4X %s\r\n"), pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_function)
			{
				t.Format(_T("\t%s; //0x%0.4X %s\r\n"), pNode->Name, pNode->offset, pNode->Comment);
				var.push_back(t);
			}

			if (Type == nt_pointer)
			{
				CNodePtr* pPointer = (CNodePtr*)pNode;
				t.Format(_T("\t%s* %s; //0x%0.4X %s\r\n"), pPointer->pNode->Name, pPointer->Name, pPointer->offset, pPointer->Comment);
				var.push_back(t);
			}

			if (Type == nt_instance)
			{
				CNodeClassInstance* pCls = (CNodeClassInstance*)pNode;
				if (pCls->offset == 0)
				{
					t.Format(_T(" : public %s"), pCls->pNode->Name); // Inheritance
					ClassName += t;
				}
				else
				{
					t.Format(_T("\t%s %s; //0x%0.4X %s\r\n"), pCls->pNode->Name, pCls->Name, pCls->offset, pCls->Comment);
					var.push_back(t);
				}
			}

			if (Type == nt_array)
			{
				CNodeArray* pArray = (CNodeArray*)pNode;
				t.Format(_T("\t%s %s[%i]; //0x%0.4X %s\r\n"), pArray->pNode->Name, pArray->Name, pArray->Total, pArray->offset, pArray->Comment);
				var.push_back(t);
			}

		}

		if (fill > 0)
		{
			t.Format(_T("%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n"), tdHex, fillStart, fill, fillStart);
			var.push_back(t);
		}

		t.Format(_T("%s\r\n{\r\npublic:\r\n"), ClassName);
		h += t;

		for (UINT i = 0; i < vfun.size(); i++)
			h += vfun[i];
		if (vfun.size() > 0)
			h += _T("\r\n");

		for (UINT i = 0; i < var.size(); i++)
			h += var[i];
		if (var.size() > 0)
			h += _T("\r\n");

		if (pClass->Code.GetLength() > 0)
		{
			h += pClass->Code;
			h += _T("\r\n");
		}

		t.Format(_T("};//Size=0x%0.4X\r\n"), pClass->GetMemorySize());
		h += t;
		h += _T("\r\n");
	}

	h += Footer + _T("\r\n");

	dlg.Text = h;
	dlg.DoModal();
}

void CReClass2015App::OnButtonPlugins()
{
	//TODO: Work out some sort of plugin system
	GetMainWnd( )->MessageBox( _T( "Comming Soon!" ) );
	//CDialogPlugins plugin_dlg;
	//plugin_dlg.DoModal( );
}

void CReClass2015App::DeleteClass(CNodeClass* pClass)
{
	PrintOut(_T("DeleteClass(\"%s\") called"), pClass->Name.GetString());

	CNodeBase* pNode = isNodeRef(pClass);
	if (pNode)
	{
		PrintOut(_T("Class still has a reference in %s.%s"), pNode->pParent->Name.GetString(), pNode->Name.GetString());
		CString msg;
		msg.Format(_T("Class still has a reference in %s.%s"), pNode->pParent->Name.GetString(), pNode->Name.GetString());
		MessageBox(GetMainWnd()->GetSafeHwnd(), msg, _T("Error"), MB_OK);
		return;
	}
	for (UINT i = 0; i < Classes.size(); i++)
	{
		if (Classes[i] == pClass)
		{
			Classes.erase(Classes.begin() + i);
			return;
		}
	}
}

CNodeBase* CReClass2015App::isNodeRef(CNodeBase* pTestNode)
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
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
			if (pNode->GetType() == nt_pointer)
			{
				CNodePtr* pInstance = (CNodePtr*)pNode;
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
			if (pNode->GetType() == nt_array)
			{
				CNodeArray* pInstance = (CNodeArray*)pNode;
				if (pInstance->pNode == pTestNode)
					return pInstance;
			}
		}
	}
	return NULL;
}

void CReClass2015App::OnButtonClean()
{
	CMDIFrameWnd* pFrame = STATIC_DOWNCAST(CMDIFrameWnd, AfxGetApp()->m_pMainWnd);
	CMDIChildWnd* wnd = pFrame->MDIGetActive();
	while (wnd)
	{
		wnd->SendMessage(WM_CLOSE, 0, 0);
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

	PrintOut(_T("Unused Classes removed: %i"), count);
	CString msg; msg.Format(_T("Unused Classes removed: %i"), count);
	MessageBox(this->GetMainWnd()->GetSafeHwnd(), msg, _T("Cleaner"), MB_OK);
}
void CReClass2015App::OnUpdateButtonClean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}

void CReClass2015App::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((CurrentFilePath.GetLength() > 0));
}
