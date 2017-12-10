// DialogProcSelect.cpp : implementation file
//

#include "stdafx.h"

#include <afxdialogex.h>

#include "ReClassEx.h"

#include "DialogProcSelect.h"
#include "DialogProgress.h"

#include "CMainFrame.h"

#include <memory>
#include <algorithm>
#include <Psapi.h>

// CDialogProcSelect dialog

std::initializer_list<const wchar_t*> CommonProcesses =
{
	L"svchost.exe", L"System", L"conhost.exe", L"wininit.exe", L"smss.exe", L"winint.exe", L"wlanext.exe",
	L"spoolsv.exe", L"spoolsv.exe", L"notepad.exe", L"explorer.exe", L"itunes.exe",
	L"sqlservr.exe", L"nvtray.exe",L"nvxdsync.exe", L"lsass.exe", L"jusched.exe",
	L"conhost.exe", L"chrome.exe", L"firefox.exe", L"winamp.exe", L"TrustedInstaller.exe",
	L"WinRAR.exe", L"calc.exe", L"taskhostex.exe", L"Taskmgr.exe",L"dwm.exe",L"SpotifyWebHelper.exe"
	L"plugin-container.exe", L"services.exe", L"devenv.exe", L"flux.exe", L"skype.exe", L"spotify.exe",
	L"csrss.exe", L"taskeng.exe", L"spotifyhelper.exe", L"vcpkgsrv.exe", L"msbuild.exe", L"cmd.exe", L"taskhost.exe",
	L"SettingSyncHost.exe", L"SkyDrive.exe", L"ctfmon.exe", L"RuntimeBroker.exe", L"BTTray.exe", L"BTStackServer.exe",
	L"Bluetooth Headset Helper.exe", L"winlogon.exe", L"PnkBstrA.exe", L"armsvc.exe", L"MSIAfterburner.exe", L"vmnat.exe",
	L"vmware-authd.exe", L"vmnetdhcp.exe", L"pia_manager.exe", L"SpotifyWebHelper.exe", L"Dropbox.exe", L"Viber.exe", L"idaq.exe",
	L"idaq64.exe", L"CoreSync.exe", L"SpotifyCrashService.exe", L"RzSynapse.exe", L"acrotray.exe",
	L"CCLibrary.exe", L"pia_tray.exe", L"rubyw.exe", L"netsession_win.exe", L"NvBackend.exe", L"TeamViewer_Service.exe",
	L"DisplayFusionHookAppWIN6032.exe", L"DisplayFusionHookAppWIN6064.exe", L"GameScannerService.exe", L"AdobeUpdateService.exe",
	L"steamwebhelper.exe", L"c2c_service.exe", L"Sync Server.exe", L"NvNetworkService.exe", L"Creative Cloud.exe", L"foobar2000.exe",
	L"code.exe", L"ReClass.exe", L"ReClass64.exe", L"Discord.exe", L"node.exe", L"TeamViewer.exe", L"Everything.exe"
};

// standard constructor
CDialogProcSelect::CDialogProcSelect( CWnd* pParent )
	: CDialogEx( CDialogProcSelect::IDD, pParent ),
	m_bLoadingProcesses( FALSE ),
	m_bSortAscendingName( FALSE ),
	m_bSortAscendingId( FALSE )
{
}

CDialogProcSelect::~CDialogProcSelect( )
{ }

IMPLEMENT_DYNAMIC( CDialogProcSelect, CDialogEx )

BEGIN_MESSAGE_MAP( CDialogProcSelect, CDialogEx )
	ON_NOTIFY( NM_DBLCLK, IDC_PROCESS_LIST, CDialogProcSelect::OnDblClkListControl )
	ON_NOTIFY( LVN_COLUMNCLICK, IDC_PROCESS_LIST, CDialogProcSelect::OnColumnClick )
	ON_COMMAND( IDC_ATTACH_PROCESS, &CDialogProcSelect::OnAttachButton )
	ON_COMMAND( IDC_REFRESH_PROCESS, &CDialogProcSelect::OnRefreshButton )
	ON_WM_CLOSE( )
END_MESSAGE_MAP( )

void CDialogProcSelect::ListRunningProcs( )
{
	if (m_bLoadingProcesses)
		return;

	m_ProcessIcons.DeleteImageList( );
	m_ProcessIcons.Create( 15, 15, ILC_COLOR32, 1, 1 );
	m_ProcessIcons.SetBkColor( RGB( 255, 255, 255 ) );
	m_ProcessList.SetImageList( &m_ProcessIcons, LVSIL_SMALL );
	m_ProcessList.DeleteAllItems( );
	m_ProcessInfos.clear( );

	PSYSTEM_PROCESS_INFORMATION ProcessInfo = NULL;
	std::unique_ptr<uint8_t[]> BufferArray;
	ULONG BufferSize = 0;
	NTSTATUS status;

	status = ntdll::NtQuerySystemInformation( SystemProcessInformation, NULL, NULL, &BufferSize );
	if (status != STATUS_SUCCESS && status != STATUS_INFO_LENGTH_MISMATCH)
	{
		#ifdef _DEBUG
		PrintOut( _T( "[CDialogProcSelect::RefreshRunningProcesses] Failed to get size for system process list from ProcessBasicInformation" ) );
		#endif
		return;
	}

	BufferArray = std::make_unique<uint8_t[]>( BufferSize + 1 );

	if (NT_SUCCESS( ntdll::NtQuerySystemInformation( SystemProcessInformation, (PVOID)BufferArray.get( ), BufferSize, &BufferSize ) ))
	{
		int CurrentProcessIndex = 0;
		
		m_bLoadingProcesses = TRUE;

		ProcessInfo = (PSYSTEM_PROCESS_INFORMATION)BufferArray.get( );
		while (ProcessInfo)
		{
			if (ProcessInfo->ImageName.Buffer && ProcessInfo->ImageName.Length > 0)
			{
				if (m_FilterCheck.GetCheck( ) != BST_CHECKED || 
					CommonProcesses.end( ) == std::find_if( CommonProcesses.begin( ), CommonProcesses.end( ), 
															[ProcessInfo] ( const wchar_t* proc ) { return _wcsnicmp( proc, ProcessInfo->ImageName.Buffer, ProcessInfo->ImageName.MaximumLength / sizeof(wchar_t) ) == 0; } )
					)
				{
					HANDLE hProcess = ReClassOpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)ProcessInfo->UniqueProcessId );
					#ifdef _WIN64
					if (hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX64)
					#else
					if (hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX86)
					#endif
					{
						ProcessInfoStack Info = { 0 };
						TCHAR tcsProcessId[16] = { 0 };
						TCHAR tcsProcessPath[MAX_PATH] = { 0 };
						SHFILEINFO FileInfo = { 0 };
						LVITEM lvi = { 0 };
						int pos;

						Info.dwProcessId = (DWORD)ProcessInfo->UniqueProcessId;
						#ifdef UNICODE
						Info.strProcessName = ProcessInfo->ImageName.Buffer;
						#else
						Info.strProcessName = CW2A( proc_info->ImageName.Buffer );
						#endif

						GetModuleFileNameEx( hProcess, NULL, tcsProcessPath, MAX_PATH );
						SHGetFileInfo( tcsProcessPath, NULL, &FileInfo, sizeof( SHFILEINFO ), SHGFI_ICON );		
						m_ProcessIcons.Add( FileInfo.hIcon );
	
						lvi.mask = LVIF_TEXT | LVIF_IMAGE;
						lvi.pszText = Info.strProcessName.GetBuffer( );
						lvi.cchTextMax = Info.strProcessName.GetLength( );
						lvi.iImage = CurrentProcessIndex++;
						lvi.iItem = m_ProcessList.GetItemCount( );
						pos = m_ProcessList.InsertItem( &lvi );

						_ui64tot_s( Info.dwProcessId, tcsProcessId, 16, 10 );
						m_ProcessList.SetItemText( pos, COLUMN_PROCESSID, (LPTSTR)tcsProcessId );

						m_ProcessInfos.push_back( Info );
					}
					CloseHandle( hProcess );
				}
			}

			// Make sure not to loop infinitely (Fix for issue where refresh wasnt updating closed applications)
			if (ProcessInfo->NextEntryOffset == 0) 
				break;

			ProcessInfo = (PSYSTEM_PROCESS_INFORMATION)((uint8_t*)ProcessInfo + ProcessInfo->NextEntryOffset);
		}
	}
	m_bLoadingProcesses = FALSE;
}

void CDialogProcSelect::DoDataExchange( CDataExchange* pDX )
{
	DDX_Control( pDX, IDC_PROCESS_LIST, m_ProcessList );
	DDX_Control( pDX, IDC_FILTER_PROCESS_CHECK, m_FilterCheck );
	DDX_Control( pDX, IDC_CHECK_LOADSYMBOLS, m_LoadAllSymbols );
	CDialogEx::DoDataExchange( pDX );
}

BOOL CDialogProcSelect::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );

	if (!g_bSymbolResolution)
		m_LoadAllSymbols.EnableWindow( FALSE );

	GetWindowRect( &m_OriginalSize );
	ScreenToClient( &m_OriginalSize );
	m_ProcessList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER );
	m_ProcessList.InsertColumn( COLUMN_PROCESSNAME, _T( "Process" ), LVCFMT_LEFT, 214 );
	m_ProcessList.InsertColumn( COLUMN_PROCESSID, _T( "ID" ), LVCFMT_LEFT, 45 );
	m_FilterCheck.SetCheck( g_bFilterProcesses ? BST_CHECKED : BST_UNCHECKED );
	m_LoadAllSymbols.SetCheck( g_bLoadModuleSymbol ? BST_CHECKED : BST_UNCHECKED );
	CenterWindow( );
	ListRunningProcs( );
	return TRUE;
}

int CALLBACK CDialogProcSelect::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort )
{
	PCOMPARESTRUCT pCompare = (PCOMPARESTRUCT)lParamSort;
	if (pCompare)
	{
		CListCtrl* pListCtrl = pCompare->pListCtrl;
		INT column = pCompare->iColumn;
		BOOLEAN bAscending = pCompare->bAscending;

		int item1 = bAscending ? static_cast<int>(lParam1) : static_cast<int>(lParam2);
		int item2 = bAscending ? static_cast<int>(lParam2) : static_cast<int>(lParam1);

		if (column == COLUMN_PROCESSID)
		{
			CString strValue1 = pListCtrl->GetItemText( item1, column );
			CString strValue2 = pListCtrl->GetItemText( item2, column );
			DWORD value1 = (DWORD)_tcstoul( strValue1.GetString( ), NULL, 16 );
			DWORD value2 = (DWORD)_tcstoul( strValue2.GetString( ), NULL, 16 );

			return (int)(value2 - value1);
		}
		else if (column == COLUMN_PROCESSNAME)
		{
			CString strModuleName1 = pListCtrl->GetItemText( item1, column );
			CString strModuleName2 = pListCtrl->GetItemText( item2, column );

			return strModuleName1.CompareNoCase( strModuleName2 );
		}
	}
	return 0;
}

void CDialogProcSelect::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	COMPARESTRUCT compare;
	ZeroMemory( &compare, sizeof( COMPARESTRUCT ) );
	compare.pListCtrl = &m_ProcessList;
	compare.iColumn = pNMListView->iSubItem;
	switch (compare.iColumn)
	{
	case COLUMN_PROCESSNAME:
		m_bSortAscendingName = !m_bSortAscendingName;
		compare.bAscending = m_bSortAscendingName;
		break;
	case COLUMN_PROCESSID:
		m_bSortAscendingId = !m_bSortAscendingId;
		compare.bAscending = m_bSortAscendingId;
		break;
	default:
		compare.bAscending = FALSE;
		break;
	}
	m_ProcessList.SortItemsEx( CompareFunction, (LPARAM)&compare );
	*pResult = 0;
}

void CDialogProcSelect::OnDblClkListControl( NMHDR* pNMHDR, LRESULT* pResult )
{
	OnAttachButton( );
}

void CDialogProcSelect::OnAttachButton( )
{
	int SelectedIndex = m_ProcessList.GetSelectionMark( );
	if (SelectedIndex != -1)
	{
		TCHAR tcsdSelectedProcessId[64] = { 0 };
		m_ProcessList.GetItemText( SelectedIndex, COLUMN_PROCESSID, tcsdSelectedProcessId, 64 );
		DWORD dwSelectedProcessId = _tcstoul( tcsdSelectedProcessId, NULL, 10 );

		auto FoundProcessInfo = std::find_if( m_ProcessInfos.begin( ), m_ProcessInfos.end( ),
											 [dwSelectedProcessId] ( const ProcessInfoStack& proc ) -> bool { return proc.dwProcessId == dwSelectedProcessId; } );

		if (FoundProcessInfo != m_ProcessInfos.end( ))
		{
			HANDLE hProcess = ReClassOpenProcess( PROCESS_ALL_ACCESS, FALSE, FoundProcessInfo->dwProcessId );
			if (hProcess == NULL || GetLastError( ) != ERROR_SUCCESS)
			{
				CString message;
				message.Format( _T( "Failed to attach to process \"%s\"!" ), FoundProcessInfo->strProcessName.GetString( ) );
				MessageBox( message, _T( "ReClass 2016" ), MB_OK | MB_ICONERROR );
			}
			else
			{
				if (g_hProcess != NULL) // Stop leaking handles!
					CloseHandle( g_hProcess ); 

				g_hProcess = hProcess;
				g_ProcessID = FoundProcessInfo->dwProcessId;
				g_ProcessName = FoundProcessInfo->strProcessName;

				UpdateMemoryMap( );

				if (g_bSymbolResolution && m_LoadAllSymbols.GetCheck( ) == BST_CHECKED)
				{
					OnClose( );
				
					CStatusBar* pStatusBar = g_ReClassApp.GetStatusBar( );

					CProgressBar progressBar( _T( "Progress" ), 100, 100, TRUE, 0, pStatusBar );
					progressBar.SetStep( 1 );
					progressBar.SetText( _T( "Symbols loading: " ) );

					for (ULONG i = 0; i < g_MemMapModules.size( ); i++)
					{
						TCHAR tcsProgressText[256] = { 0 };
						MemMapInfo CurrentModule = g_MemMapModules[i];

						progressBar.SetRange32( 0, g_MemMapModules.size( ) );

						_stprintf_s( tcsProgressText, _T( "[%d/%d] %s" ), i + 1, g_MemMapModules.size( ), CurrentModule.Name.GetString( ) );
						pStatusBar->SetPaneText( 1, tcsProgressText );

						//MemMapInfo* pCurrentModule = new MemMapInfo( CurrentModule );
						//Utils::NtCreateThread( LoadModuleSymbolsThread, pCurrentModule, 0 );

						if (!g_ReClassApp.m_pSymbolLoader->LoadSymbolsForModule( CurrentModule.Path, CurrentModule.Start, CurrentModule.Size ))
						{
							PrintOut( _T( "Failed to load symbols for %s" ), CurrentModule.Name.GetString( ) );
						}

						progressBar.StepIt( );

						// Peek and pump through messages to stop reclass from hanging
						MSG msg;
						while (::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ))
						{
							if (!AfxGetApp( )->PumpMessage( ))
								::PostQuitMessage( 0 );
						}
					}

					pStatusBar->SetPaneText( 1, _T( "" ) );

					return;
				}

				OnClose( );
			}
		}
	}
}

void CDialogProcSelect::OnRefreshButton( )
{
	ListRunningProcs( );
}

void CDialogProcSelect::OnClose( )
{
	g_bFilterProcesses = (m_FilterCheck.GetCheck( ) == BST_CHECKED);
	g_bLoadModuleSymbol = (m_LoadAllSymbols.GetCheck( ) == BST_CHECKED);
	EndDialog( 0 );
	CDialogEx::OnClose( );
}