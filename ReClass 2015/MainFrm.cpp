#include "stdafx.h"
#include "ReClass2015.h"
#include "Classes.h"
#include "ChildFrm.h"
#include "Debug.h"
#include "DialogClasses.h"
#include "MainFrm.h"

#ifndef NDEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	//ON_COMMAND(ID_BUTTON_SHOWCLASSES, &CMainFrame::OnButtonShowclasses)
	ON_COMMAND(ID_BUTTON_CBACKGROUND, &CMainFrame::OnButtonCbackground)
	ON_COMMAND(ID_BUTTON_CSELECT, &CMainFrame::OnButtonCselect)
	ON_COMMAND(ID_BUTTON_CHIDDEN, &CMainFrame::OnButtonChidden)
	ON_COMMAND(ID_BUTTON_COFFSET, &CMainFrame::OnButtonCoffset)
	ON_COMMAND(ID_BUTTON_CADDRESS, &CMainFrame::OnButtonCaddress)
	ON_COMMAND(ID_BUTTON_CHEX, &CMainFrame::OnButtonChex)
	ON_COMMAND(ID_BUTTON_CTYPE, &CMainFrame::OnButtonCtype)
	ON_COMMAND(ID_BUTTON_CNAME, &CMainFrame::OnButtonCname)
	ON_COMMAND(ID_BUTTON_CVALUE, &CMainFrame::OnButtonCvalue)
	ON_COMMAND(ID_BUTTON_CINDEX, &CMainFrame::OnButtonCindex)
	ON_COMMAND(ID_BUTTON_CCOMMENT, &CMainFrame::OnButtonCcomment)
	ON_COMMAND(ID_BUTTON_CTEXT, &CMainFrame::OnButtonCtext)
	ON_COMMAND(ID_BUTTON_CVTABLE, &CMainFrame::OnButtonCvtable)
	ON_COMMAND(ID_BUTTON_CFUNCTION, &CMainFrame::OnButtonCfunction)
	ON_COMMAND(ID_BUTTON_CCUSTOM, &CMainFrame::OnButtonCcustom)
	ON_COMMAND(ID_CHECK_CBADDRESS, &CMainFrame::OnCheckCbaddress)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBADDRESS, &CMainFrame::OnUpdateCheckCbaddress)
	ON_COMMAND(ID_CHECK_CBOFFSET, &CMainFrame::OnCheckCboffset)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBOFFSET, &CMainFrame::OnUpdateCheckCboffset)
	ON_COMMAND(ID_BUTTON_TYPEDEF, &CMainFrame::OnButtonTypedef)
	ON_COMMAND(ID_CHECK_CBTEXT, &CMainFrame::OnCheckCbtext)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBTEXT, &CMainFrame::OnUpdateCheckCbtext)
	//ON_COMMAND(ID_BUTTON_SELECT, &CMainFrame::OnButtonSelect)
	ON_COMMAND(ID_BUTTON_SELECTPROCESS, &CMainFrame::OnButtonSelectprocess)
	ON_COMMAND(ID_BUTTON_EDITCLASS, &CMainFrame::OnButtonEditclass)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EDITCLASS, &CMainFrame::OnUpdateButtonEditclass)
	ON_COMMAND(ID_BUTTON_DELETECLASS, &CMainFrame::OnButtonDeleteclass)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETECLASS, &CMainFrame::OnUpdateButtonDeleteclass)
	ON_WM_TIMER()
	ON_COMMAND(ID_CHECK_TOPMOST, &CMainFrame::OnCheckTopmost)
	ON_UPDATE_COMMAND_UI(ID_CHECK_TOPMOST, &CMainFrame::OnUpdateCheckTopmost)

	ON_COMMAND(ID_CHECK_CLASSBROWSER, &CMainFrame::OnCheckClassBrowser)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CLASSBROWSER, &CMainFrame::OnUpdateCheckClassBrowser)
	ON_COMMAND(ID_BUTTON_LEFT, &CMainFrame::OnButtonLeft)
	ON_COMMAND(ID_BUTTON_RIGHT, &CMainFrame::OnButtonRight)
	ON_COMMAND(ID_CHECK_FLOAT, &CMainFrame::OnCheckFloat)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FLOAT, &CMainFrame::OnUpdateCheckFloat)
	ON_COMMAND(ID_CHECK_INTEGER, &CMainFrame::OnCheckInteger)
	ON_UPDATE_COMMAND_UI(ID_CHECK_INTEGER, &CMainFrame::OnUpdateCheckInteger)
	ON_COMMAND(ID_CHECK_STRING, &CMainFrame::OnCheckString)
	ON_UPDATE_COMMAND_UI(ID_CHECK_STRING, &CMainFrame::OnUpdateCheckString)
	ON_COMMAND(ID_CHECK_POINTER, &CMainFrame::OnCheckPointer)
	ON_UPDATE_COMMAND_UI(ID_CHECK_POINTER, &CMainFrame::OnUpdateCheckPointer)
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt("ApplicationLook", ID_VIEW_APPLOOK_WIN_XP);
}

CMainFrame::~CMainFrame()
{
	ClearProcMenuItems();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager and style based on persisted value
	OnApplicationLook(ID_VIEW_APPLOOK_VS_2005/*theApp.m_nAppLook*/);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = FALSE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_IMMEDIATE);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	//Update Colors
	CMFCRibbonColorButton* pColor;
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CBACKGROUND); pColor->SetColor(crBackground);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CSELECT); pColor->SetColor(crSelect);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHIDDEN); pColor->SetColor(crHidden);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_COFFSET); pColor->SetColor(crOffset);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CADDRESS); pColor->SetColor(crAddress);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTYPE); pColor->SetColor(crType);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CNAME); pColor->SetColor(crName);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CINDEX); pColor->SetColor(crIndex);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVALUE); pColor->SetColor(crValue);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCOMMENT); pColor->SetColor(crComment);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVTABLE); pColor->SetColor(crVTable);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CFUNCTION); pColor->SetColor(crFunction);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTEXT); pColor->SetColor(crChar);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCUSTOM); pColor->SetColor(crCustom);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHEX); pColor->SetColor(crHex);

	//CreateConsole( );

	// Why?
	SetTimer( 69, 5000, NULL );

	return 0;
}
void CMainFrame::OnButtonCbackground()
{
	crBackground = ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CBACKGROUND))->GetColor();
}
void CMainFrame::OnButtonCselect()
{
	crSelect = ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CSELECT))->GetColor();
}
void CMainFrame::OnButtonChidden()
{
	crHidden= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHIDDEN))->GetColor();
}
void CMainFrame::OnButtonCoffset()
{
	crOffset= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_COFFSET))->GetColor();
}
void CMainFrame::OnButtonCaddress()
{
	crAddress= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CADDRESS))->GetColor();
}
void CMainFrame::OnButtonChex()
{
	crHex= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHEX))->GetColor();
}
void CMainFrame::OnButtonCtype()
{
	crType= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTYPE))->GetColor();
}
void CMainFrame::OnButtonCname()
{
	crName= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CNAME))->GetColor();
}
void CMainFrame::OnButtonCvalue()
{
	crValue= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVALUE))->GetColor();
}
void CMainFrame::OnButtonCindex()
{
	crIndex= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CINDEX))->GetColor();
}
void CMainFrame::OnButtonCcomment()
{
	crComment= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCOMMENT))->GetColor();
}
void CMainFrame::OnButtonCtext()
{
	crChar= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTEXT))->GetColor();
}
void CMainFrame::OnButtonCvtable()
{
	crVTable= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVTABLE))->GetColor();
}
void CMainFrame::OnButtonCfunction()
{
	crFunction= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CFUNCTION))->GetColor();
}
void CMainFrame::OnButtonCcustom()
{
	crCustom= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCUSTOM))->GetColor();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	if (gbTop) cs.dwExStyle |= WS_EX_TOPMOST;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	UpdateMDITabbedBarsIcons();
}

// CMainFrame message handlers
void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		if (nID >= WM_CLASSMENU && nID < (WM_CLASSMENU + WM_MAXITEMS))
		{ 
			((CCmdUI*)pExtra)->Enable(TRUE); 
			return TRUE;
		}
		else if (nID >= WM_PROCESSMENU && nID < (WM_PROCESSMENU + WM_MAXITEMS))
		{ 
			((CCmdUI*)pExtra)->Enable(TRUE); 
			return TRUE;
		}
		else if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS))
		{ 
			((CCmdUI*)pExtra)->Enable(TRUE); 
			return TRUE;
		}
	}
	if (nCode == CN_COMMAND)
	{
		if (nID >= WM_CLASSMENU && nID < (WM_CLASSMENU+WM_MAXITEMS) )
		{
			UINT idx = nID - WM_CLASSMENU;

			CChildFrame* pChild = (CChildFrame*)this->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel);
			CNodeClass* pClass = theApp.Classes[idx];

			pChild->SetTitle(pClass->Name);
			pChild->SetWindowTextA(pClass->Name);
			UpdateFrameTitleForDocument(pClass->Name);
			pChild->m_wndView.m_pClass = pClass;
			return TRUE;
		}
		if (nID >= WM_PROCESSMENU && nID < (WM_PROCESSMENU+WM_MAXITEMS) )
		{
			UINT idx = nID - WM_PROCESSMENU;
			ProcessID = ProcMenuItems[idx].ProcessId;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS,false,ProcessID);
			UpdateMemoryMap();
			return TRUE;
		}
		if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU+WM_MAXITEMS) )
		{
			UINT idx = nID - WM_DELETECLASSMENU;
			theApp.DeleteClass(theApp.Classes[idx]);
			return TRUE;
		}
	}


	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnCheckCbaddress()
{
	gbAddress = !gbAddress;
}
void CMainFrame::OnUpdateCheckCbaddress(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbAddress);
}

void CMainFrame::OnCheckCboffset()
{
	gbOffset = !gbOffset;
}
void CMainFrame::OnUpdateCheckCboffset(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbOffset);
}

void CMainFrame::OnCheckCbtext()
{
	gbText = !gbText;
}
void CMainFrame::OnUpdateCheckCbtext(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbText);
}

#include "DialogTypes.h"
void CMainFrame::OnButtonTypedef()
{
	CDialogTypes dlg;
	dlg.DoModal();
}

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
// TODO fix to is64bit
BOOL is64bit(HANDLE hProcess)
{
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
	if (!fnIsWow64Process)
		return TRUE;//WTF update, assume 32bit noob

	BOOL bIs64BitOS = FALSE;
	fnIsWow64Process( GetCurrentProcess( ), &bIs64BitOS ); //are we running as a 32bit in a 64bit OS (assume we compiled as a 32bit process)
	if (bIs64BitOS)
	{
		BOOL bIs32Bit;
		fnIsWow64Process( hProcess,&bIs32Bit ); //is this 32bit process in a 64bit OS?
		return bIs32Bit;
	}
	else
		return TRUE;// 32bit OS, so it's a 32bit process
}
BOOL is32Bit(HANDLE hProcess)
{
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	if (!fnIsWow64Process)
		return TRUE; // WTF update, assume 32bit noob

	BOOL bIs64BitOS = FALSE;
	fnIsWow64Process(GetCurrentProcess(), &bIs64BitOS);//are we running as a 32bit in a 64bit OS (assume we compiled as a 32bit process)
	if (bIs64BitOS)
	{
		BOOL bIs32Bit;
		fnIsWow64Process(hProcess, &bIs32Bit);//is this 32bit process in a 64bit OS?
		return bIs32Bit;
	}
	else
		return TRUE;// 32bit OS, so it's a 32bit process
}

std::string CommonProcesses[] = 
{
	"svchost.exe",	"conhost.exe",	"wininit.exe",	"smss.exe",
	"winint.exe",	"wlanext.exe",	"spoolsv.exe",	"spoolsv.exe",
	"notepad.exe",	"explorer.exe", "itunes.exe",	"sqlservr.exe",
	"nvtray.exe",	"nvxdsync.exe", "lsass.exe",	"jusched.exe",
	"conhost.exe",  "chrome.exe",	"firefox.exe", 	"winamp.exe",
	"WinRAR.exe",	"calc.exe",		"taskhostex.exe", "Taskmgr.exe",
	"plugin-container.exe"
};

void CMainFrame::OnButtonSelectprocess()
{
	CMFCRibbonButton* pButton = (CMFCRibbonButton*)m_wndRibbonBar.FindByID( ID_BUTTON_SELECTPROCESS );
	CRect pos = pButton->GetRect( );
	ClientToScreen( &pos );

	CMenu menu;
	menu.CreatePopupMenu( );

	ClearProcMenuItems( );

#ifdef _WIN64
	HANDLE ProcessList = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	if (ProcessList != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 ProcInfo;
		ProcInfo.dwSize	= sizeof( PROCESSENTRY32 );
		BOOL rp = Process32First( ProcessList, &ProcInfo );

		bool bSkip = false;

		while( rp == TRUE )
		{
			// Are we filtering out processes
			if (gbFilterProcesses)
			{
				for (int i = 0; i < sizeof(CommonProcesses) / sizeof(CommonProcesses[0]) ; i++)
				{
					if (strcmp( ProcInfo.szExeFile, CommonProcesses[i].c_str()) == 0 )
					{
						//printf( "True %s\n", ProcInfo.szExeFile );
						bSkip = true;
					}
				}
			}

			if (bSkip)
			{
				bSkip = false;
				rp = Process32Next(ProcessList,&ProcInfo);
				continue;
			}

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcInfo.th32ProcessID);
			if (hProcess)
			{
				if (is64bit( hProcess ))
				{
					char filename[1024];
					GetModuleFileNameEx(hProcess, NULL, filename, 1024);

					SHFILEINFO    sfi;
					SHGetFileInfo(filename,FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

					CBitmap* pBitmap = new CBitmap;
					CProcessMenuInfo Item;
					Item.ProcessId = ProcInfo.th32ProcessID;
					Item.pBitmap = pBitmap;

					CClientDC clDC(this);
					CDC dc;
					dc.CreateCompatibleDC(&clDC);

					int cx = 16;
					int cy = 16;
					pBitmap->CreateCompatibleBitmap(&clDC, cx, cy);
					CBitmap* pOldBmp = dc.SelectObject(pBitmap);

					dc.FillSolidRect(0, 0, cx, cy, GetSysColor(COLOR_3DFACE));
					::DrawIconEx(dc.GetSafeHdc(), 0, 0, sfi.hIcon, cx, cy, 0, NULL,DI_NORMAL);

					dc.SelectObject(pOldBmp);
					dc.DeleteDC();

					DWORD MsgID = WM_PROCESSMENU + ProcMenuItems.size();
					menu.AppendMenu( MF_STRING | MF_ENABLED, MsgID , ProcInfo.szExeFile );
					menu.SetMenuItemBitmaps(MsgID, MF_BYCOMMAND, pBitmap, pBitmap);

					ProcMenuItems.push_back(Item); 
				}
				CloseHandle(hProcess);
			}
			rp = Process32Next(ProcessList,&ProcInfo);
		}
		CloseHandle(ProcessList);
	}
#else
	HANDLE ProcessList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (ProcessList != INVALID_HANDLE_VALUE) 
	{
		PROCESSENTRY32 ProcInfo;
		ProcInfo.dwSize	= sizeof( PROCESSENTRY32 );
		BOOL rp = Process32First(ProcessList, &ProcInfo);
		while( rp == TRUE )
		{			 
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ProcInfo.th32ProcessID);
			if (hProcess)
			{
				if (is32Bit(hProcess))
				{
					char filename[1024];
					GetModuleFileNameEx(hProcess, NULL, filename, 1024);

					SHFILEINFO    sfi;
					SHGetFileInfo(filename, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

					CBitmap* pBitmap = new CBitmap;
					CProcessMenuInfo Item;
					Item.ProcessId = ProcInfo.th32ProcessID;
					Item.pBitmap = pBitmap;

					CClientDC clDC(this);
					CDC dc;
					dc.CreateCompatibleDC(&clDC);

					int cx = 16;
					int cy = 16;
					pBitmap->CreateCompatibleBitmap(&clDC, cx, cy);
					CBitmap* pOldBmp = dc.SelectObject(pBitmap);

					dc.FillSolidRect(0, 0, cx, cy, GetSysColor(COLOR_3DFACE));
					::DrawIconEx(dc.GetSafeHdc(), 0, 0, sfi.hIcon, cx, cy, 0, NULL, DI_NORMAL);

					dc.SelectObject(pOldBmp);
					dc.DeleteDC();

					DWORD MsgID = WM_PROCESSMENU + ProcMenuItems.size();
					menu.AppendMenu(MF_STRING | MF_ENABLED, MsgID , ProcInfo.szExeFile);
					menu.SetMenuItemBitmaps(MsgID, MF_BYCOMMAND, pBitmap, pBitmap);

					ProcMenuItems.push_back(Item);
				}
				CloseHandle(hProcess);
			}
			rp = Process32Next(ProcessList, &ProcInfo);
		}
		CloseHandle(ProcessList);
	}
#endif

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this);
}
void CMainFrame::ClearProcMenuItems()
{
	for (UINT i = 0; i < ProcMenuItems.size(); i++)
	{
		ProcMenuItems[i].pBitmap->DeleteObject();
		delete ProcMenuItems[i].pBitmap;
	}
	ProcMenuItems.clear();
}


void CMainFrame::OnButtonEditclass()
{
	if ( gbClassBrowser )
	{	
		CDialogClasses dlg;
		dlg.DoModal( );
	} 
	else 
	{
		// TODO: instead of menu popup a window
		CMFCRibbonBaseElement* pButton = m_wndRibbonBar.FindByID(ID_BUTTON_EDITCLASS);

		CRect pos = pButton->GetRect( );
		ClientToScreen( &pos );

		CMenu menu;
		menu.CreatePopupMenu( );

		for ( UINT m = 0; m < theApp.Classes.size( ); m++ )
		{
			CString MenuItem;
			MenuItem.Format( "%i - %s", m, theApp.Classes[m]->Name );

			menu.AppendMenu( MF_STRING | MF_ENABLED, WM_CLASSMENU + m, MenuItem );
		}

		menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this );
	}
}

void CMainFrame::OnButtonDeleteclass()
{
	CMFCRibbonBaseElement* pButton = m_wndRibbonBar.FindByID(ID_BUTTON_DELETECLASS);

	CRect pos = pButton->GetRect();
	ClientToScreen(&pos);

	CMenu menu;
	menu.CreatePopupMenu();
	for ( UINT m = 0; m < theApp.Classes.size(); m++ )
	{
		menu.AppendMenu(MF_STRING | MF_ENABLED, WM_DELETECLASSMENU + m, theApp.Classes[m]->Name);
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this);
}

void CMainFrame::OnUpdateButtonEditclass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}

void CMainFrame::OnUpdateButtonDeleteclass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 69)
		UpdateMemoryMap();
	CMDIFrameWndEx::OnTimer(nIDEvent);
}

// TODO toggle this
void CMainFrame::OnCheckTopmost()
{
	gbTop = !gbTop;
	if (gbTop)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	else
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
}

void CMainFrame::OnUpdateCheckTopmost(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbTop);
}

void CMainFrame::OnCheckClassBrowser()
{
	gbClassBrowser = !gbClassBrowser;
}

void CMainFrame::OnUpdateCheckClassBrowser(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbClassBrowser);
}

void CMainFrame::OnButtonLeft()
{
	RECT Screen;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &Screen, 0);
	SetWindowPos(NULL, 0, 0, Screen.right/2, Screen.bottom, SWP_NOZORDER);
}

void CMainFrame::OnButtonRight()
{
	RECT Screen;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &Screen, 0);
	SetWindowPos(NULL, Screen.right/2, 0, Screen.right/2, Screen.bottom, SWP_NOZORDER);
}

void CMainFrame::OnCheckFloat()
{
	gbFloat = !gbFloat;
}

void CMainFrame::OnUpdateCheckFloat(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbFloat);
}

void CMainFrame::OnCheckInteger()
{
	gbInt = !gbInt;
}

void CMainFrame::OnUpdateCheckInteger(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbInt);
}

void CMainFrame::OnCheckString()
{
	gbString = !gbString;
}

void CMainFrame::OnUpdateCheckString(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbString);
}

void CMainFrame::OnCheckPointer()
{
	gbPointers = !gbPointers;
}

void CMainFrame::OnUpdateCheckPointer(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbPointers);
}
