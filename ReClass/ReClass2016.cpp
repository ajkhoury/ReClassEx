#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ReClass2016.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DialogEdit.h"
#include "DialogClasses.h"
#include "DialogModules.h"
#include "DialogPlugins.h"
#include "DialogAbout.h"
#include "Parser.h"

// The one and only CReClass2016App object
CReClass2016App theApp;

// CReClass2016App
BEGIN_MESSAGE_MAP(CReClass2016App, CWinAppEx) 
	ON_COMMAND(ID_APP_ABOUT, &CReClass2016App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CReClass2016App::OnFileNew)
	ON_COMMAND(ID_RECLASS_PLUGINS, &CReClass2016App::OnButtonPlugins)
	ON_COMMAND(ID_BUTTON_NEWCLASS, &CReClass2016App::OnButtonNewClass)
	ON_COMMAND(ID_BUTTON_NOTES, &CReClass2016App::OnButtonNotes)
	ON_COMMAND(ID_BUTTON_SEARCH, &CReClass2016App::OnButtonSearch)
	ON_COMMAND(ID_BUTTON_CONSOLE, &CReClass2016App::OnButtonConsole) 
	ON_COMMAND(ID_BUTTON_MODULES, &CReClass2016App::OnButtonModules)
	ON_COMMAND(ID_BUTTON_PARSER, &CReClass2016App::OnButtonParser)
	ON_COMMAND(ID_BUTTON_HEADER, &CReClass2016App::OnButtonHeader)
	ON_COMMAND(ID_BUTTON_FOOTER, &CReClass2016App::OnButtonFooter)
	ON_COMMAND(ID_FILE_SAVE, &CReClass2016App::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CReClass2016App::OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, &CReClass2016App::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_PDB, &CReClass2016App::OnOpenPDB)
	ON_COMMAND(ID_BUTTON_RESET, &CReClass2016App::OnButtonReset)
	ON_COMMAND(ID_BUTTON_PAUSE, &CReClass2016App::OnButtonPause)
	ON_COMMAND(ID_BUTTON_RESUME, &CReClass2016App::OnButtonResume)
	ON_COMMAND(ID_BUTTON_KILL, &CReClass2016App::OnButtonKill)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PAUSE, &CReClass2016App::OnUpdateButtonPause)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RESUME, &CReClass2016App::OnUpdateButtonResume)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_KILL, &CReClass2016App::OnUpdateButtonKill)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SEARCH, &CReClass2016App::OnUpdateButtonSearch)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MODULES, &CReClass2016App::OnUpdateButtonModules)
	ON_UPDATE_COMMAND_UI(ID_RECLASS_PLUGINS, &CReClass2016App::OnUpdateButtonPlugins)
	ON_COMMAND(ID_BUTTON_GENERATE, &CReClass2016App::OnButtonGenerate)
	ON_COMMAND(ID_BUTTON_CLEAN, &CReClass2016App::OnButtonClean)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLEAN, &CReClass2016App::OnUpdateButtonClean)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CReClass2016App::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_PDB, &CReClass2016App::OnUpdateOpenPDB)
END_MESSAGE_MAP()

CReClass2016App::CReClass2016App()
{
	m_bHiColorIcons = TRUE;
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetAppID(_T("ReClass 2016"));

	g_FontWidth = FONT_DEFAULT_WIDTH;
	g_FontHeight = FONT_DEFAULT_HEIGHT;
}

void CReClass2016App::ResizeMemoryFont(int font_width, int font_height)
{
	g_ViewFont.DeleteObject();

	HMODULE shcore_load_address = LoadLibrary(_T("shcore.dll"));
	auto pfnGetProcessDpiAwareness = reinterpret_cast<decltype(&GetProcessDpiAwareness)>(GetProcAddress(shcore_load_address, "GetProcessDpiAwareness"));
	auto pfnGetDpiForMonitor = reinterpret_cast<decltype(&GetDpiForMonitor)>(GetProcAddress(shcore_load_address, "GetDpiForMonitor"));
	
	if (pfnGetProcessDpiAwareness != nullptr && pfnGetDpiForMonitor != nullptr)
	{
		PROCESS_DPI_AWARENESS dpi;
		pfnGetProcessDpiAwareness(NULL, &dpi);
		if (dpi == PROCESS_DPI_AWARENESS::PROCESS_PER_MONITOR_DPI_AWARE || dpi == PROCESS_DPI_AWARENESS::PROCESS_SYSTEM_DPI_AWARE)
		{
			UINT dpiX, dpiY;
			HMONITOR monitor = ::MonitorFromWindow(m_pMainWnd->GetSafeHwnd( ), MONITOR_DEFAULTTONEAREST);
			pfnGetDpiForMonitor(monitor, MONITOR_DPI_TYPE::MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
			g_FontWidth = MulDiv(font_width, MulDiv(dpiX, 100, 96), 100);
			g_FontHeight = MulDiv(font_height, MulDiv(dpiY, 100, 96), 100);
		}
	}
	g_ViewFont.CreateFont(g_FontHeight, g_FontWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, FIXED_PITCH, _T("Terminal"));
}

BOOL CReClass2016App::InitInstance()
{
#ifdef _DEBUG
	Utils::CreateDbgConsole(_T("dbg"));
#endif

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	ntdll::Base = (PVOID)Utils::GetLocalModuleHandle("ntdll.dll");
	ntdll::NtQuerySystemInformation = reinterpret_cast<tNtQuerySystemInformation>(Utils::GetLocalProcAddress((HMODULE)ntdll::Base, "NtQuerySystemInformation"));
	ntdll::NtQueryInformationProcess = reinterpret_cast<tNtQueryInformationProcess>(Utils::GetLocalProcAddress((HMODULE)ntdll::Base, "NtQueryInformationProcess"));
	ntdll::RtlGetVersion = reinterpret_cast<tRtlGetVersion>(Utils::GetLocalProcAddress((HMODULE)ntdll::Base, "RtlGetVersion"));
	ntdll::RtlGetNativeSystemInformation = reinterpret_cast<tRtlGetNativeSystemInformation>(Utils::GetLocalProcAddress((HMODULE)ntdll::Base, "RtlGetNativeSystemInformation"));
	ntdll::NtCreateThreadEx = reinterpret_cast<tNtCreateThreadEx>(Utils::GetLocalProcAddress((HMODULE)ntdll::Base, "NtCreateThreadEx"));

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	SetRegistryKey(_T("ReClass 2016"));
	EnableTaskbarInteraction(FALSE);
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//Typedefs
	tdHex = GetProfileString( _T( "Typedefs" ), _T( "tdHex" ), _T( "char" ) );
	tdInt64 = GetProfileString( _T( "Typedefs" ), _T( "tdInt64" ), _T( "__int64" ) );
	tdInt32 = GetProfileString( _T( "Typedefs" ), _T( "tdInt32" ), _T( "__int32" ) );
	tdInt16 = GetProfileString( _T( "Typedefs" ), _T( "tdInt16" ), _T( "__int16" ) );
	tdInt8 = GetProfileString( _T( "Typedefs" ), _T( "tdInt8" ), _T( "__int8" ) );
	tdQWORD = GetProfileString( _T( "Typedefs" ), _T( "tdQWORD" ), _T( "DWORD64" ) );
	tdDWORD = GetProfileString( _T( "Typedefs" ), _T( "tdDWORD" ), _T( "DWORD" ) );
	tdWORD = GetProfileString( _T( "Typedefs" ), _T( "tdWORD" ), _T( "WORD" ) );
	tdBYTE = GetProfileString( _T( "Typedefs" ), _T( "tdBYTE" ), _T( "unsigned char" ) );
	tdFloat = GetProfileString( _T( "Typedefs" ), _T( "tdFloat" ), _T( "float" ) );
	tdDouble = GetProfileString( _T( "Typedefs" ), _T( "tdDouble" ), _T( "double" ) );
	tdVec2 = GetProfileString( _T( "Typedefs" ), _T( "tdVec2" ), _T( "Vector2" ) );
	tdVec3 = GetProfileString( _T( "Typedefs" ), _T( "tdVec3" ), _T( "Vector3" ) );
	tdQuat = GetProfileString( _T( "Typedefs" ), _T( "tdQuat" ), _T( "Vector4" ) );
	tdMatrix = GetProfileString( _T( "Typedefs" ), _T( "tdMatrix" ), _T( "matrix3x4_t" ) );
	tdPChar = GetProfileString( _T( "Typedefs" ), _T( "tdPChar" ), _T( "char *" ) );
	tdPWChar = GetProfileString( _T( "Typedefs" ), _T( "tdPWChar" ), _T( "wchar_t *" ) );

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
	gbPrivatePadding = GetProfileInt(_T("Display"), _T("gbPrivatePadding"), gbPrivatePadding) > 0 ? true : false;
	gbClipboardCopy = GetProfileInt(_T("Display"), _T("gbClipboardCopy"), gbClipboardCopy) > 0 ? true : false;

	// make toggle
	gbTop = false; //GetProfileInt("Display","gbTop",gbTop) > 0 ? true : false;

	CMDIFrameWnd* pFrame = new CMainFrame();
	m_pMainWnd = pFrame;
	
	if (!pFrame)
		return FALSE;
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_ReClass2016TYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ReClass2016TYPE));

#define PushIcon(id) Icons.push_back(::LoadIcon(hInst, MAKEINTRESOURCE(id)));
	
	PushIcon(IDI_ICON_OPEN);
	PushIcon(IDI_ICON_CLOSED);
	PushIcon(IDI_ICON_CLASS);
	PushIcon(IDI_ICON_METHOD);
	PushIcon(IDI_ICON_VTABLE);
	PushIcon(IDI_ICON_DELETE);
	PushIcon(IDI_ICON_ADD);
	PushIcon(IDI_ICON_RANDOM);
	PushIcon(IDI_ICON_DROPARROW);
	PushIcon(IDI_ICON_POINTER);
	PushIcon(IDI_ICON_ARRAY);
	PushIcon(IDI_ICON_CUSTOM);
	PushIcon(IDI_ICON_ENUM);
	PushIcon(IDI_ICON_FLOAT);
	PushIcon(IDI_ICON_LEFT);
	PushIcon(IDI_ICON_RIGHT);
	PushIcon(IDI_ICON_MATRIX);
	PushIcon(IDI_ICON_INTEGER);
	PushIcon(IDI_ICON_TEXT);
	PushIcon(IDI_ICON_UNSIGNED);
	PushIcon(IDI_ICON_VECTOR);
	PushIcon(IDI_ICON_CHANGE);
	PushIcon(IDI_ICON_CAMERA);

#undef PushIcon

	ResizeMemoryFont(g_FontWidth, g_FontHeight);

	g_hProcess = NULL;
	g_ProcessID = NULL;
	g_AttachedProcessAddress = NULL;

	// Initialize the editor
	if (!Scintilla_RegisterClasses(m_hInstance))
	{
		AfxMessageBox(_T("Scintilla failed to initiailze"));
		return FALSE;
	}
	
	Console = new CDialogConsole(_T("Console"));
	if (Console->Create(CDialogConsole::IDD, CWnd::GetDesktopWindow()))
		Console->ShowWindow(SW_HIDE);
	
	g_SymLoader = new (std::nothrow) Symbols;
	if(g_SymLoader != nullptr) 
	{
		PrintOut(_T("Symbol resolution enabled"));
		gbSymbolResolution = true;
	} 
	else 
	{
		PrintOut(_T("Failed to init symbol loader, disabling globally"));
		gbSymbolResolution = false;
	}

	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	LoadPlugins();

	// Get debug privilege
	//Utils::SetDebugPrivilege(TRUE);
	
	return TRUE;
}

int CReClass2016App::ExitInstance()
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);

	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	if (Console) 
		delete Console;

	if(g_SymLoader)
		delete g_SymLoader;

	AfxOleTerm(FALSE);

	// Release Scintilla
	Scintilla_ReleaseResources();

	WriteProfileString(_T("Typedefs"), _T("tdHex"), tdHex);
	WriteProfileString(_T("Typedefs"), _T("tdInt64"), tdInt64);
	WriteProfileString(_T("Typedefs"), _T("tdInt32"), tdInt32);
	WriteProfileString(_T("Typedefs"), _T("tdInt16"), tdInt16);
	WriteProfileString(_T("Typedefs"), _T("tdInt8"), tdInt8);
	WriteProfileString(_T("Typedefs"), _T("tdQWORD"), tdQWORD);
	WriteProfileString(_T("Typedefs"), _T("tdDWORD"), tdDWORD);
	WriteProfileString(_T("Typedefs"), _T("tdWORD"), tdWORD);
	WriteProfileString(_T("Typedefs"), _T("tdBYTE"), tdBYTE);
	WriteProfileString(_T("Typedefs"), _T("tdFloat"), tdFloat);
	WriteProfileString(_T("Typedefs"), _T("tdDouble"), tdDouble);
	WriteProfileString(_T("Typedefs"), _T("tdVec2"), tdVec2);
	WriteProfileString(_T("Typedefs"), _T("tdVec3"), tdVec3);
	WriteProfileString(_T("Typedefs"), _T("tdQuat"), tdQuat);
	WriteProfileString(_T("Typedefs"), _T("tdMatrix"), tdMatrix);
	WriteProfileString(_T("Typedefs"), _T("tdPChar"), tdPChar);
	WriteProfileString(_T("Typedefs"), _T("tdPWChar"), tdPWChar);

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
	WriteProfileInt(_T("Display"), _T("gbPrivatePadding"), gbPrivatePadding);
	WriteProfileInt(_T("Display"), _T("gbClipboardCopy"), gbClipboardCopy);

	for ( auto plugin : LoadedPlugins )
		FreeLibrary( plugin.LoadedBase );

	return CWinAppEx::ExitInstance();
}

void CReClass2016App::OnButtonReset()
{
	CloseHandle(g_hProcess);
	g_hProcess = NULL;
	g_ProcessID = 0;
	g_AttachedProcessAddress = NULL;

	CMDIFrameWnd* pFrame = static_cast<CMDIFrameWnd*>(AfxGetApp()->m_pMainWnd);
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

	CurrentFilePath = "";
}

void CReClass2016App::OnButtonPause()
{
	PauseResumeThreadList(false);
}

void CReClass2016App::OnUpdateButtonPause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2016App::OnButtonResume()
{
	PauseResumeThreadList(true);
}

void CReClass2016App::OnUpdateButtonResume(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2016App::OnButtonKill()
{
	TerminateProcess(g_hProcess, 0);
	g_hProcess = NULL;
}

void CReClass2016App::OnUpdateButtonKill(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2016App::CalcOffsets(CNodeClass* pClass)
{
	DWORD offset = 0;
	for (UINT i = 0; i < pClass->Nodes.size(); i++)
	{
		pClass->Nodes[i]->offset = offset;
		offset += pClass->Nodes[i]->GetMemorySize();
	}
}

void CReClass2016App::CalcAllOffsets()
{
	for (UINT i = 0; i < Classes.size(); i++)
		CalcOffsets(Classes[i]);
}

void CReClass2016App::OnFileNew()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, m_hMDIMenu, m_hMDIAccel);

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
void CReClass2016App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CReClass2016App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CReClass2016App::LoadCustomState()
{
}

void CReClass2016App::SaveCustomState()
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
void CReClass2016App::OnFileImport()
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
	//			//CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, m_hMDIMenu, m_hMDIAccel);
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

void CReClass2016App::ClearSelection()
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

void CReClass2016App::ClearHidden()
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

bool CReClass2016App::IsNodeValid(CNodeBase* pCheckNode)
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
void CReClass2016App::OnButtonNewClass()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, m_hMDIMenu, m_hMDIAccel);

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

void CReClass2016App::OnButtonSearch()
{
	GetMainWnd( )->MessageBox( _T( "Coming Soon!" ), _T( "ReClass 2016" ) );
}

void CReClass2016App::OnUpdateButtonSearch(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hProcess != NULL);
}

void CReClass2016App::OnButtonConsole()
{
	Console->ShowWindow(SW_SHOW);
	Console->SetForegroundWindow();
}

void CReClass2016App::OnButtonModules()
{
	PrintOut(_T("OnButtonModules called"));
	CDialogModules dlg;
	dlg.DoModal();
}

void CReClass2016App::OnUpdateButtonModules( CCmdUI * pCmdU )
{ 
	pCmdU->Enable( g_hProcess != NULL );
}

void CReClass2016App::OnButtonNotes()
{
	PrintOut(_T("OnButtonNotes called"));
	CDialogEdit dlg;
	dlg.Title = _T("Notes");
	dlg.Text = Notes;
	dlg.DoModal();
	Notes = dlg.Text;
}

void CReClass2016App::OnButtonParser()
{
	CDialogClasses dlg;
	dlg.DoModal();
}

void CReClass2016App::OnButtonHeader()
{
	PrintOut(_T("OnButtonHeader called"));
	CDialogEdit dlg;
	dlg.Title = _T("Header");
	dlg.Text = Header;
	dlg.DoModal();
	Header = dlg.Text;
}

void CReClass2016App::OnButtonFooter()
{
	PrintOut(_T("OnButtonFooter called"));
	CDialogEdit dlg;
	dlg.Title = _T("Footer");
	dlg.Text = Footer;
	dlg.DoModal();
	Footer = dlg.Text;
}

CNodeBase* CReClass2016App::CreateNewNode(NodeType Type)
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

	if (Type == nt_uint64) return new CNodeQWORD;
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
	if (Type == nt_pwchar) return new CNodeWCharPtr;
	if (Type == nt_unicode) return new CNodeUnicode;

	if (Type == nt_vtable) return new CNodeVTable;
	if (Type == nt_function) return new CNodeFunctionPtr;

	if (Type == nt_pointer) return new CNodePtr;
	if (Type == nt_array) return new CNodeArray;
	if (Type == nt_instance) return new CNodeClassInstance;

	return NULL;
}

void CReClass2016App::SaveXML(TCHAR* FileName)
{
	PrintOut(_T("SaveXML(\"%s\") called"), FileName);

	TiXMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(/*"xml version = \"1.0\" encoding=\"UTF-8\""*/);
	doc.LinkEndChild(decl);

	XMLElement* root = doc.NewElement("ReClass");
	doc.LinkEndChild(root);

	XMLComment* comment = doc.NewComment("Reclass 2016");
	root->LinkEndChild(comment);
	//---------------------------------------------
	XMLElement* settings = doc.NewElement("TypeDef");
#ifdef UNICODE
	settings->SetAttribute("tdHex", CW2A(tdHex));
	settings->SetAttribute("tdInt64", CW2A(tdInt64));
	settings->SetAttribute("tdInt32", CW2A(tdInt32));
	settings->SetAttribute("tdInt16", CW2A(tdInt16));
	settings->SetAttribute("tdInt8", CW2A(tdInt8));
	settings->SetAttribute("tdQWORD", CW2A(tdQWORD));
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
	settings->SetAttribute("tdPWChar", CW2A(tdPWChar));
#else
	settings->SetAttribute("tdHex",	  tdHex);
	settings->SetAttribute("tdInt64", tdInt64);
	settings->SetAttribute("tdInt32", tdInt32);
	settings->SetAttribute("tdInt16", tdInt16);
	settings->SetAttribute("tdInt8",  tdInt8);
	settings->SetAttribute("tdQWORD", tdQWORD);
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
	settings->SetAttribute("tdPWChar", tdPWChar);
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

void CReClass2016App::OnFileSave()
{
	SaveXML(CurrentFilePath.GetBuffer());
}

void CReClass2016App::OnFileSaveAs()
{
	TCHAR Filters[] = _T("ReClass (*.reclass)|*.reclass|All Files (*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("reclass"), _T(""), OFN_HIDEREADONLY, Filters, NULL);
	if (fileDlg.DoModal() != IDOK)
		return;

	CString pathName = fileDlg.GetPathName();
	CurrentFilePath = pathName;
	SaveXML(pathName.GetBuffer());
}

void CReClass2016App::OnFileOpen()
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

void CReClass2016App::OnButtonGenerate()
{
	PrintOut(_T("OnButtonGenerate() called"));

	CString generated_text, t;

	generated_text += _T("// Generated using ReClass 2016\r\n\r\n");
	
	if(!Header.IsEmpty())
		generated_text += Header + _T("\r\n\r\n");

	for (UINT c = 0; c < Classes.size(); c++)
	{
		t.Format(_T("class %s;\r\n"), Classes[c]->Name);
		generated_text += t;
	}

	generated_text += _T("\r\n");

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
					if ( gbPrivatePadding )
						t.Format( _T( "private:\r\n\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\npublic:\r\n" ), tdHex, fillStart, fill, fillStart );
					else
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
			if(Type == nt_uint64)
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), tdQWORD, pNode->Name, pNode->offset, pNode->Comment );
				var.push_back( t );
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
			if ( Type == nt_pwchar )
			{
				t.Format( _T( "\t%s %s; //0x%0.4X %s\r\n" ), tdPWChar, pNode->Name, pNode->offset, pNode->Comment );
				var.push_back( t );
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
			if ( gbPrivatePadding )
				t.Format(_T("private:\r\n\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n"), tdHex, fillStart, fill, fillStart); //Maybe add public at the end for user impl of class inline functions?: public:\r\n
			else
				t.Format(_T("\t%s pad_0x%0.4X[0x%X]; //0x%0.4X\r\n"), tdHex, fillStart, fill, fillStart);

			var.push_back(t);
		}

		t.Format(_T("%s\r\n{\r\npublic:\r\n"), ClassName);
		generated_text += t;

		for (UINT i = 0; i < vfun.size(); i++)
			generated_text += vfun[i];

		if (vfun.size() > 0)
			generated_text += _T("\r\n");

		for (UINT i = 0; i < var.size(); i++)
			generated_text += var[i];

		if (var.size() > 0)
			generated_text += _T("\r\n");

		if (pClass->Code.GetLength() > 0)
		{
			generated_text += pClass->Code;
			generated_text += _T("\r\n");
		}

		t.Format(_T("}; //Size=0x%0.4X\r\n\r\n"), pClass->GetMemorySize());
		generated_text += t;
	}

	if (!Footer.IsEmpty())
		generated_text += (Footer + _T("\r\n"));

	if (gbClipboardCopy)
	{
		::OpenClipboard(NULL);
		::EmptyClipboard();
		int string_size = generated_text.GetLength() * sizeof(CString::StrTraits::XCHAR);
		HGLOBAL memory_blob = ::GlobalAlloc(GMEM_FIXED, string_size);
		memcpy(memory_blob, generated_text.GetBuffer(), string_size);
#ifdef UNICODE
		::SetClipboardData(CF_UNICODETEXT, memory_blob);
#else
		::SetClipboardData(CF_TEXT, memory_blob);
#endif
		::CloseClipboard();
		GetMainWnd()->MessageBox(_T("Coppied generated code to clipboard..."), _T("ReClass 2016"), MB_OK | MB_ICONINFORMATION);
	} else {
		CDialogEdit dlg;
		dlg.Title = _T( "Class Code Generated" );
		dlg.Text = generated_text;
		dlg.DoModal();
	}
}

void CReClass2016App::OnButtonPlugins()
{
	CDialogPlugins plugin_dlg;
	plugin_dlg.DoModal( );
}

void CReClass2016App::OnUpdateButtonPlugins( CCmdUI * pCmdUI )
{ 
	pCmdUI->Enable(!LoadedPlugins.empty());
}

void CReClass2016App::OnOpenPDB()
{
	PrintOut(_T("OnOpenPDB() called"));

	CString concat_name = g_ProcessName;
	if(concat_name.ReverseFind('.') != -1)
		concat_name.Truncate(concat_name.ReverseFind('.'));

	CFileDialog fileDlg { TRUE, _T( "pdb" ), concat_name, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "PDB (*.pdb)|*.pdb|All Files (*.*)|*.*||" ), NULL };
	if (fileDlg.DoModal() != IDOK)
		return;

	//pdb.LoadFile(fileDlg.GetPathName());
}

void CReClass2016App::OnUpdateOpenPDB(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((g_ProcessID != NULL));
}

void CReClass2016App::DeleteClass(CNodeClass* pClass)
{
	PrintOut(_T("DeleteClass(\"%s\") called"), pClass->Name.GetString());

	CNodeBase* pNode = IsNodeRef(pClass);
	if (pNode)
	{
		PrintOut(_T("Class still has a reference in %s.%s"), pNode->pParent->Name.GetString(), pNode->Name.GetString());
		CString msg;
		msg.Format(_T("Class still has a reference in %s.%s"), pNode->pParent->Name.GetString(), pNode->Name.GetString());
		GetMainWnd( )->MessageBox( msg );
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

CNodeBase* CReClass2016App::IsNodeRef(CNodeBase* pTestNode)
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

void CReClass2016App::OnButtonClean()
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
		if (IsNodeRef(Classes[i]) == NULL)
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

void CReClass2016App::OnUpdateButtonClean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}

void CReClass2016App::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((CurrentFilePath.GetLength() > 0));
}

