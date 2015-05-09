#include "stdafx.h"
#include "ReClass2015.h"
#include "ChildFrm.h"
#include "ChildView.h"
#include "DialogEdit.h"

#ifndef NDEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(myCEdit, CEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, &myCEdit::OnEnChange)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

BOOL myCEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_brBackGnd = CreateSolidBrush(crSelect);
	dwStyle = dwStyle | ES_AUTOHSCROLL; // ICY
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

HBRUSH myCEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkColor(crSelect);
	return m_brBackGnd;
}

void myCEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		ShowWindow(SW_HIDE);
		GetWindowText(spot.Text);
		
		CChildView* pChild = (CChildView*)GetParent();
		CNodeBase* c = (CNodeBase*)spot.object;

		DWORD before = c->GetMemorySize();
		c->Update(spot);
		DWORD after = c->GetMemorySize();

		pChild->ResizeNode((CNodeClass*)c->pParent, pChild->FindNodeIndex(c), before, after);
		pChild->Invalidate();
	}

	//UINT limit = this->GetLimitText( );
	printf("text %c %d %d\n", nChar, nRepCnt, nFlags);
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void myCEdit::OnEnChange()
{
	CString text;
	GetWindowText( text );
	int  w = ( text.GetLength( ) + 1 ) * FontWidth; // + 6;

	printf( "change**\n" );
	if (w > MinWidth)
		SetWindowPos(NULL, 0, 0, w, FontHeight, SWP_NOMOVE );
}

BEGIN_MESSAGE_MAP(myCToolTip, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

BOOL myCToolTip::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_brBackGnd = CreateSolidBrush(0xffffff);
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}

HBRUSH myCToolTip::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkColor(0xffffff);
	return m_brBackGnd;
}


// CChildView
CChildView::CChildView()
{
	m_pClass = NULL;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()

	ON_COMMAND(ID_ADD_ADD8,						&CChildView::OnAddAdd8)
	ON_COMMAND(ID_ADD_ADD64,					&CChildView::OnAddAdd64)
	ON_COMMAND(ID_ADD_ADD1024,					&CChildView::OnAddAdd1024)
	ON_COMMAND(ID_TYPE_HEX64,					&CChildView::OnTypeHex64)
	ON_COMMAND(ID_TYPE_HEX32,					&CChildView::OnTypeHex32)
	ON_COMMAND(ID_TYPE_INT64,					&CChildView::OnTypeInt64)
	ON_COMMAND(ID_TYPE_INT32,					&CChildView::OnTypeInt32)
	ON_COMMAND(ID_TYPE_INT16,					&CChildView::OnTypeInt16)
	ON_COMMAND(ID_TYPE_INT8,					&CChildView::OnTypeInt8)
	ON_COMMAND(ID_TYPE_HEX16,					&CChildView::OnTypeHex16)
	ON_COMMAND(ID_TYPE_HEX8,					&CChildView::OnTypeHex8)
	ON_COMMAND(ID_TYPE_DWORD,					&CChildView::OnTypeDword)
	ON_COMMAND(ID_TYPE_WORD,					&CChildView::OnTypeWord)
	ON_COMMAND(ID_TYPE_BYTE,					&CChildView::OnTypeByte)
	ON_COMMAND(ID_TYPE_VEC2,					&CChildView::OnTypeVec2)
	ON_COMMAND(ID_TYPE_VEC3,					&CChildView::OnTypeVec3)
	ON_COMMAND(ID_TYPE_QUAT,					&CChildView::OnTypeQuat)
	ON_COMMAND(ID_TYPE_FLOAT,					&CChildView::OnTypeFloat)
	ON_COMMAND(ID_TYPE_DOUBLE,					&CChildView::OnTypeDouble)
	ON_COMMAND(ID_TYPE_MATRIX,					&CChildView::OnTypeMatrix)
	ON_COMMAND(ID_TYPE_CUSTOM,					&CChildView::OnTypeCustom)
	ON_COMMAND(ID_TYPE_TEXT,					&CChildView::OnTypeText)
	ON_COMMAND(ID_TYPE_PCHAR,					&CChildView::OnTypePChar)
	ON_COMMAND(ID_TYPE_UNICODE,					&CChildView::OnTypeUnicode)
	ON_COMMAND(ID_INSERT_INSERT4,				&CChildView::OnInsertInsert4)
	ON_COMMAND(ID_INSERT_INSERT8,				&CChildView::OnInsertInsert8)
	ON_COMMAND(ID_INSERT_INSERT64,				&CChildView::OnInsertInsert64)
	ON_COMMAND(ID_INSERT_INSERT1024,			&CChildView::OnInsertInsert1024)
	ON_COMMAND(ID_INSERT_INSERT2048,			&CChildView::OnInsertInsert2048)
	ON_COMMAND(ID_TYPE_VTABLE,					&CChildView::OnTypeVtable)
	ON_COMMAND(ID_TYPE_FUNCTION,				&CChildView::OnTypeFunction)
	ON_COMMAND(ID_TYPE_POINTER,					&CChildView::OnTypePointer)
	ON_COMMAND(ID_TYPE_ARRAY,					&CChildView::OnTypeArray)
	ON_COMMAND(ID_TYPE_CLASS,					&CChildView::OnTypeClass)
	ON_COMMAND(ID_MODIFY_DELETE,				&CChildView::OnModifyDelete)
	ON_COMMAND(ID_MODIFY_SHOW,					&CChildView::OnModifyShow)
	ON_COMMAND(ID_MODIFY_HIDE,					&CChildView::OnModifyHide)

	ON_UPDATE_COMMAND_UI(ID_ADD_ADD8,			&CChildView::OnUpdateAddAdd8)
	ON_UPDATE_COMMAND_UI(ID_ADD_ADD64,			&CChildView::OnUpdateAddAdd64)
	ON_UPDATE_COMMAND_UI(ID_ADD_ADD1024,		&CChildView::OnUpdateAddAdd1024)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INSERT4,		&CChildView::OnUpdateInsertInsert4)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INSERT8,		&CChildView::OnUpdateInsertInsert8)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INSERT64,	&CChildView::OnUpdateInsertInsert64)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INSERT1024,	&CChildView::OnUpdateInsertInsert1024)
	ON_UPDATE_COMMAND_UI(ID_INSERT_INSERT2048,	&CChildView::OnUpdateInsertInsert2048)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_DELETE,		&CChildView::OnUpdateModifyDelete)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_SHOW,		&CChildView::OnUpdateModifyShow)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_HIDE,		&CChildView::OnUpdateModifyHide)
	ON_UPDATE_COMMAND_UI(ID_TYPE_HEX64,			&CChildView::OnUpdateTypeHex64)
	ON_UPDATE_COMMAND_UI(ID_TYPE_HEX32,			&CChildView::OnUpdateTypeHex32)
	ON_UPDATE_COMMAND_UI(ID_TYPE_HEX16,			&CChildView::OnUpdateTypeHex16)
	ON_UPDATE_COMMAND_UI(ID_TYPE_HEX8,			&CChildView::OnUpdateTypeHex8)
	ON_UPDATE_COMMAND_UI(ID_TYPE_INT64,			&CChildView::OnUpdateTypeInt64)
	ON_UPDATE_COMMAND_UI(ID_TYPE_INT32,			&CChildView::OnUpdateTypeInt32)
	ON_UPDATE_COMMAND_UI(ID_TYPE_INT16,			&CChildView::OnUpdateTypeInt16)
	ON_UPDATE_COMMAND_UI(ID_TYPE_INT8,			&CChildView::OnUpdateTypeInt8)
	ON_UPDATE_COMMAND_UI(ID_TYPE_DWORD,			&CChildView::OnUpdateTypeDword)
	ON_UPDATE_COMMAND_UI(ID_TYPE_WORD,			&CChildView::OnUpdateTypeWord)
	ON_UPDATE_COMMAND_UI(ID_TYPE_BYTE,			&CChildView::OnUpdateTypeByte)
	ON_UPDATE_COMMAND_UI(ID_TYPE_TEXT,			&CChildView::OnUpdateTypeText)
	ON_UPDATE_COMMAND_UI(ID_TYPE_PCHAR,			&CChildView::OnUpdateTypePChar)
	ON_UPDATE_COMMAND_UI(ID_TYPE_UNICODE,		&CChildView::OnUpdateTypeUnicode)
	ON_UPDATE_COMMAND_UI(ID_TYPE_DOUBLE,		&CChildView::OnUpdateTypeDouble)
	ON_UPDATE_COMMAND_UI(ID_TYPE_FLOAT,			&CChildView::OnUpdateTypeFloat)
	ON_UPDATE_COMMAND_UI(ID_TYPE_CUSTOM,		&CChildView::OnUpdateTypeCustom)
	ON_UPDATE_COMMAND_UI(ID_TYPE_VEC2,			&CChildView::OnUpdateTypeVec2)
	ON_UPDATE_COMMAND_UI(ID_TYPE_VEC3,			&CChildView::OnUpdateTypeVec3)
	ON_UPDATE_COMMAND_UI(ID_TYPE_QUAT,			&CChildView::OnUpdateTypeQuat)
	ON_UPDATE_COMMAND_UI(ID_TYPE_MATRIX,		&CChildView::OnUpdateTypeMatrix)
	ON_UPDATE_COMMAND_UI(ID_TYPE_ARRAY,			&CChildView::OnUpdateTypeArray)
	ON_UPDATE_COMMAND_UI(ID_TYPE_CLASS,			&CChildView::OnUpdateTypeClass)
	ON_UPDATE_COMMAND_UI(ID_TYPE_VTABLE,		&CChildView::OnUpdateTypeVtable)
	ON_UPDATE_COMMAND_UI(ID_TYPE_FUNCTION,		&CChildView::OnUpdateTypeFunction)
	ON_UPDATE_COMMAND_UI(ID_TYPE_POINTER,		&CChildView::OnUpdateTypePointer)

	ON_COMMAND(ID_BUTTON_EDITCODE,				&CChildView::OnButtonEditcode)
	ON_COMMAND(ID_EDIT_COPY,					&CChildView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE,					&CChildView::OnEditPaste)

	ON_WM_MOUSELEAVE()

	ON_COMMAND(ID_BUTTON_ZERO,					&CChildView::OnButtonZero)
	ON_COMMAND(ID_BUTTON_ONE,					&CChildView::OnButtonOne)
	ON_COMMAND(ID_BUTTON_RANDOM,				&CChildView::OnButtonRandom)

	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZERO,		&CChildView::OnUpdateButtonZero)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ONE,			&CChildView::OnUpdateButtonOne)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RANDOM,		&CChildView::OnUpdateButtonRandom)
END_MESSAGE_MAP()

#define SB_WIDTH 14

// CChildView message handlers
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOWFRAME), NULL);
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0, 0, 100, 100);
	m_Edit.CreateEx(WS_EX_WINDOWEDGE, _T("EDIT"),  _T(" "), WS_CHILD | WS_TABSTOP, rect, this, 1);
	//m_Edit.Create(WS_CHILD | WS_TABSTOP, rect, this, 1);
	m_Edit.ShowWindow(SW_HIDE);
	m_Edit.SetFont(&Font);

	m_Scroll.Create(SBS_VERT, rect, this, 0);
	m_Scroll.ShowScrollBar();

	m_ToolTip.Create(ES_MULTILINE | WS_BORDER, rect, this, 1);
	m_ToolTip.SetFont(&Font);
	m_ToolTip.EnableWindow(FALSE);

	SetTimer(1, 250, NULL);

	return 0;
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
		Invalidate(FALSE);
	CWnd::OnTimer(nIDEvent);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_DOWN)
	{
		if (Selected.size() > 0)
		{
			HotSpot* firstSelected = &Selected[0];
			if (firstSelected->Address != HotSpots[HotSpots.size() - 1].Address)
			{
				theApp.ClearSelection();
				Selected.clear();
				for (UINT i = 0; i < HotSpots.size(); i++)
				{
					if (HotSpots[i].Type != HS_SELECT)
						continue;
					if (firstSelected->Address != HotSpots[i].Address)
						continue;

					#if _WIN64
					__int64 nextAddress = HotSpots[i].Address + firstSelected->object->GetMemorySize();
					#else
					int nextAddress = HotSpots[i].Address + firstSelected->object->GetMemorySize();
					#endif				

					UINT newIndex = 0;
					for (int j = 0; HotSpots[i + j].Address != nextAddress; j++)
						newIndex = i + j + 1;

					HotSpots[newIndex].object->bSelected = true;
					Selected.push_back(HotSpots[newIndex]);
						
					break;
				}
			}
		}
	}
	else if(nChar == VK_UP)
	{
		if (Selected.size() > 0)
		{
			HotSpot* firstSelected = &Selected[0];
			if (firstSelected->Address != HotSpots[0].Address)
			{
				theApp.ClearSelection();
				Selected.clear();

				for (UINT i = 0; i < HotSpots.size(); i++)
				{
					if (HotSpots[i].Type != HS_SELECT)
						continue;
					if (firstSelected->Address != HotSpots[i].Address)
						continue;

					HotSpots[i - 1].object->bSelected = true;
					Selected.push_back(HotSpots[i - 1]);

					break;
				}
			}
		}
	}
	else if(nChar == VK_DELETE)
	{
		isDeleting = true; // Ghetto fix to stop crashing from OnMouseHover
		for (UINT i = 0; i < Selected.size(); i++)
		{
			CNodeClass* pClass = (CNodeClass*)Selected[i].object->pParent;
			UINT idx = FindNodeIndex(Selected[i].object);
			if (idx != MAX_NODES)
			{
				delete pClass->Nodes[idx];
				pClass->Nodes.erase(pClass->Nodes.begin() + idx);
				theApp.CalcAllOffsets();
			}
		}
		Selected.clear();
		isDeleting = false;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk( nFlags, point );

	for ( UINT i = 0; i < HotSpots.size();i++ )
	{
		if ( HotSpots[i].Rect.PtInRect(point) )
		{
			if (HotSpots[i].Type == HS_EDIT)
			{
				// Sets the edit "window" to where to cursor was editing at
				m_Edit.SetWindowPos(NULL, HotSpots[i].Rect.left, HotSpots[i].Rect.top, HotSpots[i].Rect.Width(), HotSpots[i].Rect.Height(), 0);
				m_Edit.spot = HotSpots[i];
				m_Edit.MinWidth = m_Edit.spot.Rect.Width();// + 10;
				m_Edit.SetWindowTextA( HotSpots[i].Text);
				m_Edit.ShowWindow(SW_NORMAL);
				m_Edit.SetMargins(0, 99999);
				m_Edit.ShowScrollBar(0, true);
				m_Edit.SetFocus();

				//printf( "Create\n" );

				// m_Edit.SetWindowTextA( "7F72E270000" );

				//HotSpots[i].Rect.InflateRect( 100, 100 );
				
				// commented out
				//m_Edit.CreateSolidCaret( FontWidth, FontHeight );
				//m_Edit.ShowCaret( );
				
				m_Edit.LimitText(99999);

				m_Edit.SetSel(0, 2048);
				//m_Edit.SetSel( 0, 1024 );
				return;
			}

			if ((HotSpots[i].Type == HS_CHANGE_A) || (HotSpots[i].Type == HS_CHANGE_X))
			{
				ExchangeTarget = HotSpots[i];
				CRect pos = ExchangeTarget.Rect;
				ClientToScreen(&pos);

				CNodeBase* pNode = (CNodeBase*)HotSpots[i].object;

				CMenu menu;
				menu.CreatePopupMenu();
				for (UINT m = 0; m < theApp.Classes.size(); m++)
				{
					if ((HotSpots[i].Type == HS_CHANGE_X) && (pNode->pParent == theApp.Classes[m]))
						continue;
					menu.AppendMenu( MF_STRING | MF_ENABLED, WM_CHANGECLASSMENU + m, theApp.Classes[m]->Name );
				}
				menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, pos.left, pos.bottom, this);
			}
		}
	}
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Edit.ShowWindow(SW_HIDE);
	for (UINT i = 0; i < HotSpots.size(); i++)
	{
		if (HotSpots[i].Rect.PtInRect(point))
		{
			CNodeBase* pHitObject = (CNodeBase*)HotSpots[i].object;

			if (HotSpots[i].Type == HS_OPENCLOSE)
				pHitObject->bOpen[HotSpots[i].Level] = !pHitObject->bOpen[HotSpots[i].Level];
			else if (HotSpots[i].Type == HS_CLICK)  
				pHitObject->Update(HotSpots[i]);
			else if (HotSpots[i].Type == HS_SELECT)
			{
				if (nFlags == MK_LBUTTON)
				{
					theApp.ClearSelection();
					Selected.clear();
					pHitObject->bSelected = true;
					Selected.push_back(HotSpots[i]);
				}
				if (nFlags == (MK_LBUTTON | MK_CONTROL))
				{
					pHitObject->bSelected = !pHitObject->bSelected;
					if (pHitObject->bSelected)
					{
						Selected.push_back(HotSpots[i]);
					}
					else
					{
						for (UINT s = 0; s < Selected.size(); s++)
						{
							if (Selected[s].object == pHitObject)
							{
								Selected.erase(Selected.begin()+s);
								break;
							}
						}
					}
				}
				if (nFlags == (MK_LBUTTON | MK_SHIFT))
				{
					if (Selected.size() > 0)
					{
						CNodeBase* pSelected = Selected[0].object;
						if (pSelected->pParent != pHitObject->pParent)
							continue;
						CNodeClass* pClass = (CNodeClass*)pSelected->pParent;
						//if (pClass->GetType() != nt_class) continue;

						UINT idx1 = FindNodeIndex(pSelected);
						if (idx1 == MAX_NODES)
							continue; 
						UINT idx2 = FindNodeIndex(pHitObject);
						if (idx2 == MAX_NODES)
							continue;
						if (idx2 < idx1)
						{
							UINT idxTemp = idx1;
							idx1 = idx2;
							idx2 = idxTemp;
						}

						theApp.ClearSelection();
						Selected.clear();
						for (UINT s=idx1; s <= idx2; s++)
						{
							pClass->Nodes[s]->bSelected = true;
							HotSpot spot;
							//ZeroMemory(&spot,sizeof(HotSpot));
							spot.Address = pClass->offset + pClass->Nodes[s]->offset;
							spot.object = pClass->Nodes[s];
							Selected.push_back(spot);
						}
					}
				}
			}
			else if (HotSpots[i].Type == HS_DROP)
			{
				CRect client;
				GetClientRect(&client);
				ClientToScreen(&client);
				CMenu menu;
				menu.LoadMenuA(MAKEINTRESOURCE(IDR_MENU_QUICKMODIFY));
				menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, client.left + HotSpots[i].Rect.left, client.top + HotSpots[i].Rect.bottom, this);
			}
			else if (HotSpots[i].Type == HS_DELETE)
			{
				isDeleting = true; // Ghetto fix to stop crashing from OnMouseHover
				for (UINT i = 0; i < Selected.size(); i++)
				{
					CNodeClass* pClass = (CNodeClass*)Selected[i].object->pParent;
					UINT idx = FindNodeIndex(Selected[i].object);
					if (idx != MAX_NODES)
					{
						delete pClass->Nodes[idx];
						pClass->Nodes.erase(pClass->Nodes.begin() + idx);
						theApp.CalcAllOffsets();
					}
				}
				Selected.clear();
				isDeleting = false;
			}
			Invalidate();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{	
	if (Selected.size() > 0)
	{
		for (UINT i = 0; i < HotSpots.size(); i++)
		{
			if (HotSpots[i].Rect.PtInRect(point))
			{
				if (HotSpots[i].Type == HS_SELECT)
				{
					CRect client;
					GetClientRect(&client);
					ClientToScreen(&client);

					CMenu menu;
					menu.LoadMenuA(MAKEINTRESOURCE(IDR_MENU_QUICKMODIFY));
					menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_HORNEGANIMATION, client.left + HotSpots[i].Rect.left + point.x, client.top + point.y, this);
				}
			}
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnPaint() 
{
	CRect pos(0,0,0,0);
	HotSpots.clear();

	CPaintDC pdc(this); // device context for painting
	CMemDC m(pdc,this);
	CDC& dc = m.GetDC();
	
	CRect client;
	GetClientRect(&client);

	dc.FillSolidRect(&client,crBackground);
	if (!m_pClass) return;

	dc.SelectObject(&Font);

	HotSpots.clear();

	DWORD classSize = m_pClass->GetMemorySize();
	Memory.SetSize(classSize);
	ReadMemory(m_pClass->offset,Memory.pMemory,classSize);

	ViewInfo View;
	//printf( "Setting address: %p\n", m_pClass->offset );
	View.Address		= m_pClass->offset;
	View.pData			= Memory.pMemory;
	View.Classes		= &theApp.Classes;
	View.client			= &client;
	View.dc				= &dc;
	View.Level			= 0;
	View.HotSpots		= &HotSpots;
	View.bMultiSelected = (Selected.size()>1)?true:false;

	int ypos = m_Scroll.GetScrollPos() * FontHeight;
	if (m_Scroll.IsWindowVisible()) View.client->right -= SB_WIDTH;

	int DrawMax = m_pClass->Draw(View, 0, -ypos) + ypos;
	
	if (m_pClass->RequestPosition != -1)
	{
		if ((m_pClass->RequestPosition >= 0) && ( ( unsigned int ) m_pClass->RequestPosition < theApp.Classes.size()) )
		{
			int idx = -1;
			for (UINT i=0; i<theApp.Classes.size();i++)
			{
				CNodeClass* pClass = View.Classes->at(i);
				if (m_pClass == theApp.Classes[i])
					idx = i;
			}
			theApp.Classes.erase(theApp.Classes.begin()+idx);
			theApp.Classes.insert(theApp.Classes.begin()+m_pClass->RequestPosition,m_pClass);
		}
		m_pClass->RequestPosition = -1;
	}
	if (client.Height() < DrawMax)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = DrawMax/FontHeight;
		si.nPage = client.Height()/FontHeight;
		m_Scroll.SetScrollInfo(&si);
		m_Scroll.ShowScrollBar(1);
	}
	else
	{
		m_Scroll.SetScrollPos(0);
		m_Scroll.ShowScrollBar(0);
	}


	// this makes tabs
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pFrame->GetActiveFrame();
	if (pChild->m_wndView.m_hWnd == this->m_hWnd)
	{
		pChild->SetWindowText(m_pClass->Name);
		pChild->SetTitle( m_pClass->Name ); 
		pFrame->UpdateFrameTitleForDocument(m_pClass->Name);

		//char txt[256];
		//sprintf (txt,"Total HotSpots: %i",HotSpots.size());

		//dc.SetTextColor(0xFF0000);
		//dc.SetBkColor(0x000000);
		//dc.SetBkMode(OPAQUE);
		//dc.DrawText(txt,-1,&CRect(0,0,0,0), DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	}

	//for (UINT i=0; i < HotSpots.size();i++)
	//{
	//	dc.DrawFocusRect(HotSpots[i].Rect);
	//}
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	CRect client;
	GetClientRect(&client);
	m_Scroll.SetWindowPos(NULL,client.right-SB_WIDTH,0,SB_WIDTH,client.Height(),SWP_NOZORDER);
	m_Edit.ShowWindow(SW_HIDE);
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Edit.ShowWindow(SW_HIDE);
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		m_Scroll.SetScrollPos(nPos);
		Invalidate();
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_Scroll.IsWindowVisible())
	{
		m_Scroll.SetScrollPos(m_Scroll.GetScrollPos() - zDelta/FontHeight);
		m_Edit.ShowWindow(SW_HIDE);
		m_ToolTip.ShowWindow(SW_HIDE);
		Invalidate();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

DISASM MyDisasm;
CString DisassembleCode(char *StartCodeSection, char *EndCodeSection, int Virtual_Address)
{
	CString d,t;
	(void) memset (&MyDisasm, 0, sizeof(DISASM));
	MyDisasm.EIP = (int) StartCodeSection;
	MyDisasm.VirtualAddr = (long long) Virtual_Address;
	MyDisasm.Archi = 0;
	MyDisasm.Options = PrefixedNumeral;

	bool Error = 0;
	while (!Error)
	{
		MyDisasm.SecurityBlock = (int)EndCodeSection - MyDisasm.EIP;

		int len = Disasm(&MyDisasm);
		if (len == OUT_OF_BLOCK) Error = 1;
		else if (len == UNKNOWN_OPCODE)Error = 1;
		else 
		{
			t.Format("%p %s\r\n", MyDisasm.VirtualAddr, &MyDisasm. CompleteInstr);
			d += t;
			MyDisasm.EIP = MyDisasm.EIP + len;
			MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
			if (MyDisasm.EIP >= (UIntPtr) EndCodeSection)Error = 1;
		}
	};
	return d;
}

bool bTracking = false;
CPoint HoverPoint;
void CChildView::OnMouseHover(UINT nFlags, CPoint point)
{
	CString msg;
	if (Selected.size() > 1)
	{
		DWORD size = 0;
		for (UINT i = 0; i < Selected.size(); i++)
			size += Selected[i].object->GetMemorySize();
		msg.Format("%i selected, %i bytes",Selected.size(),size);
		m_ToolTip.EnableWindow(FALSE);
		m_ToolTip.SetWindowText(msg);
		m_ToolTip.SetWindowPos(NULL,point.x+16,point.y+16,msg.GetLength()*FontWidth+8,FontHeight+6,SWP_NOZORDER);
		m_ToolTip.ShowWindow(SW_SHOW);
	}
	else
	{
		if (!isDeleting) // Ghetto fix to stop crashing when deleting nodes you're hovered over
		{
			BYTE data[16];
			for (UINT i = 0; i < HotSpots.size(); i++)
			{
				if (HotSpots[i].Rect.PtInRect(point))
				{
					if (HotSpots[i].Type == HS_SELECT)
					{
						CNodeBase* pNode = (CNodeBase*)HotSpots[i].object;
						if (pNode->GetType() == nt_function)
						{
							DWORD addy;
							ReadMemory(HotSpots[i].Address, &addy, 4);
							char code[512];
							ReadMemory(addy,code,512);
							CString d = DisassembleCode(code, code + 100, addy);
							m_ToolTip.EnableWindow(FALSE);
							m_ToolTip.SetWindowText(d);
							m_ToolTip.SetWindowPos(NULL, point.x + 16, point.y + 16, 400, 350, SWP_NOZORDER);
							m_ToolTip.ShowWindow(SW_SHOW);
						}
						if (pNode->GetType() == nt_hex64)
						{
							ReadMemory(HotSpots[i].Address, data, 4);
							float* pf	= (float*)data;
							__int64* pi		= (__int64*)data;
							DWORD_PTR* pd	= (DWORD_PTR*)data;
							msg.Format("Int64: %i\r\nDWORD64: %u\r\nFloat: %.3f", *pi, *pd, *pf);

							m_ToolTip.EnableWindow(FALSE);
							m_ToolTip.SetWindowText(msg);
							m_ToolTip.SetWindowPos(NULL, point.x+16, point.y+16, 200, 16*3+6, SWP_NOZORDER);
							m_ToolTip.ShowWindow(SW_SHOW);
						}
						else if (pNode->GetType() == nt_hex32)
						{
							ReadMemory(HotSpots[i].Address,data,4);
							float* pf	= (float*)data;
							int* pi		= (int*)data;
							DWORD* pd	= (DWORD*)data;
							msg.Format("Int32: %i\r\nDWORD: %u\r\nFloat: %.3f", *pi, *pd, *pf);
							m_ToolTip.EnableWindow(FALSE);
							m_ToolTip.SetWindowText(msg);
							m_ToolTip.SetWindowPos(NULL, point.x+16, point.y+16, 200, 16*3+6, SWP_NOZORDER);
							m_ToolTip.ShowWindow(SW_SHOW);
						}
						else if (pNode->GetType() == nt_hex16)
						{
							ReadMemory(HotSpots[i].Address,data,4);
							__int16* pi		= (__int16*)data;
							WORD* pd	= (WORD*)data;
							msg.Format("Int16: %i\r\nWORD: %u\r\n",*pi,*pd);
							m_ToolTip.EnableWindow(FALSE);
							m_ToolTip.SetWindowText(msg);
							m_ToolTip.SetWindowPos(NULL,point.x+16,point.y+16,200,16*2+6,SWP_NOZORDER);
							m_ToolTip.ShowWindow(SW_SHOW);
						}
						else if (pNode->GetType() == nt_hex8)
						{
							ReadMemory(HotSpots[i].Address,data,4);
							__int8* pi		= (__int8*)data;
							BYTE* pd	= (BYTE*)data;
							msg.Format("Int8: %i\r\nBYTE: %u\r\n",*pi,*pd);
							m_ToolTip.EnableWindow(FALSE);
							m_ToolTip.SetWindowText(msg);
							m_ToolTip.SetWindowPos(NULL,point.x+16,point.y+16,200,16*2+6,SWP_NOZORDER);
							m_ToolTip.ShowWindow(SW_SHOW);
						}
					}
				}
			}
		}
	}

	bTracking = false;
	HoverPoint = point;

	CWnd::OnMouseHover(nFlags, point);
}

void CChildView::OnMouseMove( UINT nFlags, CPoint point )
{
	if (point != HoverPoint)
		m_ToolTip.ShowWindow(SW_HIDE);
	if (!bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent(&tme);
		bTracking = true;
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnMouseLeave( )
{
	m_ToolTip.ShowWindow(SW_HIDE);
	bTracking = false;
	CWnd::OnMouseLeave();
}

UINT CChildView::FindNodeIndex(CNodeBase* pNode)
{
	if (!pNode->pParent)
		return MAX_NODES;
	CNodeClass* pClass = (CNodeClass*)pNode->pParent;
	for (UINT i = 0; i < pClass->Nodes.size(); i++)
	{
		if (pClass->Nodes[i] == pNode)
			return i;
	}
	return MAX_NODES;
}

CNodeBase* CChildView::FindNodeFromIndex(CNodeBase* currentlySelectedNode, UINT index)
{
	CNodeBase* pNode = currentlySelectedNode;
	if (!pNode->pParent)
		return NULL;
	CNodeClass* pClass = (CNodeClass*)pNode->pParent;
	if (index >= pClass->Nodes.size())
		return NULL;
	return pClass->Nodes[index];
}

void CChildView::ReplaceNode(CNodeClass* pClass, UINT idx, CNodeBase* pNewNode)
{
	if (!pClass || idx == MAX_NODES)
		return;

	CNodeBase* pOldNode = pClass->Nodes[idx];
	pNewNode->Name		= pOldNode->Name;
	pNewNode->Comment	= pOldNode->Comment;

	//printf( "OFFSET %p ********************************\n ", pOldNode->offset );
	
	// This looks wrong
	// TODO: fix this
	//if ( pOldNode->offset < 0x140000000 )
	//{
	//	pNewNode->offset = 0x140000000;
	//	pNewNode->strOffset = "0x140000000";
	//} else {
	//	pNewNode->offset = pOldNode->offset;
	//}

	pNewNode->pParent	= pClass;
	pNewNode->bSelected = false;

	//m_pSelected = pNewNode;
	pClass->Nodes[idx] = pNewNode;

	DWORD sOld = pOldNode->GetMemorySize();
	DWORD sNew = pNewNode->GetMemorySize();

	if (sOld != sNew)
	{
		if (sNew < sOld)
			FillNodes( pClass, idx+1, sOld - sNew );
		else
			RemoveNodes( pClass, idx+1, sNew - sOld );
	}

	delete pOldNode;

	theApp.CalcAllOffsets();
}

void CChildView::RemoveNodes(CNodeClass* pClass,UINT idx,DWORD Length)
{
	if (!pClass || idx == MAX_NODES) return;

	UINT t = 0;
	DWORD totalSize = 0;
	for (UINT i=idx; i<pClass->Nodes.size();i++)
	{
		totalSize += pClass->Nodes[i]->GetMemorySize();
		t++;
		if (totalSize >= Length) break;
	}

	for (UINT i=0; i < t; i++)
	{
		delete pClass->Nodes[idx];
		pClass->Nodes.erase(pClass->Nodes.begin()+idx);
	}
	if (totalSize > Length)
	{
		FillNodes(pClass,idx,totalSize - Length);
	}
	theApp.CalcAllOffsets();
}

void CChildView::FillNodes(CNodeClass* pClass, UINT idx, DWORD Length)
{
	if (!pClass || idx == MAX_NODES)
		return;

	DWORD_PTR newOffset = 0;
	
	if ( idx > 0 )
		newOffset = pClass->Nodes[idx-1]->offset + pClass->Nodes[idx-1]->GetMemorySize();

	while(Length != 0)
	{
		// Assume 8?
		// TODO: to figure this out
		if ( Length >= 8 )
		{
			CNodeHex64* pFill = new CNodeHex64;
			pFill->pParent = pClass;
			pFill->offset = newOffset;
			//pFill->Comment.Format("%i-%i",idx,Length);

			//printf( "___________begin %p _______\n", pClass->Nodes.begin( ) );

			pClass->Nodes.insert( pClass->Nodes.begin( ) + idx, pFill );
			newOffset += 8;
			Length -= 8;
			idx++;
		}

		if (Length >= 4 )
		{
			CNodeHex32* pFill = new CNodeHex32;
			pFill->pParent = pClass;
			pFill->offset = newOffset;
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->Nodes.insert(pClass->Nodes.begin()+idx,pFill);
			newOffset += 4;
			Length -= 4;
			idx++;
		}

		if (Length >= 2 && Length < 4 )
		{
			CNodeHex16* pFill = new CNodeHex16;
			pFill->pParent = pClass;
			pFill->offset = newOffset;
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->Nodes.insert(pClass->Nodes.begin()+idx,pFill);
			newOffset += 2;
			Length -= 2;
			idx++;
		}

		if ( Length == 1 )
		{
			CNodeHex8* pFill = new CNodeHex8;
			pFill->pParent = pClass;
			pFill->offset = newOffset;
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->Nodes.insert(pClass->Nodes.begin()+idx,pFill);
			newOffset += 1;
			Length -= 1;
			idx++;
		}
	}
}

void CChildView::ResizeNode(CNodeClass* pClass,UINT idx,DWORD before,DWORD After)
{
	if (!pClass || idx == MAX_NODES) 
		return;

	if (before != After)
	{
		if (After < before)
			FillNodes(pClass,idx+1,before-After);
		else
			RemoveNodes(pClass,idx+1,After-before);
	}

	theApp.CalcAllOffsets();
}

void CChildView::AddBytes(CNodeClass* pClass, DWORD Length)
{
	if (!pClass)
		return;

	#ifdef _WIN64
	for (UINT i = 0; i < Length / 8; i++)
	{
		CNodeBase* pNode;
		if (pClass->GetType() == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			 pNode = new CNodeHex64;

		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	#else 
	for (UINT i = 0; i < Length / 4; i++)
	{
		CNodeBase* pNode;
		if (pClass->GetType() == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex32;

		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	#endif

	theApp.CalcAllOffsets();
}

void CChildView::InsertBytes( CNodeClass* pClass, UINT idx, DWORD Length)
{
	if ( !pClass || idx == MAX_NODES )
		return;

	// Ghetto fix
	if (Length == 4)
	{
		CNodeBase* pNode = new CNodeHex32;
		pNode->pParent = pClass;
		pClass->Nodes.insert(pClass->Nodes.begin() + idx, pNode);
		theApp.CalcAllOffsets();
		return;
	}

	#ifdef _WIN64
	for ( UINT i = 0; i < Length / 8 ; i++ )
	{
		CNodeBase* pNode;
		if ( pClass->GetType( ) == nt_vtable )
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex64;

		pNode->pParent = pClass;
		pClass->Nodes.insert( pClass->Nodes.begin( ) + idx, pNode );
	}
	#else
	for (UINT i = 0; i < Length / 4; i++)
	{
		CNodeBase* pNode;
		if ( pClass->GetType( ) == nt_vtable )
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex32;

		pNode->pParent = pClass;
		pClass->Nodes.insert( pClass->Nodes.begin( ) + idx, pNode );
	}
	#endif

	theApp.CalcAllOffsets( );
}

void CChildView::OnAddAdd8()
{
	if (Selected[0].object->GetType() == nt_class)
		AddBytes((CNodeClass*)Selected[0].object, 8);
	else
		AddBytes((CNodeClass*)Selected[0].object->pParent, 8);
	Invalidate(FALSE);
}
void CChildView::OnUpdateAddAdd8(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && (Selected[0].object->pParent || (Selected[0].object->GetType()==nt_class)))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
void CChildView::OnAddAdd64()
{
	if (Selected[0].object->GetType()==nt_class)
		AddBytes((CNodeClass*)Selected[0].object, 64);
	else
		AddBytes((CNodeClass*)Selected[0].object->pParent, 64);
	Invalidate(FALSE);
}
void CChildView::OnUpdateAddAdd64(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && (Selected[0].object->pParent || (Selected[0].object->GetType()==nt_class)) )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
void CChildView::OnAddAdd1024()
{
	if (Selected[0].object->GetType() == nt_class)
		AddBytes((CNodeClass*)Selected[0].object, 1024);
	else
		AddBytes((CNodeClass*)Selected[0].object->pParent, 1024);
	Invalidate(FALSE);
}
void CChildView::OnUpdateAddAdd1024(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && (Selected[0].object->pParent || (Selected[0].object->GetType() == nt_class)) )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CChildView::OnInsertInsert4()
{
	InsertBytes((CNodeClass*)Selected[0].object->pParent, FindNodeIndex(Selected[0].object), 4 );
	Invalidate(FALSE);
}
void CChildView::OnUpdateInsertInsert4( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->pParent)
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert8( )
{
	InsertBytes((CNodeClass*)Selected[0].object->pParent, FindNodeIndex(Selected[0].object), 8 );
	Invalidate(FALSE);
}
void CChildView::OnUpdateInsertInsert8(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && Selected[0].object->pParent)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CChildView::OnInsertInsert64()
{
	InsertBytes((CNodeClass*)Selected[0].object->pParent, FindNodeIndex(Selected[0].object), 64);
	Invalidate(FALSE);
}

void CChildView::OnUpdateInsertInsert64(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && Selected[0].object->pParent)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CChildView::OnInsertInsert1024()
{
	InsertBytes((CNodeClass*)Selected[0].object->pParent, FindNodeIndex(Selected[0].object), 1024);
	Invalidate(FALSE);
}

void CChildView::OnUpdateInsertInsert1024(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && Selected[0].object->pParent)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CChildView::OnInsertInsert2048()
{
	InsertBytes((CNodeClass*)Selected[0].object->pParent, FindNodeIndex(Selected[0].object), 2048);
	Invalidate(FALSE);
}

void CChildView::OnUpdateInsertInsert2048(CCmdUI *pCmdUI)
{
	if (Selected.size() == 1 && Selected[0].object->pParent)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void MakeBasicClass(CNodeClass* pClass)
{

#ifdef _WIN64
	for (int i = 0; i < 64/8; i++)
	{
		CNodeHex64* pNode = new CNodeHex64();
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	theApp.CalcOffsets(pClass);
	theApp.Classes.push_back(pClass);
#else
	for (int i = 0; i < 1; i++)
	{
		CNodeHex32* pNode = new CNodeHex32();
		pNode->pParent = pClass;
		pClass->Nodes.push_back(pNode);
	}
	theApp.CalcOffsets(pClass);
	theApp.Classes.push_back(pClass);
#endif
}

void CChildView::ReplaceSelectedWithType(NodeType Type)
{
	std::vector<CNodeBase*> newSelected;

	printf( "Replace Node Type %s\n", NodeTypeToString(Type));

	for (UINT i = 0; i < Selected.size(); i++)
	{
		if (!theApp.IsNodeValid(Selected[i].object))
			continue;
		if (Selected[i].object->pParent->GetType() == nt_vtable)
			Type = nt_function;

		CNodeBase* pNewNode = theApp.CreateNewNode(Type);

		if (Type == nt_class)	MakeBasicClass((CNodeClass*)pNewNode);
		if (Type == nt_custom)	((CNodeCustom*)	pNewNode)->memsize = Selected[i].object->GetMemorySize();
		if (Type == nt_text)	((CNodeText*)	pNewNode)->memsize = Selected[i].object->GetMemorySize();
		if (Type == nt_unicode)	((CNodeUnicode*)pNewNode)->memsize = Selected[i].object->GetMemorySize();
		if (Type == nt_vtable)
		{
			for (int i = 0; i < 10; i++)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNewNode;
				CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
				pFun->offset = i * 8;
				pFun->pParent = pVTable;
				pVTable->Nodes.push_back(pFun);
			}
		}
		if (Type == nt_pointer)
		{
			//printf( "Create POINTER******\n" );

			CNodePtr*	pPtr	= (CNodePtr*)pNewNode;
			CNodeClass* pClass	= (CNodeClass*)theApp.CreateNewNode(nt_class);
			MakeBasicClass(pClass);
			pPtr->pNode = pClass;
		}
		if (Type == nt_array)
		{
			CNodeArray* pArray = (CNodeArray*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)theApp.CreateNewNode(nt_class);
			MakeBasicClass(pClass);
			pArray->pNode = pClass;
		}
		if (Type == nt_instance)
		{
			CNodeClassInstance* pInstance	= (CNodeClassInstance*)pNewNode;
			CNodeClass*			pClass		= (CNodeClass*)theApp.CreateNewNode(nt_class);
			MakeBasicClass(pClass);
			pInstance->pNode = pClass;
		}

		ReplaceNode((CNodeClass*)Selected[i].object->pParent, FindNodeIndex(Selected[i].object), pNewNode);
		newSelected.push_back(pNewNode);
	}

	Selected.clear();
	for (UINT i = 0; i < newSelected.size(); i++)
	{
		newSelected[i]->bSelected = true;
		CNodeClass* pClass = (CNodeClass*)newSelected[i]->pParent;

		HotSpot spot;
		spot.Address = pClass->offset + newSelected[i]->offset;
		spot.object = newSelected[i];
		Selected.push_back(spot);
	}

	Invalidate(FALSE);
}

void CChildView::OnTypeHex64()
{
	ReplaceSelectedWithType(nt_hex64);
}

void CChildView::OnUpdateTypeHex64(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeHex32()
{
	ReplaceSelectedWithType(nt_hex32);
}

void CChildView::OnUpdateTypeHex32(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeHex16()
{
	ReplaceSelectedWithType(nt_hex16);
}

void CChildView::OnUpdateTypeHex16(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeHex8()
{
	ReplaceSelectedWithType(nt_hex8);
}

void CChildView::OnUpdateTypeHex8(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeInt64()
{
	ReplaceSelectedWithType(nt_int64);
}

void CChildView::OnUpdateTypeInt64(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeInt32()
{
	ReplaceSelectedWithType(nt_int32);
}

void CChildView::OnUpdateTypeInt32(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeInt16()
{
	ReplaceSelectedWithType(nt_int16);
}

void CChildView::OnUpdateTypeInt16(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeInt8()
{
	ReplaceSelectedWithType(nt_int8);
}

void CChildView::OnUpdateTypeInt8(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeDword()
{
	ReplaceSelectedWithType(nt_uint32);
}

void CChildView::OnUpdateTypeDword(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeWord()
{
	ReplaceSelectedWithType(nt_uint16);
}

void CChildView::OnUpdateTypeWord(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeByte()
{
	ReplaceSelectedWithType(nt_uint8);
}

void CChildView::OnUpdateTypeByte(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeVec2()
{
	ReplaceSelectedWithType(nt_vec2);
}

void CChildView::OnUpdateTypeVec2(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeVec3()
{
	ReplaceSelectedWithType(nt_vec3);
}

void CChildView::OnUpdateTypeVec3(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeQuat()
{
	ReplaceSelectedWithType(nt_quat);
}

void CChildView::OnUpdateTypeQuat(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeMatrix()
{
	ReplaceSelectedWithType(nt_matrix);
}

void CChildView::OnUpdateTypeMatrix(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeFloat()
{
	ReplaceSelectedWithType(nt_float);
}

void CChildView::OnUpdateTypeFloat(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeDouble()
{
	ReplaceSelectedWithType(nt_double);
}

void CChildView::OnUpdateTypeDouble(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeCustom()
{
	ReplaceSelectedWithType(nt_custom);
}

void CChildView::OnUpdateTypeCustom(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeUnicode( void )
{
	ReplaceSelectedWithType(nt_unicode);
}

void CChildView::OnUpdateTypeUnicode( CCmdUI *pCmdUI )
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeText()
{
	ReplaceSelectedWithType(nt_text);
}

void CChildView::OnUpdateTypeText(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypePChar()
{
	ReplaceSelectedWithType(nt_pchar);
}

void CChildView::OnUpdateTypePChar(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeVtable()
{
	ReplaceSelectedWithType(nt_vtable);
}

void CChildView::OnUpdateTypeVtable(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeFunction()
{
	ReplaceSelectedWithType(nt_function);
}

void CChildView::OnUpdateTypeFunction(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypePointer()
{
	ReplaceSelectedWithType(nt_pointer);
}

void CChildView::OnUpdateTypePointer(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeArray()
{
	ReplaceSelectedWithType(nt_array);
}

void CChildView::OnUpdateTypeArray(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnTypeClass()
{
	ReplaceSelectedWithType(nt_instance);
}

void CChildView::OnUpdateTypeClass(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnModifyDelete()
{
	for (UINT i = 0; i < Selected.size(); i++)
	{
		CNodeClass* pClass = (CNodeClass*)Selected[i].object->pParent;
		UINT idx = FindNodeIndex(Selected[i].object);
		if (idx != MAX_NODES)
		{
			delete pClass->Nodes[idx];
			pClass->Nodes.erase(pClass->Nodes.begin() + idx);
			theApp.CalcAllOffsets();
		}
	}
	Selected.clear();
}

void CChildView::OnUpdateModifyDelete(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnModifyShow()
{
	theApp.ClearHidden();
}

void CChildView::OnUpdateModifyShow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CChildView::OnModifyHide()
{
	for(UINT i=0; i < Selected.size();i++) Selected[i].object->bHidden = true;
	Invalidate(FALSE);
}

void CChildView::OnUpdateModifyHide(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnButtonEditcode()
{
	CDialogEdit dlg;
	dlg.Title.Format("Code for %s",m_pClass->Name);
	dlg.Text = m_pClass->Code;
	dlg.DoModal();
	m_pClass->Code = dlg.Text;
}

BOOL CChildView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		if (nID >= WM_CHANGECLASSMENU && nID   < (WM_CHANGECLASSMENU+WM_MAXITEMS)){ ((CCmdUI*)pExtra)->Enable(TRUE); return TRUE;}
	}
	if (nCode == CN_COMMAND)
	{
		if (nID >= WM_CHANGECLASSMENU && nID < (WM_CHANGECLASSMENU+WM_MAXITEMS) )
		{
			UINT idx = nID - WM_CHANGECLASSMENU;
			CNodeBase* pNode = (CNodeBase*)ExchangeTarget.object;

			if (pNode->GetType() == nt_array) ((CNodeArray*)pNode)->pNode = theApp.Classes[idx];
			if (pNode->GetType() == nt_instance) ((CNodeClassInstance*)pNode)->pNode = theApp.Classes[idx];
			if (pNode->GetType() == nt_pointer) ((CNodePtr*)pNode)->pNode = theApp.Classes[idx];
			theApp.CalcAllOffsets();

			return TRUE;
		}
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CChildView::OnEditCopy()
{
	m_Edit.Copy();
}

void CChildView::OnEditPaste()
{
	//printf( "paste here\n" );
	//m_Edit.LimitText( 999 );
	m_Edit.Paste( );
}

void CChildView::StandardTypeUpdate(CCmdUI *pCmdUI)
{
	if ( Selected.size( ) > 0 )
	{
		if (Selected[0].object->GetType() == nt_class) 
			return pCmdUI->Enable(FALSE);
		return pCmdUI->Enable(TRUE);
	}
	pCmdUI->Enable(FALSE);
}

void CChildView::OnButtonZero()
{
	CMemory mem;
	for (UINT i = 0; i < Selected.size(); i++)
	{
		DWORD s = Selected[i].object->GetMemorySize();
		DWORD_PTR a = Selected[i].Address;
		mem.SetSize(s);
		ZeroMemory(mem.pMemory,s);
		WriteMemory(a,mem.pMemory,s);
	}
}

void CChildView::OnUpdateButtonZero(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnButtonOne()
{
	CMemory mem;
	for (UINT i = 0; i < Selected.size(); i++)
	{
		DWORD s = Selected[i].object->GetMemorySize();
		DWORD_PTR a = Selected[i].Address;
		mem.SetSize(s);
		memset(mem.pMemory, 1, s);
		WriteMemory(a, mem.pMemory, s);
	}
}

void CChildView::OnUpdateButtonOne(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}

void CChildView::OnButtonRandom()
{
	CMemory mem;
	srand(GetTickCount());
	for (UINT i=0; i < Selected.size();i++)
	{
		DWORD s = Selected[i].object->GetMemorySize();
		DWORD_PTR a = Selected[i].Address;
		mem.SetSize(s);
		for (UINT r=0; r < s; r++) 
			mem.pMemory[r] = rand();
		WriteMemory(a,mem.pMemory,s);
	}

}
void CChildView::OnUpdateButtonRandom(CCmdUI *pCmdUI)
{
	StandardTypeUpdate(pCmdUI);
}
