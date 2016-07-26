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

// standard constructor
CDialogProcSelect::CDialogProcSelect(CWnd* pParent)
	: CDialogEx(CDialogProcSelect::IDD, pParent), 
	m_bLoadingProcesses(false),
	m_bSortAscendingName(false),
	m_bSortAscendingId(false)
{
}

CDialogProcSelect::~CDialogProcSelect()
{
}

IMPLEMENT_DYNAMIC( CDialogProcSelect, CDialogEx )

BEGIN_MESSAGE_MAP( CDialogProcSelect, CDialogEx )
	ON_NOTIFY( NM_DBLCLK, IDC_PROCESS_LIST, CDialogProcSelect::OnDblClkListControl )
	ON_NOTIFY( LVN_COLUMNCLICK, IDC_PROCESS_LIST, CDialogProcSelect::OnColumnClick )
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
				HANDLE hProcess = ReClassOpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)proc_info->UniqueProcessId);

			#ifdef _WIN64
				if ( hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX64 ) {
			#else
				if ( hProcess && Utils::GetProcessPlatform( hProcess ) == Utils::ProcessPlatformX86 ) {
			#endif
					ProcessInfoStack info;
					info.ProcessId = (DWORD) proc_info->UniqueProcessId;

					TCHAR process_path[ MAX_PATH + 1 ] = { 0 };
					GetModuleFileNameEx( hProcess, NULL, process_path, MAX_PATH );
					SHFILEINFO info_shell = { 0 };
					SHGetFileInfo( process_path, NULL, &info_shell, sizeof( SHFILEINFO ), SHGFI_ICON );
					m_ProcessIcons.Add( info_shell.hIcon );

					LVITEM lvi = { 0 };
					lvi.mask = LVIF_TEXT | LVIF_IMAGE;

				#ifdef UNICODE
					info.Procname = proc_info->ImageName.Buffer;
				#else
					info.Procname = CW2A( proc_info->ImageName.Buffer );
				#endif

					lvi.pszText = info.Procname.GetBuffer();
					lvi.cchTextMax = info.Procname.GetLength();
					lvi.iImage = proc_index++;;
					lvi.iItem = m_ProcessList.GetItemCount();
					int pos = m_ProcessList.InsertItem( &lvi );

					TCHAR strProcId[64];
					_stprintf_s(strProcId, _T("%d"), info.ProcessId);
					m_ProcessList.SetItemText(pos, COLUMN_PROCESSID, (LPTSTR)strProcId);

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

	m_ProcessList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_ProcessList.SetImageList( &m_ProcessIcons, LVSIL_SMALL );
	m_ProcessList.InsertColumn(COLUMN_PROCESSNAME, _T( "Process" ), LVCFMT_LEFT, 200 );
	m_ProcessList.InsertColumn(COLUMN_PROCESSID, _T("ID"), LVCFMT_LEFT, 50);
	

	RefreshRunningProcesses( );

	return TRUE;
}

int CALLBACK CDialogProcSelect::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	COMPARESTRUCT* compare = (COMPARESTRUCT*)lParamSort;
	if (compare)
	{
		CListCtrl* pListCtrl = (CListCtrl*)compare->pListCtrl;
		int column = compare->iColumn;
		bool ascending = compare->bAscending;

		int item1 = ascending ? static_cast<int>(lParam1) : static_cast<int>(lParam2);
		int item2 = ascending ? static_cast<int>(lParam2) : static_cast<int>(lParam1);

		if (column == COLUMN_PROCESSID)
		{
			CString strNum1 = pListCtrl->GetItemText(item1, column);
			CString strNum2 = pListCtrl->GetItemText(item2, column);

			size_t num1 = (size_t)_tcstoui64(strNum1.GetBuffer(), NULL, 16);
			size_t num2 = (size_t)_tcstoui64(strNum2.GetBuffer(), NULL, 16);

			return num2 - num1;
		}
		else if (column == COLUMN_PROCESSNAME)
		{
			CString strModuleName1 = pListCtrl->GetItemText(item1, column);
			CString strModuleName2 = pListCtrl->GetItemText(item2, column);

			return _tcsicmp(strModuleName1.GetBuffer(), strModuleName2.GetBuffer());
		}
	}

	return 0;
}

void CDialogProcSelect::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LPCOMPARESTRUCT compare = new COMPARESTRUCT;
	compare->pListCtrl = &m_ProcessList;
	compare->iColumn = pNMListView->iSubItem;

	switch (compare->iColumn)
	{
	case COLUMN_PROCESSNAME:
		m_bSortAscendingName = !m_bSortAscendingName;
		compare->bAscending = m_bSortAscendingName;
		break;
	case COLUMN_PROCESSID:
		m_bSortAscendingId = !m_bSortAscendingId;
		compare->bAscending = m_bSortAscendingId;
		break;
	default:
		compare->bAscending = false;
		break;
	}

	m_ProcessList.SortItemsEx(CompareFunction, (LPARAM)compare);

	delete compare;

	*pResult = 0;
}

void CDialogProcSelect::OnDblClkListControl( NMHDR* pNMHDR, LRESULT* pResult )
{
	OnAttachButton( );
}

void CDialogProcSelect::OnAttachButton( )
{
	int selected_index = m_ProcessList.GetSelectionMark( );
	if ( selected_index == -1 )
		return;
	g_ProcessID = m_ProcessInfos[ selected_index ].ProcessId;
	g_hProcess = ReClassOpenProcess( PROCESS_ALL_ACCESS, FALSE, g_ProcessID );
	g_ProcessName = m_ProcessInfos[ selected_index ].Procname;
	UpdateMemoryMap( );
	EndDialog( 0 );
}

void CDialogProcSelect::OnRefreshButton( )
{
	if ( m_bLoadingProcesses ) 
		return;
	RefreshRunningProcesses( );
}

