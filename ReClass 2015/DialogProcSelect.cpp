// DialogProcSelect.cpp : implementation file
//

#include "stdafx.h"
#include "DialogProcSelect.h"
#include "ReClass2015.h"
#include "afxdialogex.h"

// CDialogProcSelect dialog

// Annoying processes to filter out that you probably won't be looking in the memory of
const std::initializer_list<const wchar_t*> CommonProcesses =
{
	L"svchost.exe", L"conhost.exe", L"wininit.exe", L"smss.exe", L"winint.exe", L"wlanext.exe",
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
	L"code.exe", L"ReClass.exe", L"ReClass64.exe"
};

IMPLEMENT_DYNAMIC( CDialogProcSelect, CDialogEx )

BEGIN_MESSAGE_MAP( CDialogProcSelect, CDialogEx )
	ON_NOTIFY( NM_DBLCLK, IDC_PROCESS_LIST, CDialogProcSelect::OnDblclkListControl )
	ON_COMMAND( IDC_ATTACH_PROCESS, &CDialogProcSelect::OnAttachButton )
	ON_COMMAND( IDC_REFRESH_PROCESS, &CDialogProcSelect::OnRefreshButton )
END_MESSAGE_MAP( )

void CDialogProcSelect::RefreshRunningProcesses( )
{
	if ( m_bLoadingProcesses )
		return;

	m_ProcessList.DeleteAllItems( );

	PSYSTEM_PROCESS_INFORMATION proc_info;
	DWORD buffer_size = 0;

	if ( NT_SUCCESS( ntdll::NtQuerySystemInformation( SystemProcessInformation, NULL, NULL, &buffer_size ) ) )
	{
		PrintOut( _T( "[CDialogProcSelect::RefreshRunningProcesses] Failed to get size for system process list from ProcessBasicInformation" ) );
		return;
	}

	auto buffer_array = std::make_unique<uint8_t[]>( buffer_size + 1 );

	if ( NT_SUCCESS( ntdll::NtQuerySystemInformation( SystemProcessInformation, buffer_array.get( ), buffer_size, &buffer_size ) ) )
	{
		m_bLoadingProcesses = true;

		proc_info = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>( buffer_array.get( ) );

		int proc_index = 0;

		while ( proc_info->NextEntryOffset != 0 )
		{
			if ( proc_info->ImageName.Length
				 && ( !gbFilterProcesses || std::find_if( CommonProcesses.begin( ), CommonProcesses.end( ),
														  [proc_info] ( const wchar_t* iter_val ) -> bool { return _wcsicmp( iter_val, proc_info->ImageName.Buffer ) == 0; } ) == CommonProcesses.end( ) ) )
			{
				HANDLE hProcess = ReClassOpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD) proc_info->UniqueProcessId );

			#ifdef _WIN64
				if ( hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX64 )
			#else
				if ( hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX86 )
			#endif
				{
					ProcessInfoStack info;
					info.ProcessId = (DWORD) proc_info->UniqueProcessId;

					TCHAR process_path[ MAX_PATH + 1 ] = { 0 };
					GetModuleFileNameEx( hProcess, NULL, process_path, MAX_PATH );
					SHFILEINFO info_shell = { 0 };
					SHGetFileInfo( process_path, NULL, &info_shell, sizeof( SHFILEINFO ), SHGFI_ICON );
					m_ProcessIcons.Add( info_shell.hIcon );

					LVITEM lvi = { 0 };
					lvi.mask = LVIF_TEXT | LVIF_IMAGE;

					CString formated_item;
				#ifdef UNICODE
					formated_item.Format( _T( "%s (%d)" ), proc_info->ImageName.Buffer, proc_info->UniqueProcessId );
					info.Procname = proc_info->ImageName.Buffer;
				#else
					formated_item.Format( _T( "%S (%d)" ), proc_info->ImageName.Buffer, proc_info->UniqueProcessId);
					info.Procname = CW2A( proc_info->ImageName.Buffer );
				#endif

					lvi.pszText = formated_item.GetBuffer( );
					lvi.cchTextMax = formated_item.GetLength( );
					lvi.iImage = proc_index++;;
					lvi.iItem = m_ProcessList.GetItemCount( );

					m_ProcessList.InsertItem( &lvi );
					m_ProcessInfos.push_back( info );
				}

				CloseHandle( hProcess );
			}
			proc_info = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>( (ULONG) proc_info + proc_info->NextEntryOffset );
		}
	}
	m_bLoadingProcesses = false;
}

void CDialogProcSelect::DoDataExchange( CDataExchange* pDX )
{
	DDX_Control( pDX, IDC_PROCESS_LIST, m_ProcessList );
	CDialogEx::DoDataExchange( pDX );
}

BOOL CDialogProcSelect::OnInitDialog( )
{
	CDialogEx::OnInitDialog( );

	GetWindowRect( &m_OriginalSize );
	ScreenToClient( &m_OriginalSize );

	m_ProcessIcons.Create( 15, 15, ILC_COLOR32, 1, 1 );
	m_ProcessIcons.SetBkColor( RGB( 255, 255, 255 ) );

	m_ProcessList.SetImageList( &m_ProcessIcons, LVSIL_SMALL );
	m_ProcessList.InsertColumn( 0, _T( "Process" ), LVCFMT_LEFT, 250 );

	RefreshRunningProcesses( );

	return TRUE;
}

void CDialogProcSelect::OnDblclkListControl( NMHDR* pNMHDR, LRESULT* pResult )
{
	OnAttachButton( );
}

void CDialogProcSelect::OnAttachButton( )
{
	int selected_index = m_ProcessList.GetSelectionMark( );
	if ( selected_index == -1 ) return;
	g_ProcessID = m_ProcessInfos[ selected_index ].ProcessId;
	g_hProcess = ReClassOpenProcess( PROCESS_ALL_ACCESS, FALSE, g_ProcessID );
	g_ProcessName = m_ProcessInfos[ selected_index ].Procname;
	UpdateMemoryMap( );
	EndDialog( 0 );
}

void CDialogProcSelect::OnRefreshButton( )
{
	if ( m_bLoadingProcesses ) return;
	RefreshRunningProcesses( );
}

