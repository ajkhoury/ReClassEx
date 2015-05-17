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

void CDialogClasses::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CLASSLIST, m_ClassView);
	DDX_Control(pDX, IDC_CLASSNAME, m_Edit);
}


BEGIN_MESSAGE_MAP(CDialogClasses, CDialogEx)
	ON_EN_CHANGE( IDC_CLASSNAME, &CDialogClasses::OnEnChangeClassname )
END_MESSAGE_MAP()


void CDialogClasses::BuildList()
{
	CDialogEx::OnInitDialog();

	m_ImageList.Add(m_hClassIcon);
	ListView_SetImageList(m_ClassView.GetSafeHwnd(), m_ImageList.GetSafeHandle(), LVSIL_SMALL);

	for (UINT i = 0; i < theApp.Classes.size( ); i++)
	{
		CString name = theApp.Classes[i]->Name;
		if (m_Filter.GetLength() != 0 && name.MakeUpper().Find(m_Filter.MakeUpper()) == -1 )
			continue;

		AddData(m_ClassView, i, 0, theApp.Classes[i]->Name.GetBuffer());
	}
}

BOOL CDialogClasses::OnInitDialog( )
{
	m_ImageList.Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32, 1, 1);
	m_ImageList.SetBkColor(RGB(255, 255, 255));
	m_hClassIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CLASS));
	m_ImageList.Add(m_hClassIcon);
	
	BuildList();

	return TRUE;
}

__inline int FindClassByName(const char* szName)
{
	for (int id = 0; id < theApp.Classes.size(); id++)
	{
		CNodeClass* pNodeClass = theApp.Classes[id];
		if (!pNodeClass)
			continue;
		if(stricmp(pNodeClass->Name, szName) == 0)
			return id;
	}
	return -1;
};

void CDialogClasses::OnOK()
{
	unsigned numselected = m_ClassView.GetSelectedCount();
	POSITION pos = m_ClassView.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nItem = m_ClassView.GetNextSelectedItem(pos);

		// FUUUUCK pushign this out last second shutup
		CString szBuffer = m_ClassView.GetItemText(nItem, 0);
	
		printf("nitem %d\n", nItem);

		nItem = FindClassByName(szBuffer.GetBuffer());

		//printf( "szBuffer %s new %d\n", szBuffer.GetBuffer( ), nItem );

		CMainFrame*  pFrame = static_cast<CMainFrame*>(AfxGetApp( )->m_pMainWnd);
		CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2015TYPE, theApp.m_hMDIMenu, theApp.m_hMDIAccel);
		pChild->m_wndView.m_pClass = theApp.Classes[nItem];

		// This will get overwritten for each class that is opened
		pChild->SetTitle(theApp.Classes[nItem]->Name);
		pChild->SetWindowTextA(theApp.Classes[nItem]->Name);
		pFrame->UpdateFrameTitleForDocument(theApp.Classes[nItem]->Name);
	}

	CDialogEx::OnOK();
}

void CDialogClasses::AddData(CListCtrl& ctrl, int row, int col, const char* str)
{
	LVITEM lv;

	lv.iItem	= row;
	lv.iSubItem = col;
	lv.pszText	= (LPSTR)str;
	
	if( col == 0 )
	{
		lv.mask   = LVIF_IMAGE | LVIF_TEXT;
		lv.iImage = 0;
		ctrl.InsertItem( &lv );
	} 
	else 
	{
		lv.mask   = LVIF_TEXT;
		ctrl.SetItem( &lv );  
	}
}

void CDialogClasses::OnEnChangeClassname()
{
	m_Edit.GetWindowTextA(m_Filter);
	m_ClassView.DeleteAllItems();
	BuildList();
}
