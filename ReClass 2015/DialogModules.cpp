#include "stdafx.h"
#include "ReClass2015.h"
#include "DialogModules.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SDK.h"

#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogModules, CDialogEx)

CDialogModules::CDialogModules(CWnd* pParent) : CDialogEx(CDialogModules::IDD, pParent)
{
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

}

BEGIN_MESSAGE_MAP(CDialogModules, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_MODULELIST, OnDblclkListControl)
	ON_EN_CHANGE(IDC_MODULENAME, &CDialogModules::OnEnChangeModuleName)
END_MESSAGE_MAP()


void CDialogModules::BuildList()
{
	for (UINT i = 0; i < MemMapModule.size(); i++)
	{
		MemMapInfo moduleInfo = MemMapModule[i];

		SHFILEINFO    sfi;
		SHGetFileInfo(MemMapModule[i].Path, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
		m_ImageList.Add(sfi.hIcon);

		CString name = moduleInfo.Name;
		if (m_Filter.GetLength() != 0 && name.MakeUpper().Find(m_Filter.MakeUpper()) == -1)
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

	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32, 1, 1);
	m_ImageList.SetBkColor(RGB(255, 255, 255));

	m_ModuleViewList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_ModuleViewList.InsertColumn(COLUMN_MODULE, _T("Module"), LVCFMT_LEFT, 200);
	m_ModuleViewList.InsertColumn(COLUMN_START, _T("Start"), LVCFMT_LEFT, 200);
	m_ModuleViewList.InsertColumn(COLUMN_END, _T("End"), LVCFMT_LEFT, 200);
	m_ModuleViewList.InsertColumn(COLUMN_SIZE, _T("Size"), LVCFMT_LEFT, 200);

	m_ModuleViewList.SetImageList(&m_ImageList, LVSIL_SMALL);

	BuildList();

	return TRUE;
}

__inline int FindModuleByName(const TCHAR* szName)
{
	for (int id = 0; id < MemMapModule.size(); id++)
	{
		MemMapInfo moduleInfo = MemMapModule[id];
		if (_tcsicmp(moduleInfo.Name, szName) == 0)
			return id;
	}
	return -1;
};

__inline CNodeClass* GetClassByName(const TCHAR* szClassName)
{
	CNodeClass* pClass = 0;
	for (unsigned int i = 0; i < theApp.Classes.size(); i++) {
		if (theApp.Classes[i]->Name == szClassName) {
			pClass = theApp.Classes[i];
			break;
		}
	}
	return pClass;
}

void CDialogModules::SetSelected()
{
	unsigned numselected = m_ModuleViewList.GetSelectedCount();
	POSITION pos = m_ModuleViewList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ModuleViewList.GetNextSelectedItem(pos);
		CString szBuffer = m_ModuleViewList.GetItemText(nItem, 0);

		nItem = FindModuleByName(szBuffer.GetBuffer());

		//printf( "szBuffer %s new %d\n", szBuffer.GetBuffer( ), nItem );
		CMainFrame*  pFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
		CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel);
		
		CString ClassName = MemMapModule[nItem].Name.Left(MemMapModule[nItem].Name.GetLength() - 4);
		ClassName += _T("_base");

		CNodeClass* pNewClass = GetClassByName(ClassName);
		if (!pNewClass)
		{
			pNewClass = new CNodeClass;

			
			pNewClass->Name = ClassName;

			TCHAR strStart[64];
			_stprintf(strStart, _T("%IX"), MemMapModule[nItem].Start);
			pNewClass->strOffset = strStart;
			pNewClass->offset = MemMapModule[nItem].Start;

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

void CDialogModules::OnDblclkListControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);
	SetSelected();
	CDialogEx::OnOK();
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

	lvi.pszText = (LPTSTR)ModuleName;
	lvi.cchTextMax = static_cast<int>(_tcslen(ModuleName)) + 1;
	lvi.iImage = Index;
	lvi.lParam = lParam;
	lvi.iItem = ListView_GetItemCount(m_ModuleViewList.GetSafeHwnd());

	int pos = ListView_InsertItem(m_ModuleViewList.GetSafeHwnd(), &lvi);

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
