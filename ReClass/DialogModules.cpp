#include "stdafx.h"

#include "DialogModules.h"
#include "DialogProgress.h"

#include "MainFrame.h"
#include "ChildFrame.h"
#include "SDK.h"

#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogModules, CDialogEx)

CDialogModules::CDialogModules(CWnd* pParent)
	: CDialogEx(CDialogModules::IDD, pParent),
	m_bSortAscendingName(false),
	m_bSortAscendingStart(false),
	m_bSortAscendingEnd(false),
	m_bSortAscendingSize(false)
{}

CDialogModules::~CDialogModules()
{}

void CDialogModules::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULELIST, m_ModuleList);
	DDX_Control(pDX, IDC_MODULENAME, m_Edit);
	DDX_Control(pDX, IDC_MODULES_DEBUG_LOAD, m_SymbolLoad);
}

void CDialogModules::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}

void CDialogModules::OnGetMinMaxInfo(MINMAXINFO *lpinfo)
{
	if (!m_OriginalSize.IsRectNull())
	{
		lpinfo->ptMinTrackSize.x = m_OriginalSize.Width();
		lpinfo->ptMinTrackSize.y = m_OriginalSize.Height();
	}
	CDialogEx::OnGetMinMaxInfo(lpinfo);
}

void CDialogModules::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	CDialogEx::OnContextMenu(pWnd, pos);
}

BEGIN_MESSAGE_MAP(CDialogModules, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_MODULELIST, CDialogModules::OnDblClkListControl)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_MODULELIST, CDialogModules::OnColumnClick)
	ON_EN_CHANGE(IDC_MODULENAME, &CDialogModules::OnEnChangeModuleName)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CDialogModules::BuildList()
{
	for (UINT idx = 0; idx < MemMapModule.size(); idx++)
	{
		MemMapInfo moduleInfo = MemMapModule[idx];

		SHFILEINFO sfi = { 0 };
		SHGetFileInfo(moduleInfo.Path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
		m_ModuleIcons.Add(sfi.hIcon);

		CString uppercase_name = CString(moduleInfo.Name).MakeUpper();
		if (m_Filter.GetLength() != 0 && uppercase_name.Find(m_Filter.MakeUpper()) == -1)
			continue;

		TCHAR strStart[64] = { 0 };
		_stprintf_s(strStart, _T("0x%IX"), moduleInfo.Start);
		TCHAR strEnd[64] = { 0 };
		_stprintf_s(strEnd, _T("0x%IX"), moduleInfo.End);
		TCHAR strSize[64] = { 0 };
		_stprintf_s(strSize, _T("0x%X"), moduleInfo.Size);

		AddData(
			idx,
			moduleInfo.Name.GetString(),
			moduleInfo.Path.GetString(), 
			const_cast<LPCTSTR>(strStart), 
			const_cast<LPCTSTR>(strEnd),
			const_cast<LPCTSTR>(strSize), 
			static_cast<LPARAM>(moduleInfo.Start)
		);
	}
}

BOOL CDialogModules::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(NULL, TRUE);
	SetIcon(NULL, FALSE);

	GetWindowRect(&m_OriginalSize);
	ScreenToClient(&m_OriginalSize);

	m_SymbolLoad.EnableWindow(gbSymbolResolution ? TRUE : FALSE);
	m_SymbolLoad.SetCheck(gbLoadModuleSymbol ? BST_CHECKED : BST_UNCHECKED);

	m_ModuleIcons.Create(15, 15, ILC_COLOR32, 1, 1);
	m_ModuleIcons.SetBkColor(RGB(255, 255, 255));

	m_ModuleList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_ModuleList.InsertColumn(COLUMN_NAME, _T("Module"), LVCFMT_LEFT, 140);
	m_ModuleList.InsertColumn(COLUMN_PATH, _T("Path"), LVCFMT_LEFT, 200);
	m_ModuleList.InsertColumn(COLUMN_START, _T("Start"), LVCFMT_LEFT, 100);
	m_ModuleList.InsertColumn(COLUMN_END, _T("End"), LVCFMT_LEFT, 100);
	m_ModuleList.InsertColumn(COLUMN_SIZE, _T("Size"), LVCFMT_LEFT, 80);

	m_ModuleList.SetImageList(&m_ModuleIcons, LVSIL_SMALL);

	BuildList();

	return TRUE;
}

inline int CDialogModules::FindModuleByName(const TCHAR* szName)
{
	for (UINT id = 0; id < MemMapModule.size(); id++)
	{
		MemMapInfo moduleInfo = MemMapModule[id];
		if (_tcsicmp(moduleInfo.Name, szName) == 0)
			return id;
	}
	return -1;
}

__inline CNodeClass* CDialogModules::GetClassByName(const TCHAR* szClassName)
{
	auto iter = std::find_if(theApp.Classes.begin(), theApp.Classes.end(), [szClassName](const CNodeClass* value) -> bool { return (value->GetName().CompareNoCase(szClassName) == 0); });
	if (iter != theApp.Classes.end())
		return *iter;
	else
		return nullptr;
}

void CDialogModules::SetSelected()
{
	POSITION pos = m_ModuleList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ModuleList.GetNextSelectedItem(pos);
		nItem = FindModuleByName(m_ModuleList.GetItemText(nItem, 0));

		MemMapInfo mod = MemMapModule[nItem];

		if (gbSymbolResolution && m_SymbolLoad.GetCheck() == BST_CHECKED)
			g_SymLoader->LoadSymbolsForModule(mod.Path, mod.Start, mod.Size);

		int extension_size = mod.Name.ReverseFind('.');
		if (extension_size == -1)
			extension_size = 0;
		else extension_size = mod.Name.GetLength() - extension_size;

		CString ClassName = mod.Name.Left(mod.Name.GetLength() - extension_size) + _T("_base");

		CNodeClass* pNewClass = GetClassByName(ClassName);

		if (pNewClass != nullptr)
		{
			
			CMDIFrameWnd* pFrame = STATIC_DOWNCAST(CMDIFrameWnd, AfxGetApp()->m_pMainWnd);
			CChildFrame* pChild = pNewClass->pChildWindow;

			// Check if its a window first to dodge the assertion in IsWindowVisible
			if (pChild && IsWindow(pChild->GetSafeHwnd()) && pChild->IsWindowVisible())
			{
				static_cast<CMDIChildWnd*>(pChild)->MDIActivate();
			}
			else 
			{
				CChildFrame* pNewChild = static_cast<CChildFrame*>(pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel));
				pNewChild->m_wndView.m_pClass = pNewClass;
				pNewClass->pChildWindow = pNewChild;
				pNewChild->SetTitle(pNewClass->GetName());
				pNewChild->SetWindowText(pNewClass->GetName());
				pFrame->UpdateFrameTitleForDocument(pNewClass->GetName());
			}
		}
		else 
		{
			CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
			CChildFrame* pChild = static_cast<CChildFrame*>(pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel));

			pNewClass = new CNodeClass;
			pNewClass->SetName(ClassName);

			TCHAR strStart[64];
			_stprintf(strStart, _T("%IX"), mod.Start);
			pNewClass->SetOffsetString(strStart);
			pNewClass->SetOffset(mod.Start);
			pNewClass->pChildWindow = pChild;
			pNewClass->idx = (int)theApp.Classes.size();

			theApp.Classes.push_back(pNewClass);

			DWORD offset = 0;
			for (int i = 0; i < 64 / sizeof(size_t); i++)
			{
				CNodeHex* pNode = new CNodeHex;
				pNode->SetParent(pNewClass);
				pNode->SetOffset(offset);
				offset += pNode->GetMemorySize();
				pNewClass->AddNode(pNode);
			}

			pChild->m_wndView.m_pClass = pNewClass;
			pChild->SetTitle(pNewClass->GetName());
			pChild->SetWindowText(pNewClass->GetName());
			pFrame->UpdateFrameTitleForDocument(pNewClass->GetName());
		}
	}
}

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

			return (int)(num2 - num1);
		}
		else if (column == COLUMN_NAME)
		{
			CString strModuleName1 = pListCtrl->GetItemText(item1, column);
			CString strModuleName2 = pListCtrl->GetItemText(item2, column);

			return _tcsicmp(strModuleName1.GetBuffer(), strModuleName2.GetBuffer());
		}
		else if (column == COLUMN_PATH)
		{
			CString strModulePath1 = pListCtrl->GetItemText(item1, column);
			CString strModulePath2 = pListCtrl->GetItemText(item2, column);

			int idxSlash1 = strModulePath1.ReverseFind(_T('\\'));
			if (!idxSlash1)
				idxSlash1 = strModulePath1.ReverseFind(_T('/'));
			CString strModuleFile1 = strModulePath1.Mid(++idxSlash1);

			int idxSlash2 = strModulePath2.ReverseFind(_T('\\'));
			if (!idxSlash2)
				idxSlash2 = strModulePath2.ReverseFind(_T('/'));
			CString strModuleFile2 = strModulePath2.Mid(++idxSlash2);

			return _tcsicmp(strModuleFile1.GetBuffer(), strModuleFile2.GetBuffer());
		}
	}
	return 0;
}

void CDialogModules::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	LPCOMPARESTRUCT compare = new COMPARESTRUCT;
	compare->pListCtrl = &m_ModuleList;
	compare->iColumn = pNMListView->iSubItem;

	switch (compare->iColumn)
	{
	case COLUMN_NAME:
		m_bSortAscendingName = !m_bSortAscendingName;
		compare->bAscending = m_bSortAscendingName;
		break;
	case COLUMN_PATH:
		m_bSortAscendingPath = !m_bSortAscendingPath;
		compare->bAscending = m_bSortAscendingPath;
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

	m_ModuleList.SortItemsEx(CompareFunction, (LPARAM)compare);

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
	if (m_SymbolLoad.GetCheck() == BST_CHECKED)
		gbSymbolResolution = true;
	CDialogEx::OnOK();
}

int CDialogModules::AddData(int Index, LPCTSTR ModuleName, LPCTSTR ModulePath, LPCTSTR StartAddress, LPCTSTR EndAddress, LPCTSTR ModuleSize, LPARAM lParam)
{
	LVITEM lvi = { 0 };

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.pszText = (LPTSTR)ModuleName;
	lvi.cchTextMax = static_cast<int>(_tcslen(ModuleName)) + 1;
	lvi.iImage = Index;
	lvi.lParam = lParam;
	lvi.iItem = m_ModuleList.GetItemCount();

	int pos = m_ModuleList.InsertItem(&lvi);

	m_ModuleList.SetItemText(pos, COLUMN_PATH, (LPCTSTR)ModulePath);
	m_ModuleList.SetItemText(pos, COLUMN_START, (LPCTSTR)StartAddress);
	m_ModuleList.SetItemText(pos, COLUMN_END, (LPCTSTR)EndAddress);
	m_ModuleList.SetItemText(pos, COLUMN_SIZE, (LPCTSTR)ModuleSize);

	return pos;
}

void CDialogModules::OnEnChangeModuleName()
{
	m_Edit.GetWindowText(m_Filter);
	m_ModuleList.DeleteAllItems();
	BuildList();
}
