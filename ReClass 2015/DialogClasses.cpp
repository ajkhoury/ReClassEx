#include "stdafx.h"
#include "ReClass2015.h"
#include "DialogClasses.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SDK.h"

#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDialogClasses, CDialogEx)

CDialogClasses::CDialogClasses(CWnd* pParent) : CDialogEx(CDialogClasses::IDD, pParent)
{
}

CDialogClasses::~CDialogClasses()
{
}

void CDialogClasses::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLASSLIST, m_ClassViewList);
	DDX_Control(pDX, IDC_CLASSNAME, m_Edit);
}

BEGIN_MESSAGE_MAP(CDialogClasses, CDialogEx)
	ON_EN_CHANGE(IDC_CLASSNAME, &CDialogClasses::OnEnChangeClassname)
END_MESSAGE_MAP()


void CDialogClasses::BuildList()
{
	RECT listRect;
	m_ClassViewList.GetWindowRect(&listRect);
	m_ClassViewList.InsertColumn(0, _T("Class"), LVCFMT_CENTER, listRect.right - listRect.left - 4);
	
	m_ImageList.Add(m_hClassIcon);
	ListView_SetImageList(m_ClassViewList.GetSafeHwnd(), m_ImageList.GetSafeHandle(), LVSIL_SMALL);

	for (UINT i = 0; i < theApp.Classes.size(); i++)
	{
		CString name = theApp.Classes[i]->Name;
		if (m_Filter.GetLength() != 0 && name.MakeUpper().Find(m_Filter.MakeUpper()) == -1)
			continue;
		AddData(i, 0, name);
	}
}

BOOL CDialogClasses::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32, 1, 1);
	m_ImageList.SetBkColor(RGB(255, 255, 255));
	m_hClassIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CLASS));
	m_ImageList.Add(m_hClassIcon);

	BuildList();

	return TRUE;
}

__inline int FindClassByName(const TCHAR* szName)
{
	for (int id = 0; id < theApp.Classes.size(); id++)
	{
		CNodeClass* pNodeClass = theApp.Classes[id];
		if (!pNodeClass)
			continue;
		if (_tcsicmp(pNodeClass->Name, szName) == 0)
			return id;
	}
	return -1;
};

void CDialogClasses::OnOK()
{
	unsigned numselected = m_ClassViewList.GetSelectedCount();
	POSITION pos = m_ClassViewList.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ClassViewList.GetNextSelectedItem(pos);
		CString szBuffer = m_ClassViewList.GetItemText(nItem, 0);

		#ifdef _DEBUG
		PrintOut(_T("nitem %d"), nItem);
		#endif

		nItem = FindClassByName(szBuffer.GetBuffer());

		// Thanks timboy67678
		CMainFrame*  pFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
		CChildFrame* pChild = theApp.Classes[nItem]->pChildWindow;

		// Check if its a window first to dodge the assertion in IsWindowVisible
		if (pChild && IsWindow(pChild->GetSafeHwnd()) && pChild->IsWindowVisible())
		{
			static_cast<CMDIChildWnd*>(pChild)->MDIActivate();
		}
		else 
		{
			CChildFrame* pNewChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel);
			pNewChild->m_wndView.m_pClass = theApp.Classes[nItem];
			theApp.Classes[nItem]->pChildWindow = pNewChild;
			pNewChild->SetTitle(theApp.Classes[nItem]->Name);
			pNewChild->SetWindowText(theApp.Classes[nItem]->Name);
			pFrame->UpdateFrameTitleForDocument(theApp.Classes[nItem]->Name);
		}
	}

	CDialogEx::OnOK();
}

void CDialogClasses::AddData(int row, int col, const TCHAR* str)
{
	LVITEM lv;
	ZeroMemory(&lv, sizeof(LVITEM));
	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = (LPTSTR)str;

	if (col == 0)
	{
		lv.mask = LVIF_IMAGE | LVIF_TEXT;
		lv.iImage = 0;
		m_ClassViewList.InsertItem(&lv);
	}
	else
	{
		lv.mask = LVIF_TEXT;
		m_ClassViewList.SetItem(&lv);
	}
}

void CDialogClasses::OnEnChangeClassname()
{
	m_Edit.GetWindowText(m_Filter);
	m_ClassViewList.DeleteAllItems();
	BuildList();
}
