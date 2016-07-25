#include "stdafx.h"
#include "ReClass2015.h"
#include "Classes.h"
#include "ChildFrm.h"
#include "Debug.h"
#include "DialogClasses.h"
#include "MainFrm.h"

// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_BUTTON_SHOWCLASSES, &CMainFrame::OnButtonShowclasses)
	ON_COMMAND(ID_BUTTON_CBACKGROUND, &CMainFrame::OnButtonCBackground)
	ON_COMMAND(ID_BUTTON_CSELECT, &CMainFrame::OnButtonCSelect)
	ON_COMMAND(ID_BUTTON_CHIDDEN, &CMainFrame::OnButtonCHidden)
	ON_COMMAND(ID_BUTTON_COFFSET, &CMainFrame::OnButtonCOffset)
	ON_COMMAND(ID_BUTTON_CADDRESS, &CMainFrame::OnButtonCAddress)
	ON_COMMAND(ID_BUTTON_CHEX, &CMainFrame::OnButtonCHex)
	ON_COMMAND(ID_BUTTON_CTYPE, &CMainFrame::OnButtonCType)
	ON_COMMAND(ID_BUTTON_CNAME, &CMainFrame::OnButtonCName)
	ON_COMMAND(ID_BUTTON_CVALUE, &CMainFrame::OnButtonCValue)
	ON_COMMAND(ID_BUTTON_CINDEX, &CMainFrame::OnButtonCIndex)
	ON_COMMAND(ID_BUTTON_CCOMMENT, &CMainFrame::OnButtonCComment)
	ON_COMMAND(ID_BUTTON_CTEXT, &CMainFrame::OnButtonCText)
	ON_COMMAND(ID_BUTTON_CVTABLE, &CMainFrame::OnButtonCVtable)
	ON_COMMAND(ID_BUTTON_CFUNCTION, &CMainFrame::OnButtonCFunction)
	ON_COMMAND(ID_BUTTON_CCUSTOM, &CMainFrame::OnButtonCCustom)
	ON_COMMAND(ID_CHECK_CBADDRESS, &CMainFrame::OnCheckCbAddress)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBADDRESS, &CMainFrame::OnUpdateCheckCbAddress)
	ON_COMMAND(ID_CHECK_CBOFFSET, &CMainFrame::OnCheckCbOffset)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBOFFSET, &CMainFrame::OnUpdateCheckCbOffset)
	ON_COMMAND(ID_BUTTON_TYPEDEF, &CMainFrame::OnButtonTypedef)
	ON_COMMAND(ID_CHECK_CBTEXT, &CMainFrame::OnCheckCbText)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBTEXT, &CMainFrame::OnUpdateCheckCbText)
	ON_COMMAND(ID_CHECK_CBRTTI, &CMainFrame::OnCheckCbRtti)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CBRTTI, &CMainFrame::OnUpdateCheckCbRtti)
	//ON_COMMAND(ID_BUTTON_SELECT, &CMainFrame::OnButtonSelect)
	ON_COMMAND(ID_BUTTON_SELECTPROCESS, &CMainFrame::OnButtonSelectProcess)
	ON_COMMAND(ID_BUTTON_EDITCLASS, &CMainFrame::OnButtonEditClass)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EDITCLASS, &CMainFrame::OnUpdateButtonEditClass)
	ON_COMMAND(ID_BUTTON_DELETECLASS, &CMainFrame::OnButtonDeleteClass)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DELETECLASS, &CMainFrame::OnUpdateButtonDeleteClass)
	ON_COMMAND(ID_CHECK_TOPMOST, &CMainFrame::OnCheckTopmost)
	ON_UPDATE_COMMAND_UI(ID_CHECK_TOPMOST, &CMainFrame::OnUpdateCheckTopmost)
	ON_COMMAND(ID_CHECK_FILTERPROCESSES, &CMainFrame::OnCheckFilterProcesses)
	ON_UPDATE_COMMAND_UI(ID_CHECK_FILTERPROCESSES, &CMainFrame::OnUpdateCheckFilterProcesses)
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
	ON_COMMAND(ID_CHECK_CLIP_COPY, &CMainFrame::OnCheckClipboardCopy)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CLIP_COPY, &CMainFrame::OnUpdateCheckClipboardCopy)
	ON_COMMAND(ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnCheckPrivatePadding)
	ON_UPDATE_COMMAND_UI(ID_CHECK_PRIVATE_PADDING, &CMainFrame::OnUpdateCheckPrivatePadding)
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
}

CMainFrame::~CMainFrame()
{
	ClearProcMenuItems();
}

#include <afxtabctrl.h>
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Create tabs
	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D; // other styles available...
	mdiTabParams.m_tabLocation = CMFCTabCtrl::LOCATION_TOP;
	mdiTabParams.m_nTabBorderSize = 4;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at left of tab area
	mdiTabParams.m_bTabIcons = TRUE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the left edge of the tab area
	mdiTabParams.m_bFlatFrame = FALSE;
	EnableMDITabbedGroups(TRUE, mdiTabParams);


	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_STANDARD);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_BORDER_3D);

	// create docking windows
	if (!CreateDockingWindows())
	{
		PrintOut(_T("Failed to create docking windows\n"));
		return -1;
	}

	//SetTitle("Reclass 2015");

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	//Update Colors
	CMFCRibbonColorButton* pColor;
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CBACKGROUND);	pColor->SetColor(crBackground);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CSELECT);		pColor->SetColor(crSelect);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHIDDEN);		pColor->SetColor(crHidden);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_COFFSET);		pColor->SetColor(crOffset);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CADDRESS);		pColor->SetColor(crAddress);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTYPE);			pColor->SetColor(crType);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CNAME);			pColor->SetColor(crName);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CINDEX);			pColor->SetColor(crIndex);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVALUE);			pColor->SetColor(crValue);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCOMMENT);		pColor->SetColor(crComment);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVTABLE);		pColor->SetColor(crVTable);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CFUNCTION);		pColor->SetColor(crFunction);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTEXT);			pColor->SetColor(crChar);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCUSTOM);		pColor->SetColor(crCustom);
	pColor = (CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHEX);			pColor->SetColor(crHex);

	// update after 5 seconds
	SetTimer(TIMER_MEMORYMAP_UPDATE, 5000, NULL);

	return 0;
}
void CMainFrame::OnButtonCBackground()
{
	crBackground = ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CBACKGROUND))->GetColor();
}
void CMainFrame::OnButtonCSelect()
{
	crSelect = ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CSELECT))->GetColor();
}
void CMainFrame::OnButtonCHidden()
{
	crHidden= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHIDDEN))->GetColor();
}
void CMainFrame::OnButtonCOffset()
{
	crOffset= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_COFFSET))->GetColor();
}
void CMainFrame::OnButtonCAddress()
{
	crAddress= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CADDRESS))->GetColor();
}
void CMainFrame::OnButtonCHex()
{
	crHex= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CHEX))->GetColor();
}
void CMainFrame::OnButtonCType()
{
	crType= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTYPE))->GetColor();
}
void CMainFrame::OnButtonCName()
{
	crName= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CNAME))->GetColor();
}
void CMainFrame::OnButtonCValue()
{
	crValue= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVALUE))->GetColor();
}
void CMainFrame::OnButtonCIndex()
{
	crIndex= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CINDEX))->GetColor();
}
void CMainFrame::OnButtonCComment()
{
	crComment= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCOMMENT))->GetColor();
}
void CMainFrame::OnButtonCText()
{
	crChar= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CTEXT))->GetColor();
}
void CMainFrame::OnButtonCVtable()
{
	crVTable= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CVTABLE))->GetColor();
}
void CMainFrame::OnButtonCFunction()
{
	crFunction= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CFUNCTION))->GetColor();
}
void CMainFrame::OnButtonCCustom()
{
	crCustom= ((CMFCRibbonColorButton*)m_wndRibbonBar.FindByID(ID_BUTTON_CCUSTOM))->GetColor();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CMDIFrameWndEx::PreCreateWindow(cs)) 
		return FALSE;
	if (gbTop) 
		cs.dwExStyle |= WS_EX_TOPMOST;
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

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG

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
		break;
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
		if (nID >= WM_CLASSMENU && nID < (WM_CLASSMENU + WM_MAXITEMS))
		{
			UINT idx = nID - WM_CLASSMENU;

			CChildFrame* pChild = (CChildFrame*)this->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel);
			CNodeClass* pClass = theApp.Classes[idx];
			pClass->pChildWindow = pChild;

			pChild->SetTitle(pClass->Name);
			pChild->SetWindowText(pClass->Name);
			UpdateFrameTitleForDocument(pClass->Name);
			pChild->m_wndView.m_pClass = pClass;
			return TRUE;
		}
		if (nID >= WM_PROCESSMENU && nID < (WM_PROCESSMENU + WM_MAXITEMS) )
		{
			UINT idx = nID - WM_PROCESSMENU;
			
			g_ProcessID = ProcMenuItems[idx].ProcessId;
			g_hProcess = ReClassOpenProcess(PROCESS_ALL_ACCESS, false, g_ProcessID);
			g_ProcessName = ProcMenuItems[idx].Procname;

			// Update memory map
			UpdateMemoryMap();

			// Init pdb
			//pdb.Init();

			return TRUE;
		}
		if (nID >= WM_DELETECLASSMENU && nID < (WM_DELETECLASSMENU + WM_MAXITEMS) )
		{
			UINT idx = nID - WM_DELETECLASSMENU;
			if (theApp.Classes[idx]->pChildWindow)
				theApp.Classes[idx]->pChildWindow->SendMessage(WM_CLOSE, 0, 0);
			theApp.DeleteClass(theApp.Classes[idx]);
			return TRUE;
		}
	}

	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnCheckCbAddress()
{
	gbAddress = !gbAddress;
}
void CMainFrame::OnUpdateCheckCbAddress(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbAddress);
}

void CMainFrame::OnCheckCbOffset()
{
	gbOffset = !gbOffset;
}
void CMainFrame::OnUpdateCheckCbOffset(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbOffset);
}

void CMainFrame::OnCheckCbText()
{
	gbText = !gbText;
}
void CMainFrame::OnUpdateCheckCbText(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbText);
}

void CMainFrame::OnCheckCbRtti()
{
	gbRTTI = !gbRTTI;
}

void CMainFrame::OnUpdateCheckCbRtti(CCmdUI *pCmdUI)
{
	if (!gbPointers) 
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(gbRTTI);
	}
}

#include "DialogTypes.h"
void CMainFrame::OnButtonTypedef()
{
	CDialogTypes dlg;
	dlg.DoModal();
}

// Annoying processes to filter out that you probably won't be looking in the memory of
static const char* CommonProcesses[] = 
{
	"svchost.exe", "conhost.exe", "wininit.exe", "smss.exe","winint.exe", "wlanext.exe",
	"spoolsv.exe", "spoolsv.exe","notepad.exe", "explorer.exe", "itunes.exe",
	"sqlservr.exe", "nvtray.exe", "nvxdsync.exe", "lsass.exe", "jusched.exe",
	"conhost.exe", "chrome.exe", "firefox.exe", "winamp.exe", "TrustedInstaller.exe",
	"WinRAR.exe", "calc.exe", "taskhostex.exe", "Taskmgr.exe", "dwm.exe", "SpotifyWebHelper.exe"
	"plugin-container.exe", "services.exe","devenv.exe", "flux.exe", "skype.exe", "spotify.exe", 
	"csrss.exe", "taskeng.exe","spotifyhelper.exe", "vcpkgsrv.exe", "msbuild.exe", "cmd.exe", "taskhost.exe",
	"SettingSyncHost.exe", "SkyDrive.exe", "ctfmon.exe", "RuntimeBroker.exe","BTTray.exe", "BTStackServer.exe", 
	"Bluetooth Headset Helper.exe", "winlogon.exe", "PnkBstrA.exe", "armsvc.exe", "MSIAfterburner.exe", "vmnat.exe",
	"vmware-authd.exe", "vmnetdhcp.exe", "pia_manager.exe", "SpotifyWebHelper.exe", "Dropbox.exe", "Viber.exe", "idaq.exe",
	"idaq64.exe", "CoreSync.exe", "Steam.exe", "SpotifyCrashService.exe", "RzSynapse.exe", "acrotray.exe",
	"CCLibrary.exe", "pia_tray.exe", "rubyw.exe", "netsession_win.exe", "NvBackend.exe", "TeamViewer_Service.exe",
	"DisplayFusionHookAppWIN6032.exe", "DisplayFusionHookAppWIN6064.exe", "GameScannerService.exe", "AdobeUpdateService.exe",
	"steamwebhelper.exe", "c2c_service.exe", "Sync Server.exe", "NvNetworkService.exe", "Creative Cloud.exe", "foobar2000.exe",
	"code.exe"
};

void CMainFrame::OnButtonSelectProcess()
{
	HANDLE hProcess = 0;
	void* pBuffer = NULL;
	ULONG cbBuffer = 0x20000;
	HANDLE hHeap = NULL;
	NTSTATUS Status = STATUS_INFO_LENGTH_MISMATCH;
	bool bHasEnumeratedProcesses = false;
	PSYSTEM_PROCESS_INFORMATION infoP = NULL;

	CMFCRibbonButton* pButton = NULL;
	
	pButton = static_cast<CMFCRibbonButton*>(m_wndRibbonBar.FindByID(ID_BUTTON_SELECTPROCESS));

	CRect pos = pButton->GetRect();
	ClientToScreen(&pos);

	CMenu menu;
	menu.CreatePopupMenu();

	ClearProcMenuItems();

	static HMODULE hNtdll = (HMODULE)Utils::GetLocalModuleHandle("ntdll.dll");
	static tNtQuerySystemInformation fnQSI = (tNtQuerySystemInformation)Utils::GetProcAddress(hNtdll, "NtQuerySystemInformation");

	hHeap = GetProcessHeap();
	Status = STATUS_INFO_LENGTH_MISMATCH;

	while (!bHasEnumeratedProcesses)
	{
		pBuffer = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cbBuffer);
		if (pBuffer == NULL)
			return;

		Status = fnQSI(SystemProcessInformation, pBuffer, cbBuffer, &cbBuffer);
		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			HeapFree(hHeap, NULL, pBuffer);
			cbBuffer *= 2;
		}
		else if (!NT_SUCCESS(Status))
		{
			HeapFree(hHeap, NULL, pBuffer);
			return;
		}
		else
		{
			bHasEnumeratedProcesses = true;
			infoP = (PSYSTEM_PROCESS_INFORMATION)pBuffer;
			while (infoP)
			{	
				if (infoP->ImageName.Length)
				{
					char pName[256];
					memset(pName, 0, sizeof(pName));
					WideCharToMultiByte(0, 0, infoP->ImageName.Buffer, infoP->ImageName.Length, pName, 256, NULL, NULL);
					
					// Are we filtering out processes
					if (gbFilterProcesses)
					{
						bool skip = false;
						for (int i = 0; i < sizeof(CommonProcesses) / sizeof(*CommonProcesses); i++) 
						{
							if ( _stricmp( pName, CommonProcesses[ i ] ) == 0 || (DWORD)infoP->UniqueProcessId == GetCurrentProcessId( ) )
							{
								skip = true;
								break;
							}
						}

						if (skip)
						{
							if (!infoP->NextEntryOffset)
								break;
							infoP = (PSYSTEM_PROCESS_INFORMATION)((unsigned char*)infoP + infoP->NextEntryOffset);
							continue;
						}
					}

					hProcess = ReClassOpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, (DWORD)infoP->UniqueProcessId);
					
					if (hProcess)
					{
						#ifdef _WIN64
						if (Utils::GetProcessPlatform(hProcess) == Utils::ProcessPlatformX64)
						#else
						if (Utils::GetProcessPlatform(hProcess) == Utils::ProcessPlatformX86)
						#endif
						{
							TCHAR filename[1024];
							GetModuleFileNameEx(hProcess, NULL, filename, 1024);

							SHFILEINFO sfi;
							SHGetFileInfo(filename, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

							CBitmap* pBitmap = new CBitmap();
							CProcessMenuInfo Item;
							Item.ProcessId = (DWORD)infoP->UniqueProcessId;
							Item.pBitmap = pBitmap;
							Item.Procname = pName;

							CClientDC clDC(this);
							CDC dc; dc.CreateCompatibleDC(&clDC);

							int size = 16;
							pBitmap->CreateCompatibleBitmap(&clDC, size, size);
							CBitmap* pOldBmp = dc.SelectObject(pBitmap);

							dc.FillSolidRect(0, 0, size, size, GetSysColor(COLOR_3DFACE));
							::DrawIconEx(dc.GetSafeHdc(), 0, 0, sfi.hIcon, size, size, 0, NULL, DI_NORMAL);
							dc.SelectObject(pOldBmp);
							dc.DeleteDC();

							DWORD MsgID = (DWORD)(WM_PROCESSMENU + ProcMenuItems.size());
							
							CString procWithID;
							procWithID.Format(_T("%hs (%i)"), pName, (DWORD)infoP->UniqueProcessId); 

							menu.AppendMenu(MF_STRING | MF_ENABLED, MsgID, procWithID);
							menu.SetMenuItemBitmaps(MsgID, MF_BYCOMMAND, pBitmap, pBitmap);

							ProcMenuItems.push_back(Item);
						}

						CloseHandle(hProcess);
					}
				}

				if (!infoP->NextEntryOffset)
					break;
				infoP = (PSYSTEM_PROCESS_INFORMATION)((unsigned char*)infoP + infoP->NextEntryOffset);
			}
		}
	}

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

void CMainFrame::OnButtonEditClass()
{
	if (gbClassBrowser)
	{	
		CDialogClasses dlg;
		dlg.DoModal();
	}
	else 
	{
		// TODO: instead of menu popup a window
		CMFCRibbonBaseElement* pButton = m_wndRibbonBar.FindByID(ID_BUTTON_EDITCLASS);

		CRect pos = pButton->GetRect();
		ClientToScreen(&pos);

		CMenu menu;
		menu.CreatePopupMenu();

		for (UINT m = 0; m < theApp.Classes.size(); m++)
		{
			CString MenuItem;
			MenuItem.Format(_T("%i - %s"), m, theApp.Classes[m]->Name);
			menu.AppendMenu(MF_STRING | MF_ENABLED, WM_CLASSMENU + m, MenuItem);
		}

		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this);
	}
}

void CMainFrame::OnUpdateButtonEditClass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}

void CMainFrame::OnButtonDeleteClass()
{
	CMFCRibbonBaseElement* pButton = m_wndRibbonBar.FindByID(ID_BUTTON_DELETECLASS);

	CRect pos = pButton->GetRect();
	ClientToScreen(&pos);

	CMenu menu;
	menu.CreatePopupMenu();
	for (UINT m = 0; m < theApp.Classes.size(); m++)
	{
		menu.AppendMenu(MF_STRING | MF_ENABLED, WM_DELETECLASSMENU + m, theApp.Classes[m]->Name);
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this);
}

void CMainFrame::OnUpdateButtonDeleteClass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((theApp.Classes.size() > 0));
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_MEMORYMAP_UPDATE)
		UpdateMemoryMap();

	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::OnCheckTopmost()
{
	gbTop = !gbTop;

	if (gbTop)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	else
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
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

void CMainFrame::OnCheckFilterProcesses()
{
	gbFilterProcesses = !gbFilterProcesses;
}

void CMainFrame::OnUpdateCheckFilterProcesses(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(gbFilterProcesses);
}

void CMainFrame::OnCheckPrivatePadding()
{
	gbPrivatePadding = !gbPrivatePadding;
}

void CMainFrame::OnUpdateCheckPrivatePadding(CCmdUI *pCmdUI)
{ 
	pCmdUI->SetCheck(gbPrivatePadding);
}

void CMainFrame::OnCheckClipboardCopy()
{
	gbClipboardCopy = !gbClipboardCopy;
}

void CMainFrame::OnUpdateCheckClipboardCopy(CCmdUI *pCmdUI)
{ 
	pCmdUI->SetCheck(gbClipboardCopy);
}

// Multi monitor support. Thanks timboy67678
void CMainFrame::OnButtonLeft()
{
	RECT rc; HMONITOR hMon;
	MONITORINFO mi = { sizeof(MONITORINFO) };
	::GetWindowRect(GetSafeHwnd(), &rc);
	hMon = ::MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
	::GetMonitorInfo(hMon, &mi);
	LONG nWidth = mi.rcWork.right - mi.rcWork.left, nHeight = mi.rcWork.bottom - mi.rcWork.top;
	SetWindowPos(gbTop ? &wndTopMost : &wndNoTopMost, mi.rcMonitor.left, mi.rcMonitor.top, nWidth / 2, nHeight, SWP_NOZORDER);
}

void CMainFrame::OnButtonRight()
{
	RECT rc; HMONITOR hMon;
	MONITORINFO mi = { sizeof(MONITORINFO) };
	::GetWindowRect(GetSafeHwnd(), &rc);
	hMon = ::MonitorFromRect(&rc, MONITOR_DEFAULTTONEAREST);
	::GetMonitorInfo(hMon, &mi);
	LONG nWidth = mi.rcWork.right - mi.rcWork.left, nHeight = mi.rcWork.bottom - mi.rcWork.top;
	SetWindowPos(gbTop ? &wndTopMost : &wndNoTopMost, mi.rcMonitor.left + (nWidth / 2), 0, nWidth / 2, nHeight, SWP_NOZORDER);
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
