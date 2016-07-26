#include "stdafx.h"
#include "ReClass2015.h"
#include "DialogModules.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SDK.h"

#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogModules, CDialogEx)

CDialogModules::CDialogModules(CWnd* pParent) 
	: CDialogEx(CDialogModules::IDD, pParent)
{
	m_bSortAscendingName	= false;
	m_bSortAscendingStart	= false;
	m_bSortAscendingEnd		= false;
	m_bSortAscendingSize	= false;
}

CDialogModules::~CDialogModules()
{
}

void CDialogModules::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULELIST, m_ModuleViewList);
	DDX_Control(pDX, IDC_MODULENAME, m_Edit);
}

void CDialogModules::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize( nType, cx, cy );
}

void CDialogModules::OnGetMinMaxInfo( MINMAXINFO *lpinfo )
{
	if ( !m_OriginalSize.IsRectNull( ) )
	{
		lpinfo->ptMinTrackSize.x = m_OriginalSize.Width( );
		lpinfo->ptMinTrackSize.y = m_OriginalSize.Height( );
	}

	CDialogEx::OnGetMinMaxInfo( lpinfo );
}

BEGIN_MESSAGE_MAP(CDialogModules, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_MODULELIST, OnDblClkListControl)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_MODULELIST, OnColumnClick)
	ON_EN_CHANGE(IDC_MODULENAME, &CDialogModules::OnEnChangeModuleName)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDialogModules::BuildList()
{
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		MemMapInfo moduleInfo = MemMapModule[i];

		SHFILEINFO sfi = { 0 };
		SHGetFileInfo(MemMapModule[i].Path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
		m_ImageList.Add(sfi.hIcon);

		CString name = moduleInfo.Name, uppercase_name = CString(moduleInfo.Name).MakeUpper();
		if ( m_Filter.GetLength( ) != 0 && uppercase_name.Find( m_Filter.MakeUpper( ) ) == -1 )
			continue;

		TCHAR strStart[64];
		_stprintf(strStart, _T("0x%IX"), moduleInfo.Start);
		TCHAR strEnd[64];
		_stprintf(strEnd, _T("0x%IX"), moduleInfo.End);
		TCHAR strSize[64];
		_stprintf(strSize, _T("0x%X"), moduleInfo.Size);

		AddData(i, (LPTSTR)name.GetString(), strStart, strEnd, strSize, static_cast<LPARAM>(moduleInfo.Start));
	}
}

BOOL CDialogModules::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetWindowRect( &m_OriginalSize );
	ScreenToClient( &m_OriginalSize );

	m_ImageList.Create(15, 15, ILC_COLOR32, 1, 1);
	m_ImageList.SetBkColor(RGB(255, 255, 255));

	m_ModuleViewList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_ModuleViewList.InsertColumn(COLUMN_MODULE, _T("Module"), LVCFMT_LEFT, 300);
	m_ModuleViewList.InsertColumn(COLUMN_START, _T("Start"), LVCFMT_LEFT, 80);
	m_ModuleViewList.InsertColumn(COLUMN_END, _T("End"), LVCFMT_LEFT, 80);
	m_ModuleViewList.InsertColumn(COLUMN_SIZE, _T("Size"), LVCFMT_LEFT, 80);

	m_ModuleViewList.SetImageList(&m_ImageList, LVSIL_SMALL);

	BuildList();

	return TRUE;
}

__inline int CDialogModules::FindModuleByName(const TCHAR* szName)
{
	for (UINT id = 0; id < MemMapModule.size(); id++)
	{
		MemMapInfo moduleInfo = MemMapModule[id];
		if (_tcsicmp(moduleInfo.Name, szName) == 0)
			return id;
	}
	return -1;
};

__inline CNodeClass* CDialogModules::GetClassByName(const TCHAR* szClassName)
{
	CNodeClass* pClass = 0;
	for (UINT i = 0; i < theApp.Classes.size(); i++) {
		if (theApp.Classes[i]->Name == szClassName) {
			pClass = theApp.Classes[i];
			break;
		}
	}
	return pClass;
}

void CDialogModules::SetSelected()
{
	POSITION pos = m_ModuleViewList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ModuleViewList.GetNextSelectedItem(pos);

		nItem = FindModuleByName(m_ModuleViewList.GetItemText(nItem, 0));

		CMainFrame*  pFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
		CChildFrame* pChild = static_cast<CChildFrame*>(pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel));

		int extension_size = MemMapModule[ nItem ].Name.ReverseFind( '.' );
		if ( extension_size == -1 ) 
			extension_size = 0;
		else extension_size = MemMapModule[ nItem ].Name.GetLength( ) - extension_size;

		CString ClassName = MemMapModule[ nItem ].Name.Left( MemMapModule[ nItem ].Name.GetLength( ) - extension_size ) + _T( "_base" );

		CNodeClass* pNewClass = GetClassByName(ClassName);
		if (!pNewClass)
		{
			pNewClass = new CNodeClass;
			pNewClass->Name = ClassName;

			TCHAR strStart[64];
			_stprintf(strStart, _T("%IX"), MemMapModule[nItem].Start);
			pNewClass->strOffset = strStart;
			pNewClass->offset = MemMapModule[nItem].Start;
			pNewClass->pChildWindow = pChild;
			pNewClass->idx = (int)theApp.Classes.size();

			theApp.Classes.push_back(pNewClass);

			DWORD offset = 0;
			for (int i = 0; i < 64 / sizeof(size_t); i++)
			{
				CNodeHex* pNode = new CNodeHex;
				pNode->pParent = pNewClass;
				pNode->offset = offset;
				offset += pNode->GetMemorySize();
				pNewClass->Nodes.push_back(pNode);
			}
		}

		pChild->m_wndView.m_pClass = pNewClass;

		// This will get overwritten for each module that is selected
		pChild->SetTitle(ClassName);
		pChild->SetWindowText(ClassName);
		pFrame->UpdateFrameTitleForDocument(ClassName);
	}
}

//int (CALLBACK *PFNLVCOMPARE)(LPARAM, LPARAM, LPARAM);
int CALLBACK CDialogModules::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	COMPARESTRUCT* compare = (COMPARESTRUCT*)lParamSort;
	if (compare)
	{
		CListCtrl* pListCtrl = (CListCtrl*)compare->pListCtrl;
		int column = compare->iColumn;
		bool ascending = compare->bAscending;

		int item1 = ascending ? static_cast<int>(lParam1) : static_cast<int>(lParam2);
		int item2 = ascending ? static_cast<int>(lParam2) : static_cast<int>(lParam1);

		if (column == COLUMN_START || column == COLUMN_END || column == COLUMN_SIZE)
		{
			CString strNum1 = pListCtrl->GetItemText(item1, column);
			CString strNum2 = pListCtrl->GetItemText(item2, column);

			size_t num1 = (size_t)_tcstoui64(strNum1.GetBuffer(), NULL, 16);
			size_t num2 = (size_t)_tcstoui64(strNum2.GetBuffer(), NULL, 16);

			return num2 - num1;
		}
		else if (column == COLUMN_MODULE)
		{
			CString strModuleName1 = pListCtrl->GetItemText(item1, column);
			CString strModuleName2 = pListCtrl->GetItemText(item2, column);

			return _tcsicmp(strModuleName1.GetBuffer(), strModuleName2.GetBuffer());
		}
	}

	return 0;
}

void CDialogModules::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LPCOMPARESTRUCT compare = new COMPARESTRUCT;
	compare->pListCtrl = &m_ModuleViewList;
	compare->iColumn = pNMListView->iSubItem;

	switch (compare->iColumn)
	{
	case COLUMN_MODULE:
		m_bSortAscendingName = !m_bSortAscendingName;
		compare->bAscending = m_bSortAscendingName;
		break;
	case COLUMN_START:
		m_bSortAscendingStart = !m_bSortAscendingStart;
		compare->bAscending = m_bSortAscendingStart;
		break;
	case COLUMN_END:
		m_bSortAscendingEnd = !m_bSortAscendingEnd;
		compare->bAscending = m_bSortAscendingEnd;
		break;
	case COLUMN_SIZE:
		m_bSortAscendingSize = !m_bSortAscendingSize;
		compare->bAscending = m_bSortAscendingSize;
		break;
	default:
		compare->bAscending = false;
		break;
	}

	m_ModuleViewList.SortItemsEx(CompareFunction, (LPARAM)compare);

	delete compare;

	*pResult = 0;
}

void CDialogModules::OnDblClkListControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnOK();
}

void CDialogModules::OnOK()
{
	SetSelected();
	CDialogEx::OnOK();
}

int CDialogModules::AddData(int Index, LPTSTR ModuleName, LPTSTR StartAddress, LPTSTR EndAddress, LPTSTR ModuleSize, LPARAM lParam)
{
	LVITEM lvi = { 0 };

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

	lvi.pszText = ModuleName;
	lvi.cchTextMax = static_cast<int>(_tcslen(ModuleName)) + 1;
	lvi.iImage = Index;
	lvi.lParam = lParam;
	lvi.iItem = m_ModuleViewList.GetItemCount();

	int pos = m_ModuleViewList.InsertItem(&lvi);

	m_ModuleViewList.SetItemText(pos, COLUMN_START, (LPTSTR)StartAddress);
	m_ModuleViewList.SetItemText(pos, COLUMN_END, (LPTSTR)EndAddress);
	m_ModuleViewList.SetItemText(pos, COLUMN_SIZE, (LPTSTR)ModuleSize);

	return pos;
}

void CDialogModules::OnEnChangeModuleName()
{
	m_Edit.GetWindowText(m_Filter);
	m_ModuleViewList.DeleteAllItems();
	BuildList();
}
