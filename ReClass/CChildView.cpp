#include "stdafx.h"

#include "CChildFrame.h"
#include "CChildView.h"
#include "DialogEdit.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
CChildView::CChildView( ) : 
	m_pClass( NULL ),
	m_bTracking( FALSE )
{
}

CChildView::~CChildView( )
{
}

BEGIN_MESSAGE_MAP( CChildView, CWnd )
	ON_WM_PAINT( )
	ON_WM_KEYDOWN( )
	ON_WM_LBUTTONDBLCLK( )
	ON_WM_CREATE( )
	ON_WM_LBUTTONDOWN( )
	ON_WM_RBUTTONDOWN( )
	ON_WM_SIZE( )
	ON_WM_VSCROLL( )
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND( )
	ON_WM_MOUSEWHEEL( )
	ON_WM_MOUSEHOVER( )
	ON_WM_MOUSEMOVE( )
	ON_WM_TIMER( )

	ON_COMMAND( ID_ADD_ADD4, &CChildView::OnAddAdd4 )
	ON_COMMAND( ID_ADD_ADD8, &CChildView::OnAddAdd8 )
	ON_COMMAND( ID_ADD_ADD64, &CChildView::OnAddAdd64 )
	ON_COMMAND( ID_ADD_ADD1024, &CChildView::OnAddAdd1024 )
	ON_COMMAND( ID_ADD_ADD2048, &CChildView::OnAddAdd2048 )
	ON_COMMAND( ID_TYPE_HEX64, &CChildView::OnTypeHex64 )
	ON_COMMAND( ID_TYPE_HEX32, &CChildView::OnTypeHex32 )
	ON_COMMAND( ID_TYPE_INT64, &CChildView::OnTypeInt64 )
	ON_COMMAND( ID_TYPE_INT32, &CChildView::OnTypeInt32 )
	ON_COMMAND( ID_TYPE_INT16, &CChildView::OnTypeInt16 )
	ON_COMMAND( ID_TYPE_INT8, &CChildView::OnTypeInt8 )
	ON_COMMAND( ID_TYPE_HEX16, &CChildView::OnTypeHex16 )
	ON_COMMAND( ID_TYPE_HEX8, &CChildView::OnTypeHex8 )
	ON_COMMAND( ID_TYPE_BITS, &CChildView::OnTypeBits )
	ON_COMMAND( ID_TYPE_QWORD, &CChildView::OnTypeQword )
	ON_COMMAND( ID_TYPE_DWORD, &CChildView::OnTypeDword )
	ON_COMMAND( ID_TYPE_WORD, &CChildView::OnTypeWord )
	ON_COMMAND( ID_TYPE_BYTE, &CChildView::OnTypeByte )
	ON_COMMAND( ID_TYPE_VEC2, &CChildView::OnTypeVec2 )
	ON_COMMAND( ID_TYPE_VEC3, &CChildView::OnTypeVec3 )
	ON_COMMAND( ID_TYPE_QUAT, &CChildView::OnTypeQuat )
	ON_COMMAND( ID_TYPE_FLOAT, &CChildView::OnTypeFloat )
	ON_COMMAND( ID_TYPE_DOUBLE, &CChildView::OnTypeDouble )
	ON_COMMAND( ID_TYPE_MATRIX, &CChildView::OnTypeMatrix )
	ON_COMMAND( ID_TYPE_CUSTOM, &CChildView::OnTypeCustom )
	ON_COMMAND( ID_TYPE_TEXT, &CChildView::OnTypeText )
	ON_COMMAND( ID_TYPE_PCHAR, &CChildView::OnTypePChar )
	ON_COMMAND( ID_TYPE_PWCHAR, &CChildView::OnTypePWChar )
	ON_COMMAND( ID_TYPE_UNICODE, &CChildView::OnTypeUnicode )
	ON_COMMAND( ID_INSERT_INSERT4, &CChildView::OnInsertInsert4 )
	ON_COMMAND( ID_INSERT_INSERT8, &CChildView::OnInsertInsert8 )
	ON_COMMAND( ID_INSERT_INSERT64, &CChildView::OnInsertInsert64 )
	ON_COMMAND( ID_INSERT_INSERT1024, &CChildView::OnInsertInsert1024 )
	ON_COMMAND( ID_INSERT_INSERT2048, &CChildView::OnInsertInsert2048 )
	ON_COMMAND( ID_TYPE_VTABLE, &CChildView::OnTypeVtable )
	ON_COMMAND( ID_TYPE_FUNCTION, &CChildView::OnTypeFunction )
	ON_COMMAND( ID_TYPE_FUNCTION_PTR, &CChildView::OnTypeFunctionPtr )
	ON_COMMAND( ID_TYPE_POINTER, &CChildView::OnTypePointer )
	ON_COMMAND( ID_TYPE_ARRAY, &CChildView::OnTypeArray )
	ON_COMMAND( ID_TYPE_CLASS, &CChildView::OnTypeClass )
	ON_COMMAND( ID_MODIFY_DELETE, &CChildView::OnModifyDelete )
	ON_COMMAND( ID_MODIFY_SHOW, &CChildView::OnModifyShow )
	ON_COMMAND( ID_MODIFY_HIDE, &CChildView::OnModifyHide )

	ON_UPDATE_COMMAND_UI( ID_ADD_ADD4, &CChildView::OnUpdateAddAdd4 )
	ON_UPDATE_COMMAND_UI( ID_ADD_ADD8, &CChildView::OnUpdateAddAdd8 )
	ON_UPDATE_COMMAND_UI( ID_ADD_ADD64, &CChildView::OnUpdateAddAdd64 )
	ON_UPDATE_COMMAND_UI( ID_ADD_ADD1024, &CChildView::OnUpdateAddAdd1024 )
	ON_UPDATE_COMMAND_UI( ID_ADD_ADD2048, &CChildView::OnUpdateAddAdd2048 )
	ON_UPDATE_COMMAND_UI( ID_INSERT_INSERT4, &CChildView::OnUpdateInsertInsert4 )
	ON_UPDATE_COMMAND_UI( ID_INSERT_INSERT8, &CChildView::OnUpdateInsertInsert8 )
	ON_UPDATE_COMMAND_UI( ID_INSERT_INSERT64, &CChildView::OnUpdateInsertInsert64 )
	ON_UPDATE_COMMAND_UI( ID_INSERT_INSERT1024, &CChildView::OnUpdateInsertInsert1024 )
	ON_UPDATE_COMMAND_UI( ID_INSERT_INSERT2048, &CChildView::OnUpdateInsertInsert2048 )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_DELETE, &CChildView::OnUpdateModifyDelete )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_SHOW, &CChildView::OnUpdateModifyShow )
	ON_UPDATE_COMMAND_UI( ID_MODIFY_HIDE, &CChildView::OnUpdateModifyHide )
	ON_UPDATE_COMMAND_UI( ID_TYPE_HEX64, &CChildView::OnUpdateTypeHex64 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_HEX32, &CChildView::OnUpdateTypeHex32 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_HEX16, &CChildView::OnUpdateTypeHex16 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_HEX8, &CChildView::OnUpdateTypeHex8 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_BITS, &CChildView::OnUpdateTypeBits )
	ON_UPDATE_COMMAND_UI( ID_TYPE_INT64, &CChildView::OnUpdateTypeInt64 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_INT32, &CChildView::OnUpdateTypeInt32 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_INT16, &CChildView::OnUpdateTypeInt16 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_INT8, &CChildView::OnUpdateTypeInt8 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_QWORD, &CChildView::OnUpdateTypeQword )
	ON_UPDATE_COMMAND_UI( ID_TYPE_DWORD, &CChildView::OnUpdateTypeDword )
	ON_UPDATE_COMMAND_UI( ID_TYPE_WORD, &CChildView::OnUpdateTypeWord )
	ON_UPDATE_COMMAND_UI( ID_TYPE_BYTE, &CChildView::OnUpdateTypeByte )
	ON_UPDATE_COMMAND_UI( ID_TYPE_TEXT, &CChildView::OnUpdateTypeText )
	ON_UPDATE_COMMAND_UI( ID_TYPE_PCHAR, &CChildView::OnUpdateTypePChar )
	ON_UPDATE_COMMAND_UI( ID_TYPE_PWCHAR, &CChildView::OnUpdateTypePWChar )
	ON_UPDATE_COMMAND_UI( ID_TYPE_UNICODE, &CChildView::OnUpdateTypeUnicode )
	ON_UPDATE_COMMAND_UI( ID_TYPE_DOUBLE, &CChildView::OnUpdateTypeDouble )
	ON_UPDATE_COMMAND_UI( ID_TYPE_FLOAT, &CChildView::OnUpdateTypeFloat )
	ON_UPDATE_COMMAND_UI( ID_TYPE_CUSTOM, &CChildView::OnUpdateTypeCustom )
	ON_UPDATE_COMMAND_UI( ID_TYPE_VEC2, &CChildView::OnUpdateTypeVec2 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_VEC3, &CChildView::OnUpdateTypeVec3 )
	ON_UPDATE_COMMAND_UI( ID_TYPE_QUAT, &CChildView::OnUpdateTypeQuat )
	ON_UPDATE_COMMAND_UI( ID_TYPE_MATRIX, &CChildView::OnUpdateTypeMatrix )
	ON_UPDATE_COMMAND_UI( ID_TYPE_ARRAY, &CChildView::OnUpdateTypeArray )
	ON_UPDATE_COMMAND_UI( ID_TYPE_CLASS, &CChildView::OnUpdateTypeClass )
	ON_UPDATE_COMMAND_UI( ID_TYPE_VTABLE, &CChildView::OnUpdateTypeVtable )
	ON_UPDATE_COMMAND_UI( ID_TYPE_FUNCTION, &CChildView::OnUpdateTypeFunction )
	ON_UPDATE_COMMAND_UI( ID_TYPE_POINTER, &CChildView::OnUpdateTypePointer )

	ON_COMMAND( ID_BUTTON_EDITCODE, &CChildView::OnButtonEditcode )
	ON_COMMAND( ID_EDIT_COPY, &CChildView::OnEditCopy )
	ON_COMMAND( ID_EDIT_PASTE, &CChildView::OnEditPaste )

	ON_WM_MOUSELEAVE( )

	ON_COMMAND( ID_BUTTON_ZERO, &CChildView::OnButtonZero )
	ON_COMMAND( ID_BUTTON_ONE, &CChildView::OnButtonOne )
	ON_COMMAND( ID_BUTTON_RANDOM, &CChildView::OnButtonRandom )
	ON_COMMAND( ID_BUTTON_SWAP, &CChildView::OnButtonSwap )

	ON_UPDATE_COMMAND_UI( ID_BUTTON_ZERO, &CChildView::OnUpdateButtonZero )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_ONE, &CChildView::OnUpdateButtonOne )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_RANDOM, &CChildView::OnUpdateButtonRandom )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_SWAP, &CChildView::OnUpdateButtonSwap )
END_MESSAGE_MAP( )

#define SB_WIDTH 14

// CChildView message handlers
BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs )
{
	//cs.cy += g_FontHeight;
	if (!CWnd::PreCreateWindow( cs ))
		return FALSE;
	cs.dwExStyle |= WS_EX_STATICEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, ::LoadCursor( NULL, IDC_ARROW ), (HBRUSH)COLOR_WINDOWFRAME, NULL );
	return TRUE;
}

int CChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	lpCreateStruct->cy += g_FontHeight;
	if (CWnd::OnCreate( lpCreateStruct ) == -1)
		return -1;

	CRect rect( 0, 0, 100, 100 );
	CRect hrect(5, 5, 100, 30);
	//m_Edit.CreateEx(WS_EX_WINDOWEDGE, _T("EDIT"),  _T(" "), WS_CHILD | WS_TABSTOP, rect, this, 1);
	m_Edit.Create( WS_CHILD | WS_TABSTOP, rect, this, 1 );
	m_Edit.ShowWindow( SW_HIDE );
	m_Edit.SetFont( &g_ViewFont );

	m_Scroll.Create( SBS_VERT, rect, this, 0 );
	m_Scroll.EnableScrollBar( ESB_ENABLE_BOTH );
	m_Scroll.ShowScrollBar( );

	m_HScroll.Create(SBS_HORZ, hrect, this, 0);
	m_HScroll.EnableScrollBar(ESB_ENABLE_BOTH);
	m_HScroll.ShowScrollBar();

	m_ToolTip.Create( ES_MULTILINE | WS_BORDER, rect, this, 1 );
	m_ToolTip.SetFont( &g_ViewFont );
	m_ToolTip.EnableWindow( FALSE );

	SetTimer( 1, 250, NULL );

	return 0;
}

void CChildView::OnTimer( UINT_PTR nIDEvent )
{
	if (nIDEvent == 1)
		Invalidate( FALSE );
	CWnd::OnTimer( nIDEvent );
}

void CChildView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == VK_DOWN)
	{
		auto firstSelected = Selected.begin( );
		if (firstSelected != Selected.end( ))
		{
			if (firstSelected->object->GetType( ) == nt_pointer)
			{
				CNodePtr* pPtrNode = (CNodePtr*)firstSelected->object;
				CNodeBase* pFindClassNode = (CNodeBase*)pPtrNode->GetClass( );

				auto found = std::find_if( HotSpots.begin( ), HotSpots.end( ),
										   [pFindClassNode] ( const HotSpot hs ) { return (hs.object == pFindClassNode); } );
				if (found != HotSpots.end( ))
				{
					ClearSelection( );

					found->object->Select( );
					Selected.push_back( *found );
				}
			}
			else if (firstSelected->object->GetType( ) == nt_class)
			{
				CNodeClass* pClassNode = (CNodeClass*)firstSelected->object;
				CNodeBase* pFindNode = (CNodeBase*)pClassNode->GetNode( 0 );

				auto found = std::find_if( HotSpots.begin( ), HotSpots.end( ),
										   [pFindNode] ( const HotSpot hs ) { return (hs.object == pFindNode); } );
				if (found != HotSpots.end( ))
				{
					ClearSelection( );

					found->object->Select( );
					Selected.push_back( *found );
				}
			}
			else
			{
				if (HotSpots[0].object != firstSelected->object)
				{
					ULONG_PTR findAddress = firstSelected->Address + firstSelected->object->GetMemorySize( );

					auto found = std::find_if( HotSpots.begin( ), HotSpots.end( ),
											   [findAddress] ( const HotSpot hs ) { return (hs.Address == findAddress); } );
					if (found != HotSpots.end( ))
					{
						if (found->Address == HotSpots.back( ).Address)
						{
							if (m_Scroll.IsWindowEnabled( ))
								m_Scroll.SetScrollPos( m_Scroll.GetScrollPos( ) + 1 );
						}
						
						ClearSelection( );
						found->object->Select( );
						Selected.push_back( *found );
					}
					//else // Not found
					//{
					//	CNodeBase* selectedObject = firstSelected->object;
					//	CNodeBase* selectedObjectParent = selectedObject->GetParent( );
					//	if (selectedObjectParent->GetType( ) == nt_class)
					//	{
					//
					//	}
					//}
				}
			}

			// Force redraw so it doesn't appear laggy
			Invalidate( );
		}
	}
	else if (nChar == VK_UP)
	{
		auto firstSelected = Selected.begin( );
		if (firstSelected != Selected.end( ))
		{
			if (
				firstSelected->Address == (HotSpots[0].Address + HotSpots[0].object->GetMemorySize( )) ||
				firstSelected->object == HotSpots.begin( )->object)
			{
				if (m_Scroll.IsWindowEnabled( ))
				{
					INT scrollPos = m_Scroll.GetScrollPos( );
					m_Scroll.SetScrollPos( (scrollPos != 0) ? scrollPos - 1 : 0 );
				}
			}

			ULONG_PTR findAddress = firstSelected->Address;

			auto found = std::find_if( HotSpots.begin( ), HotSpots.end( ),
									   [findAddress] ( const HotSpot hs ) { return (hs.Address == findAddress); } );
			if (found != HotSpots.end( ))
			{
				ClearSelection( );
				if (found != HotSpots.begin( ))
					found--;
				found->object->Select( );
				Selected.push_back( *found );
			}
			//// Bring selected node into view
			//else
			//{
			//	firstSelected->Rect.top
			//}

			// Force redraw so it doesn't appear laggy
			Invalidate( );
		}
	}
	else if (nChar == VK_DELETE)
	{
		for (UINT i = 0; i < Selected.size( ); i++)
		{
			CNodeClass* pClass = (CNodeClass*)Selected[i].object->GetParent( );
			UINT idx = FindNodeIndex( Selected[i].object );
			if (idx != MAX_NODES)
			{
				pClass->DeleteNode( idx );
				g_ReClassApp.CalcAllOffsets( );
			}
		}
		Selected.clear( );
	}

	CWnd::OnKeyDown( nChar, nRepCnt, nFlags );
}

void CChildView::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDblClk( nFlags, point );

	for (UINT i = 0; i < HotSpots.size( ); i++)
	{
		if (HotSpots[i].Rect.PtInRect( point ))
		{
			if (HotSpots[i].Type == HS_EDIT)
			{
				// Sets the edit "window" to where to cursor was editing at
				m_Edit.SetWindowPos( NULL, HotSpots[i].Rect.left, HotSpots[i].Rect.top, HotSpots[i].Rect.Width( ), HotSpots[i].Rect.Height( ), SWP_NOZORDER );
				m_Edit.spot = HotSpots[i];
				m_Edit.MinWidth = m_Edit.spot.Rect.Width( );
				m_Edit.SetWindowText( HotSpots[i].Text );
				m_Edit.ShowWindow( SW_NORMAL );
				m_Edit.SetFocus( );
				//m_Edit.CreateSolidCaret(FontWidth,FontHeight);
				//m_Edit.ShowCaret();
				m_Edit.SetSel( 0, 1024 );
				return;
			}
			//else if (HotSpots[i].Type == HS_SCINTILLA_EDIT)
			//{
			//	//(HotSpots[i].object)->
			//}
		}
	}
}

TCHAR GetBeginChar( CString name )
{
	if (!name.IsEmpty( ))
	{
		TCHAR real = name.MakeUpper( ).GetAt( 0 );
		if (_istalpha( real ) || _istalnum( real ))
			return real;
	}
	return '_';
}

bool SortBeginnings( std::pair<TCHAR, std::vector<std::pair<CString, UINT>>> i, std::pair<TCHAR, std::vector<std::pair<CString, UINT>>> j )
{
	return (i.first < j.first);
}

bool SortClassesByName( std::pair<CString, UINT> i, std::pair<CString, UINT> j )
{
	return (GetBeginChar( i.first ) < GetBeginChar( j.first ));
}

std::vector<std::pair<TCHAR, std::vector<std::pair<CString, UINT>>>> ExplodeByFirstChar( std::vector<std::pair<CString, UINT>> classRefs )
{
	std::vector<std::pair<TCHAR, std::vector<std::pair<CString, UINT>>>> out;

	for (int i = 0; i < classRefs.size( ); i++)
	{
		std::pair<CString, UINT>* classRef = &classRefs[i];

		// determine begin char
		TCHAR begin = GetBeginChar( classRef->first );

		// find if already in out
		std::vector<std::pair<CString, UINT>>* outByBegin = NULL;
		for (int j = 0; j < out.size( ); j++)
		{
			if (out[j].first == begin)
			{
				outByBegin = &out[j].second;
				break;
			}
		}

		// create if missing
		if (outByBegin == NULL)
		{
			out.push_back( std::pair<TCHAR, std::vector<std::pair<CString, UINT>>>( begin, std::vector<std::pair<CString, UINT>>( ) ) );
			outByBegin = &out[out.size( ) - 1].second;
		}

		// add to correct begin vector
		outByBegin->push_back( *classRef );
	}

	// Sort all
	std::sort( out.begin( ), out.end( ), SortBeginnings );
	for (int i = 0; i < out.size( ); i++)
	{
		std::sort( out[i].second.begin( ), out[i].second.end( ), SortClassesByName );
	}

	return out;
}

void CChildView::OnLButtonDown( UINT nFlags, CPoint point )
{
	m_Edit.ShowWindow( SW_HIDE );
	for (UINT i = 0; i < HotSpots.size( ); i++)
	{
		if (HotSpots[i].Rect.PtInRect( point ))
		{
			CNodeBase* pHitObject = (CNodeBase*)HotSpots[i].object;

			if (HotSpots[i].Type == HS_OPENCLOSE)
			{
				pHitObject->ToggleLevelOpen( HotSpots[i].Level );
			}

			if (HotSpots[i].Type == HS_CLICK)
			{
				pHitObject->Update( HotSpots[i] );
			}

			if (HotSpots[i].Type == HS_SELECT)
			{
				if (nFlags == MK_LBUTTON)
				{
					//g_ReClassApp.ClearSelection( );
					//Selected.clear( );
					ClearSelection( );

					pHitObject->Select( );
					Selected.push_back( HotSpots[i] );
				}
				if (nFlags == (MK_LBUTTON | MK_CONTROL))
				{
					pHitObject->ToggleSelected( );
					if (pHitObject->IsSelected( ))
					{
						Selected.push_back( HotSpots[i] );
					}
					else
					{
						for (UINT s = 0; s < Selected.size( ); s++)
						{
							if (Selected[s].object == pHitObject)
							{
								Selected.erase( Selected.begin( ) + s );
								break;
							}
						}
					}
				}
				if (nFlags == (MK_LBUTTON | MK_SHIFT))
				{
					if (Selected.size( ) > 0)
					{
						CNodeBase* pSelectedNode = Selected[0].object;
						if (pSelectedNode->GetParent( ) != pHitObject->GetParent( ))
							continue;
						CNodeClass* pClass = (CNodeClass*)pSelectedNode->GetParent( );
						if (pClass->GetType( ) != nt_class)
							continue;

						UINT idx1 = FindNodeIndex( pSelectedNode );
						if (idx1 == MAX_NODES)
							continue;
						UINT idx2 = FindNodeIndex( pHitObject );
						if (idx2 == MAX_NODES)
							continue;
						if (idx2 < idx1)
						{
							UINT idxTemp = idx1;
							idx1 = idx2;
							idx2 = idxTemp;
						}

						//g_ReClassApp.ClearSelection( );
						//Selected.clear( );
						ClearSelection( );

						for (UINT s = idx1; s <= idx2; s++)
						{
							pClass->GetNode( s )->Select( );
							HotSpot spot;
							//ZeroMemory(&spot, sizeof(HotSpot));
							spot.Address = pClass->GetOffset( ) + pClass->GetNode( s )->GetOffset( );
							spot.object = pClass->GetNode( s );
							Selected.push_back( spot );
						}
					}
				}
			}

			if (HotSpots[i].Type == HS_DROP)
			{
				CRect client;
				GetClientRect( &client );
				ClientToScreen( &client );
				CMenu menu;
				menu.LoadMenu( MAKEINTRESOURCE( IDR_MENU_QUICKMODIFY ) );
				menu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, client.left + HotSpots[i].Rect.left, client.top + HotSpots[i].Rect.bottom, this );
			}

			if (HotSpots[i].Type == HS_DELETE)
			{
				for (UINT i = 0; i < Selected.size( ); i++)
				{
					CNodeClass* pClass = (CNodeClass*)Selected[i].object->GetParent( );
					UINT idx = FindNodeIndex( Selected[i].object );
					if (idx != MAX_NODES)
					{
						pClass->DeleteNode( idx );
						g_ReClassApp.CalcAllOffsets( );
					}
				}
				Selected.clear( );
			}

			if ((HotSpots[i].Type == HS_CHANGE_A) || (HotSpots[i].Type == HS_CHANGE_X))
			{
				CMenu menu;
				CImage img;
				CBitmap bmp;

				std::vector<std::pair<CString, UINT>> classRefs;

				CRect pos = { 0 };
				CNodeBase* pNode = NULL;

				ExchangeTarget = HotSpots[i];

				pos = ExchangeTarget.Rect;
				ClientToScreen( &pos );

				pNode = HotSpots[i].object;

				menu.CreatePopupMenu( );

				img.LoadFromResource( AfxGetResourceHandle( ), IDB_CLASSBITMAP );
				
				bmp.Attach( img.Detach( ) );

				for (UINT m = 0; m < g_ReClassApp.m_Classes.size( ); m++)
				{
					if ((HotSpots[i].Type == HS_CHANGE_X) && (pNode->GetParent( ) == g_ReClassApp.m_Classes[m]))
						continue;
					classRefs.push_back( std::pair<CString, UINT>( g_ReClassApp.m_Classes[m]->GetName( ), m ) );
				}

				std::vector<std::pair<wchar_t, std::vector<std::pair<CString, UINT>>>> out = ExplodeByFirstChar( classRefs );

				for (UINT i = 0; i < out.size( ); i++)
				{
					CMenu innerMenu;

					innerMenu.CreatePopupMenu( );

					for (UINT j = 0; j < out[i].second.size( ); j++)
					{
						innerMenu.AppendMenu( MF_STRING | MF_ENABLED, WM_CHANGECLASSMENU + out[i].second[j].second, out[i].second[j].first );
						innerMenu.SetMenuItemBitmaps( j, MF_BYPOSITION, &bmp, &bmp );
					}

					menu.AppendMenu( MF_POPUP, (UINT_PTR)innerMenu.m_hMenu, CString( out[i].first ) );
					menu.SetMenuItemBitmaps( i, MF_BYPOSITION, &bmp, &bmp );
				}

				menu.TrackPopupMenu( TPM_LEFTALIGN | TPM_NOANIMATION, pos.left, pos.bottom, this );
			}

			Invalidate( );
		}
	}

	CWnd::OnLButtonDown( nFlags, point );
}

void CChildView::OnRButtonDown( UINT nFlags, CPoint point )
{
	m_Edit.ShowWindow( SW_HIDE );

	//for (auto hs : HotSpots)
	//{
	//	if (hs.Rect.PtInRect( point ))
	//	{
	//		CNodeBase* pHitObject = (CNodeBase*)hs.object;
	//		if (hs.Type == HS_CLICK)
	//		{
	//			pHitObject->Update( hs );
	//		}
	//		else if (hs.Type == HS_SELECT)
	//		{
	//			if (nFlags == MK_RBUTTON)
	//			{
	//				ClearSelection( );
	//
	//				pHitObject->Select( );
	//				Selected.push_back( hs );
	//
	//				CRect client;
	//				GetClientRect( &client );
	//				ClientToScreen( &client );
	//
	//				CMenu menu;
	//				menu.LoadMenu( MAKEINTRESOURCE( IDR_MENU_QUICKMODIFY ) );
	//				menu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, client.left + hs.Rect.left + point.x, client.top + point.y, this );
	//			}
	//		}
	//		Invalidate( );
	//	}
	//}

	for (UINT i = 0; i < HotSpots.size( ); i++)
	{
		if (HotSpots[i].Rect.PtInRect( point ))
		{
			CNodeBase* pHitObject = (CNodeBase*)HotSpots[i].object;
			if (HotSpots[i].Type == HS_CLICK)
			{
				pHitObject->Update( HotSpots[i] );
			}
			else if (HotSpots[i].Type == HS_SELECT)
			{
				if (nFlags == MK_RBUTTON)
				{
					ClearSelection( );

					pHitObject->Select( );
					Selected.push_back( HotSpots[i] );

					CRect client;
					GetClientRect( &client );
					ClientToScreen( &client );

					CMenu menu;
					menu.LoadMenu( MAKEINTRESOURCE( IDR_MENU_QUICKMODIFY ) );
					menu.GetSubMenu( 0 )->TrackPopupMenu( TPM_LEFTALIGN | TPM_HORNEGANIMATION, client.left + HotSpots[i].Rect.left + point.x, client.top + point.y, this );
				}
			}
			Invalidate( );
		}
	}

	CWnd::OnRButtonDown( nFlags, point );
}

void CChildView::OnPaint( )
{
	CPaintDC paintDC( this ); // device context for painting
	CMemDC memDC( paintDC, this );
	CDC& dc = memDC.GetDC( );
	DWORD classSize = 0;
	CRect clientRect;
	GetClientRect( &clientRect );
	//clientRect.bottom += g_FontHeight;
	dc.FillSolidRect( &clientRect, g_crBackground );

	if (m_pClass != NULL)
	{
		dc.SelectObject( &g_ViewFont );

		HotSpots.clear( );

		classSize = m_pClass->GetMemorySize( );
		m_Memory.SetSize( classSize );
		ReClassReadMemory( (LPVOID)m_pClass->GetOffset( ), m_Memory.Data( ), classSize );

		ViewInfo View;
		#ifdef _DEBUG
		View.pChildView = this; // For testing
		#endif
		View.Address = m_pClass->GetOffset( );
		View.pData = m_Memory.Data( );
		View.Classes = &g_ReClassApp.m_Classes;
		View.client = &clientRect;
		View.dc = &dc;
		View.Level = 0;
		View.HotSpots = &HotSpots;
		View.bMultiSelected = (Selected.size( ) > 1) ? true : false;

		if (m_Scroll.IsWindowVisible()) {
			View.client->right -= SB_WIDTH;
			View.client->bottom -= SB_WIDTH;
		}

		int ypos = (m_Scroll.GetScrollPos( ) * g_FontHeight);
		int xpos = m_HScroll.GetScrollPos();
		NodeSize DrawMax = m_pClass->Draw( View, 0 - xpos, -ypos );
		DrawMax.y += ypos + g_FontHeight;
		// Dirty hack, fix Draw methods
		DrawMax.x += xpos;

		if (m_pClass->RequestPosition != -1)
		{
			if ((m_pClass->RequestPosition >= 0) && (m_pClass->RequestPosition < (int)g_ReClassApp.m_Classes.size( )))
			{
				int idx = -1;
				for (UINT i = 0; i < g_ReClassApp.m_Classes.size( ); i++)
				{
					CNodeClass* pClass = View.Classes->at( i );
					if (m_pClass == g_ReClassApp.m_Classes[i])
						idx = i;
				}
				g_ReClassApp.m_Classes.erase( g_ReClassApp.m_Classes.begin( ) + idx );
				g_ReClassApp.m_Classes.insert( g_ReClassApp.m_Classes.begin( ) + m_pClass->RequestPosition, m_pClass );
			}
			m_pClass->RequestPosition = -1;
		}

		if (clientRect.Height( ) < DrawMax.y)
		{
			SCROLLINFO si;
			ZeroMemory( &si, sizeof( SCROLLINFO ) );
			si.cbSize = sizeof( SCROLLINFO );
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = DrawMax.y / g_FontHeight;
			si.nPage = clientRect.Height( ) / g_FontHeight;
			m_Scroll.SetScrollInfo( &si );						
			m_Scroll.ShowScrollBar( TRUE );
		}
		else
		{
			m_Scroll.SetScrollPos( 0 );
			m_Scroll.ShowScrollBar( FALSE );
		}

		if (clientRect.Width() < DrawMax.x) {
			SCROLLINFO si = { 0 };
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = DrawMax.x;
			si.nPage = clientRect.Width();
			m_HScroll.SetScrollInfo(&si);			
			m_HScroll.ShowScrollBar(TRUE);
		}
		else {
			m_HScroll.SetScrollPos(0);
			m_HScroll.ShowScrollBar(FALSE);
		}

		CMDIFrameWnd* pFrame = STATIC_DOWNCAST( CMDIFrameWnd, AfxGetApp( )->m_pMainWnd );
		CChildFrame* pChild = STATIC_DOWNCAST( CChildFrame, pFrame->GetActiveFrame( ) );
		if (pChild->m_ChildView.m_hWnd == m_hWnd)
		{
			pChild->SetWindowText( m_pClass->GetName( ) );
			pChild->SetTitle( m_pClass->GetName( ) );
			pFrame->UpdateFrameTitleForDocument( m_pClass->GetName( ) );

			//TCHAR txt[256] = { 0 };
			//_tprintf_s( txt, _T( "Total HotSpots: %i" ), HotSpots.size( ) );
			//dc.SetTextColor( 0x555556 );
			//dc.SetBkColor( 0xFFFFFF );
			//dc.SetBkMode( OPAQUE );
			//dc.DrawText( txt, -1, &CRect( 0, 0, 0, 0 ), DT_LEFT | DT_NOCLIP | DT_NOPREFIX );
		}

		//for (UINT i = 0; i < HotSpots.size();i++)
		//	dc.DrawFocusRect(HotSpots[i].Rect);
	}
}

void CChildView::OnSize( UINT nType, int cx, int cy )
{
	CRect client;
	GetClientRect( &client );
	m_Scroll.SetWindowPos( NULL, client.right - SB_WIDTH, 0, SB_WIDTH, client.Height( ) - SB_WIDTH, SWP_NOZORDER );
	m_HScroll.SetWindowPos(NULL, client.left, client.bottom - SB_WIDTH, client.Width() - SB_WIDTH, SB_WIDTH, SWP_NOZORDER);
	m_Edit.ShowWindow( SW_HIDE );

	CWnd::OnSize( nType, cx, cy );
}

void CChildView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	m_Edit.ShowWindow( SW_HIDE );

	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		pScrollBar->SetScrollPos( nPos );
		Invalidate( );
	}
	else if (nSBCode == SB_LINEUP || nSBCode == SB_LINEDOWN)
	{
		pScrollBar->SetScrollPos( pScrollBar->GetScrollPos( ) + ((nSBCode == SB_LINEUP) ? -1 : 1) );
		Invalidate( );
	}

	CWnd::OnVScroll( nSBCode, nPos, pScrollBar );
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Edit.ShowWindow(SW_HIDE);

	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		pScrollBar->SetScrollPos(nPos);
		Invalidate();
	}
	else if (nSBCode == SB_LINELEFT || nSBCode == SB_LINERIGHT)
	{
		pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + ((nSBCode == SB_LINEUP) ? -1 : 1));
		Invalidate();
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CChildView::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

BOOL CChildView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if (m_Scroll.IsWindowVisible( ))
	{
		if (GetAsyncKeyState( VK_LCONTROL ))
			m_Scroll.SetScrollPos( m_Scroll.GetScrollPos( ) + ((zDelta < 0) ? 1 : -1) );
		else
			m_Scroll.SetScrollPos( m_Scroll.GetScrollPos( ) - ((int)zDelta / g_FontHeight) );
		m_Edit.ShowWindow( SW_HIDE );
		m_ToolTip.ShowWindow( SW_HIDE );
		Invalidate( );
	}

	return CWnd::OnMouseWheel( nFlags, zDelta, pt );
}

void CChildView::OnMouseHover( UINT nFlags, CPoint point )
{
	if (Selected.size( ) > 1)
	{
		CString msg;
		DWORD size = 0;
		for (UINT i = 0; i < Selected.size( ); i++)
			size += Selected[i].object->GetMemorySize( );
		msg.Format( _T( "%i selected, %d bytes" ), Selected.size( ), size );
		m_ToolTip.EnableWindow( FALSE );
		m_ToolTip.SetWindowText( msg );
		m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, msg.GetLength( ) * g_FontWidth + 8, g_FontHeight + 6, SWP_NOZORDER );
		m_ToolTip.ShowWindow( SW_SHOW );
	}
	else
	{
		BYTE data[16];
		for (UINT i = 0; i < HotSpots.size( ); i++)
		{
			if (HotSpots[i].Rect.PtInRect( point ))
			{
				if (HotSpots[i].Type == HS_SELECT)
				{
					CNodeBase* pNode = (CNodeBase*)HotSpots[i].object;
					NodeType nodeType = pNode->GetType( );
					if (nodeType == nt_functionptr)
					{
						if (HotSpots[i].object->IsLevelOpen( HotSpots[i].Level ) == FALSE)
						{
							
							ULONG_PTR StartAddress = HotSpots[i].Address;
							UCHAR Code[1024] = { 0xCC }; // set max function size to 1024] bytes
							UIntPtr EndCode = (UIntPtr)(Code + 1024);
							int textHeight = 0;
							CStringA strDisassembly;

							// Read in process bytes
							if (ReClassReadMemory( (LPVOID)StartAddress, (LPVOID)Code, 1024 ) == TRUE)
							{
								DISASM MyDisasm;
								BOOL Error = FALSE;

								ZeroMemory( &MyDisasm, sizeof( DISASM ) );
								MyDisasm.EIP = (UIntPtr)Code;
								MyDisasm.VirtualAddr = (UInt64)StartAddress;
								#ifdef _WIN64
								MyDisasm.Archi = 64;
								#else
								MyDisasm.Archi = 0;
								#endif
								MyDisasm.Options = MasmSyntax | PrefixedNumeral | ShowSegmentRegs;

								// Get assembly lines
								while (Error == FALSE)
								{
									int disasmLen = 0;

									MyDisasm.SecurityBlock = (UInt32)(EndCode - MyDisasm.EIP);

									disasmLen = Disasm( &MyDisasm );
									if (disasmLen == OUT_OF_BLOCK || disasmLen == UNKNOWN_OPCODE)
									{
										Error = TRUE;
									}
									else
									{
										CHAR szInstruction[256] = { 0 };
										CHAR szBytes[128] = { 0 };

										// INT3 instruction usually indicates the end of a function (obviously this is temporary)
										if (MyDisasm.Instruction.Opcode == 0xCC)
											break;

										// Generate instruction bytes
										for (int i = 0; i < disasmLen; i++)
										{
											sprintf_s( szBytes + (i * 3), 128, "%02X ", *(CHAR*)(MyDisasm.EIP + i) );
										}

										// Create full instruction string
										sprintf_s( szInstruction, 256, "%IX %-*s %s\r\n", (ULONG_PTR)MyDisasm.VirtualAddr, 20 /* change this l8r */, szBytes, MyDisasm.CompleteInstr );
										strDisassembly += szInstruction;

										// Increment the text height
										textHeight += g_FontHeight;

										// Increment by instruction length
										MyDisasm.EIP += disasmLen;
										MyDisasm.VirtualAddr += disasmLen;

										if (MyDisasm.EIP >= EndCode)
											break;
									}
								}
							}
							else
							{
								strDisassembly = "ERROR: Could not read memory";
								textHeight += g_FontHeight;
							}

							m_ToolTip.EnableWindow( FALSE );
							#ifdef UNICODE
							m_ToolTip.SetWindowText( CA2W( strDisassembly ).m_psz );
							#else
							m_ToolTip.SetWindowText( strDisassembly.GetString( ) );
							#endif

							#ifdef _WIN64
							m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 450, textHeight, SWP_NOZORDER );
							#else
							m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 450, textHeight, SWP_NOZORDER );
							#endif

							m_ToolTip.ShowWindow( SW_SHOW );
						}
					}
					else if (nodeType == nt_hex64)
					{
						CString msg;
						ReClassReadMemory( (LPVOID)HotSpots[i].Address, data, sizeof( __int64 ) );
						msg.Format( _T( "Int64: %i\r\nDWORD64: %u\r\nFloat: %.3f" ), *(__int64*)data, *(ULONG64*)data, *(float*)data );
						m_ToolTip.EnableWindow( FALSE );
						m_ToolTip.SetWindowText( msg );
						m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 200, 16 * 3 + 6, SWP_NOZORDER );
						m_ToolTip.ShowWindow( SW_SHOW );
					}
					else if (nodeType == nt_hex32)
					{
						CString msg;
						ReClassReadMemory( (LPVOID)HotSpots[i].Address, data, sizeof( __int32 ) );
						msg.Format( _T( "Int32: %i\r\nDWORD: %u\r\nFloat: %.3f" ), *(int*)data, *(DWORD*)data, *(float*)data );
						m_ToolTip.EnableWindow( FALSE );
						m_ToolTip.SetWindowText( msg );
						m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 200, 16 * 3 + 6, SWP_NOZORDER );
						m_ToolTip.ShowWindow( SW_SHOW );
					}
					else if (nodeType == nt_hex16)
					{
						CString msg;
						ReClassReadMemory( (LPVOID)HotSpots[i].Address, data, sizeof( __int16 ) );
						msg.Format( _T( "Int16: %i\r\nWORD: %u\r\n" ), *(__int16*)data, *(WORD*)data );
						m_ToolTip.EnableWindow( FALSE );
						m_ToolTip.SetWindowText( msg );
						m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 200, 16 * 2 + 6, SWP_NOZORDER );
						m_ToolTip.ShowWindow( SW_SHOW );
					}
					else if (nodeType == nt_hex8)
					{
						CString msg;
						ReClassReadMemory( (LPVOID)HotSpots[i].Address, data, sizeof( __int8 ) );
						msg.Format( _T( "Int8: %i\r\nBYTE: %u\r\n" ), *(__int8*)data, *(UCHAR*)data );
						m_ToolTip.SetWindowText( msg );
						m_ToolTip.SetWindowPos( NULL, point.x + 16, point.y + 16, 200, 16 * 2 + 6, SWP_NOZORDER );
						m_ToolTip.ShowWindow( SW_SHOW );
					}
				}
			}
		}
	}

	m_bTracking = FALSE;
	m_HoverPoint = point;

	CWnd::OnMouseHover( nFlags, point );
}

void CChildView::OnMouseMove( UINT nFlags, CPoint point )
{
	if (point != m_HoverPoint)
		m_ToolTip.ShowWindow( SW_HIDE );

	if (m_bTracking == FALSE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof( tme );
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = HOVER_DEFAULT;
		::TrackMouseEvent( &tme );
		m_bTracking = TRUE;
	}
	CWnd::OnMouseMove( nFlags, point );
}

void CChildView::OnMouseLeave( )
{
	m_ToolTip.ShowWindow( SW_HIDE );
	m_bTracking = FALSE;
	CWnd::OnMouseLeave( );
}

UINT CChildView::FindNodeIndex( CNodeBase* pNode )
{
	CNodeClass* pClass = NULL;
	int foundIdx = -1;

	if (!pNode->GetParent( ))
		return MAX_NODES;
	
	pClass = (CNodeClass*)pNode->GetParent( );
	foundIdx = pClass->FindNode( pNode );
	return (foundIdx != -1) ? foundIdx : MAX_NODES;
}

CNodeBase* CChildView::FindNodeFromIndex( CNodeBase* currentlySelectedNode, UINT index )
{
	CNodeBase* pNode = currentlySelectedNode;
	if (!pNode)
		return NULL;
	CNodeClass* pClass = (CNodeClass*)pNode->GetParent( );
	if (!pClass)
		return NULL;
	if (index >= pClass->NodeCount( ))
		return NULL;
	return pClass->GetNode( index );
}

void CChildView::ReplaceNode( CNodeClass* pClass, UINT idx, CNodeBase* pNewNode )
{
	if (!pClass || idx == MAX_NODES)
		return;

	CNodeBase* pOldNode = pClass->GetNode( idx );
	pNewNode->SetName( pOldNode->GetName( ) );
	pNewNode->SetComment( pOldNode->GetComment( ) );

	// This looks wrong
	//if ( pOldNode->offset < 0x140000000 )
	//{
	//	pNewNode->offset = 0x140000000;
	//	pNewNode->strOffset = "0x140000000";
	//} else {
	//	pNewNode->offset = pOldNode->offset;
	//}

	pNewNode->SetParent( pClass );
	pNewNode->Unselect( );

	//m_pSelected = pNewNode;
	pClass->SetNode( idx, pNewNode );

	DWORD sOld = pOldNode->GetMemorySize( );
	DWORD sNew = pNewNode->GetMemorySize( );

	if (sOld != sNew)
	{
		if (sNew < sOld)
		{
			FillNodes( pClass, idx + 1, sOld - sNew );
		}
		else
		{
			RemoveNodes( pClass, idx + 1, sNew - sOld );
		}
	}

	delete pOldNode;

	g_ReClassApp.CalcAllOffsets( );
}

void CChildView::RemoveNodes( CNodeClass* pClass, UINT idx, DWORD Length )
{
	if (!pClass || idx == MAX_NODES)
		return;

	UINT t = 0;
	DWORD totalSize = 0;
	for (UINT i = idx; i < pClass->NodeCount( ); i++)
	{
		totalSize += pClass->GetNode( i )->GetMemorySize( );
		t++;
		if (totalSize >= Length)
			break;
	}

	for (UINT i = 0; i < t; i++)
	{
		pClass->DeleteNode( idx );
	}

	if (totalSize > Length)
	{
		FillNodes( pClass, idx, totalSize - Length );
	}

	g_ReClassApp.CalcAllOffsets( );
}

void CChildView::FillNodes( CNodeClass* pClass, UINT idx, DWORD Length )
{
	if (!pClass || idx >= MAX_NODES)
		return;

	size_t NewOffset = 0;

	if (idx > 0)
	{
		CNodeBase* pNode = pClass->GetNode( idx - 1 );
		NewOffset = pNode->GetOffset( ) + pNode->GetMemorySize( );
	}

	while (Length != 0)
	{
		// Assume 8?
		// TODO: to figure this out
		if (Length >= 8)
		{
			CNodeHex64* pFill = new CNodeHex64;
			pFill->SetParent( pClass );
			pFill->SetOffset( NewOffset );
			//pFill->Comment.Format("%i-%i",idx,Length);

			pClass->InsertNode( idx, pFill );

			NewOffset += 8;
			Length -= 8;
			idx++;
		}

		if (Length >= 4)
		{
			CNodeHex32* pFill = new CNodeHex32;
			pFill->SetParent( pClass );
			pFill->SetOffset( NewOffset );
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->InsertNode( idx, pFill );
			NewOffset += 4;
			Length -= 4;
			idx++;
		}

		if (Length >= 2 && Length < 4)
		{
			CNodeHex16* pFill = new CNodeHex16;
			pFill->SetParent( pClass );
			pFill->SetOffset( NewOffset );
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->InsertNode( idx, pFill );
			NewOffset += 2;
			Length -= 2;
			idx++;
		}

		if (Length == 1)
		{
			CNodeHex8* pFill = new CNodeHex8;
			pFill->SetParent( pClass );
			pFill->SetOffset( NewOffset );
			//pFill->Comment.Format("%i-%i",idx,Length);
			pClass->InsertNode( idx, pFill );
			NewOffset += 1;
			Length -= 1;
			idx++;
		}
	}
}

void CChildView::ResizeNode( CNodeClass* pClass, UINT idx, DWORD before, DWORD after )
{
	if (!pClass || idx == MAX_NODES)
		return;

	if (before != after)
	{
		if (after < before)
		{
			FillNodes( pClass, idx + 1, before - after );
		}
		else
		{
			RemoveNodes( pClass, idx + 1, after - before );
		}
	}

	g_ReClassApp.CalcAllOffsets( );
}

void CChildView::AddBytes( CNodeClass* pClass, DWORD Length )
{
	if (!pClass)
		return;

	// Ghetto fix for adding 4 bytes in 64 bit
	if (Length == 4)
	{
		CNodeBase* pNode = 0;
		if (pClass->GetType( ) == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex32;
		pNode->SetParent( pClass );
		pClass->AddNode( pNode );
		g_ReClassApp.CalcAllOffsets( );
		return;
	}

	for (UINT i = 0; i < Length / sizeof( ULONG_PTR ); i++)
	{
		CNodeBase* pNode;
		if (pClass->GetType( ) == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex;

		pNode->SetParent( pClass );
		pClass->AddNode( pNode );
	}

	g_ReClassApp.CalcAllOffsets( );
}

void CChildView::InsertBytes( CNodeClass* pClass, UINT idx, DWORD Length )
{
	if (!pClass || idx == MAX_NODES)
		return;

	// Ghetto fix for adding 4 bytes in 64 bit
	if (Length == 4)
	{
		CNodeBase* pNode = 0;
		if (pClass->GetType( ) == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex32;
		pNode->SetParent( pClass );
		pClass->InsertNode( idx, pNode );
		g_ReClassApp.CalcAllOffsets( );
		return;
	}

	for (UINT i = 0; i < Length / sizeof( ULONG_PTR ); i++)
	{
		CNodeBase* pNode;
		if (pClass->GetType( ) == nt_vtable)
			pNode = new CNodeFunctionPtr;
		else
			pNode = new CNodeHex;

		pNode->SetParent( pClass );
		pClass->InsertNode( idx, pNode );
	}

	g_ReClassApp.CalcAllOffsets( );
}

void CChildView::OnAddAdd4( )
{
	if (Selected[0].object->GetType( ) == nt_class)
	{
		AddBytes( (CNodeClass*)Selected[0].object, 4 );
	}
	else
	{
		AddBytes( (CNodeClass*)Selected[0].object->GetParent( ), 4 );
	}

	Invalidate( FALSE );
}

void CChildView::OnUpdateAddAdd4( CCmdUI * pCmdUI )
{
	if (Selected.size( ) == 1 && (Selected[0].object->GetParent( ) || (Selected[0].object->GetType( ) == nt_class)))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnAddAdd8( )
{
	if (Selected[0].object->GetType( ) == nt_class)
		AddBytes( (CNodeClass*)Selected[0].object, 8 );
	else
		AddBytes( (CNodeClass*)Selected[0].object->GetParent( ), 8 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateAddAdd8( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && (Selected[0].object->GetParent( ) || (Selected[0].object->GetType( ) == nt_class)))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnAddAdd64( )
{
	if (Selected[0].object->GetType( ) == nt_class)
		AddBytes( (CNodeClass*)Selected[0].object, 64 );
	else
		AddBytes( (CNodeClass*)Selected[0].object->GetParent( ), 64 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateAddAdd64( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && (Selected[0].object->GetParent( ) || (Selected[0].object->GetType( ) == nt_class)))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnAddAdd1024( )
{
	if (Selected[0].object->GetType( ) == nt_class)
		AddBytes( (CNodeClass*)Selected[0].object, 1024 );
	else
		AddBytes( (CNodeClass*)Selected[0].object->GetParent( ), 1024 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateAddAdd1024( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && (Selected[0].object->GetParent( ) || (Selected[0].object->GetType( ) == nt_class)))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnAddAdd2048( )
{
	if (Selected[0].object->GetType( ) == nt_class)
		AddBytes( (CNodeClass*)Selected[0].object, 2048 );
	else
		AddBytes( (CNodeClass*)Selected[0].object->GetParent( ), 2048 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateAddAdd2048( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && (Selected[0].object->GetParent( ) || (Selected[0].object->GetType( ) == nt_class)))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert4( )
{
	InsertBytes( (CNodeClass*)Selected[0].object->GetParent( ), FindNodeIndex( Selected[0].object ), 4 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateInsertInsert4( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->GetParent( ))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert8( )
{
	InsertBytes( (CNodeClass*)Selected[0].object->GetParent( ), FindNodeIndex( Selected[0].object ), 8 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateInsertInsert8( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->GetParent( ))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert64( )
{
	InsertBytes( (CNodeClass*)Selected[0].object->GetParent( ), FindNodeIndex( Selected[0].object ), 64 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateInsertInsert64( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->GetParent( ))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert1024( )
{
	InsertBytes( (CNodeClass*)Selected[0].object->GetParent( ), FindNodeIndex( Selected[0].object ), 1024 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateInsertInsert1024( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->GetParent( ))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CChildView::OnInsertInsert2048( )
{
	InsertBytes( (CNodeClass*)Selected[0].object->GetParent( ), FindNodeIndex( Selected[0].object ), 2048 );
	Invalidate( FALSE );
}

void CChildView::OnUpdateInsertInsert2048( CCmdUI *pCmdUI )
{
	if (Selected.size( ) == 1 && Selected[0].object->GetParent( ))
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void MakeBasicClass( CNodeClass* pClass )
{
	for (int i = 0; i < 1/*64/8*/; i++)
	{
		CNodeHex* pNode = new CNodeHex( );
		pNode->SetParent( pClass );
		pClass->AddNode( pNode );
	}

	g_ReClassApp.CalcOffsets( pClass );
	g_ReClassApp.m_Classes.push_back( pClass );
}

void CChildView::ReplaceSelectedWithType( NodeType Type )
{
	std::vector<CNodeBase*> newSelected;

	PrintOut( _T( "Replace Node Type %s" ), NodeTypeToString( Type ) );

	for (UINT i = 0; i < Selected.size( ); i++)
	{
		if (!g_ReClassApp.IsNodeValid( Selected[i].object ))
			continue;

		if (Selected[i].object->GetParent( )->GetType( ) == nt_vtable)
		{
			Type = nt_functionptr;
		}

		CNodeBase* pNewNode = g_ReClassApp.CreateNewNode( Type );

		if (Type == nt_class)
		{
			MakeBasicClass( (CNodeClass*)pNewNode );
		}
		if (Type == nt_custom)
		{
			((CNodeCustom*)pNewNode)->SetSize( Selected[i].object->GetMemorySize( ) );
		}
		if (Type == nt_text)
		{
			((CNodeText*)pNewNode)->SetSize( Selected[i].object->GetMemorySize( ) );
		}
		if (Type == nt_unicode)
		{
			((CNodeUnicode*)pNewNode)->SetSize( Selected[i].object->GetMemorySize( ) );
		}
		if (Type == nt_vtable)
		{
			for (int i = 0; i < 10; i++)
			{
				CNodeVTable* pVTable = (CNodeVTable*)pNewNode;
				CNodeFunctionPtr* pFun = new CNodeFunctionPtr;
				pFun->SetOffset( i * sizeof( size_t ) );
				pFun->SetParent( pVTable );
				pVTable->AddNode( pFun );
			}
		}
		if (Type == nt_pointer)
		{
			CNodePtr*	pPtr = (CNodePtr*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)g_ReClassApp.CreateNewNode( nt_class );
			MakeBasicClass( pClass );
			pPtr->SetClass( pClass );
		}
		if (Type == nt_array)
		{
			CNodeArray* pArray = (CNodeArray*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)g_ReClassApp.CreateNewNode( nt_class );
			MakeBasicClass( pClass );
			pArray->SetClass( pClass );
		}
		if (Type == nt_instance)
		{
			CNodeClassInstance* pInstance = (CNodeClassInstance*)pNewNode;
			CNodeClass* pClass = (CNodeClass*)g_ReClassApp.CreateNewNode( nt_class );
			MakeBasicClass( pClass );
			pInstance->SetClass( pClass );
		}
		if (Type == nt_function)
		{
			CNodeFunction* pFunction = (CNodeFunction*)pNewNode;
			pFunction->Initialize( this, Selected[i].object->GetParent( )->GetOffset( ) + Selected[i].object->GetOffset( ) );
		}
		if (Type == nt_functionptr)
		{
			CNodeFunctionPtr* pFunctionPtr = (CNodeFunctionPtr*)pNewNode;
			pFunctionPtr->Initialize( this, Selected[i].object->GetParent( )->GetOffset( ) + Selected[i].object->GetOffset( ) );
		}

		ReplaceNode( (CNodeClass*)Selected[i].object->GetParent( ), FindNodeIndex( Selected[i].object ), pNewNode );
		newSelected.push_back( pNewNode );
	}

	Selected.clear( );
	for (UINT i = 0; i < newSelected.size( ); i++)
	{
		newSelected[i]->Select( );
		CNodeClass* pClass = (CNodeClass*)newSelected[i]->GetParent( );

		HotSpot spot;
		spot.Address = pClass->GetOffset( ) + newSelected[i]->GetOffset( );
		spot.object = newSelected[i];
		Selected.push_back( spot );
	}

	Invalidate( FALSE );
}

void CChildView::ClearSelection( )
{
	for (int i = 0; i < Selected.size( ); i++)
		Selected[i].object->Unselect( );
	Selected.clear( );
}

void CChildView::OnTypeHex64( )
{
	ReplaceSelectedWithType( nt_hex64 );
}

void CChildView::OnUpdateTypeHex64( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeHex32( )
{
	ReplaceSelectedWithType( nt_hex32 );
}

void CChildView::OnUpdateTypeHex32( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeHex16( )
{
	ReplaceSelectedWithType( nt_hex16 );
}

void CChildView::OnUpdateTypeHex16( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeHex8( )
{
	ReplaceSelectedWithType( nt_hex8 );
}

void CChildView::OnUpdateTypeHex8( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeBits( )
{
	ReplaceSelectedWithType( nt_bits );
}

void CChildView::OnUpdateTypeBits( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeInt64( )
{
	ReplaceSelectedWithType( nt_int64 );
}

void CChildView::OnUpdateTypeInt64( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeInt32( )
{
	ReplaceSelectedWithType( nt_int32 );
}

void CChildView::OnUpdateTypeInt32( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeInt16( )
{
	ReplaceSelectedWithType( nt_int16 );
}

void CChildView::OnUpdateTypeInt16( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeInt8( )
{
	ReplaceSelectedWithType( nt_int8 );
}

void CChildView::OnUpdateTypeInt8( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeQword( )
{
	ReplaceSelectedWithType( nt_uint64 );
}

void CChildView::OnUpdateTypeQword( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeDword( )
{
	ReplaceSelectedWithType( nt_uint32 );
}

void CChildView::OnUpdateTypeDword( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeWord( )
{
	ReplaceSelectedWithType( nt_uint16 );
}

void CChildView::OnUpdateTypeWord( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeByte( )
{
	ReplaceSelectedWithType( nt_uint8 );
}

void CChildView::OnUpdateTypeByte( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeVec2( )
{
	ReplaceSelectedWithType( nt_vec2 );
}

void CChildView::OnUpdateTypeVec2( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeVec3( )
{
	ReplaceSelectedWithType( nt_vec3 );
}

void CChildView::OnUpdateTypeVec3( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeQuat( )
{
	ReplaceSelectedWithType( nt_quat );
}

void CChildView::OnUpdateTypeQuat( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeMatrix( )
{
	ReplaceSelectedWithType( nt_matrix );
}

void CChildView::OnUpdateTypeMatrix( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeFloat( )
{
	ReplaceSelectedWithType( nt_float );
}

void CChildView::OnUpdateTypeFloat( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeDouble( )
{
	ReplaceSelectedWithType( nt_double );
}

void CChildView::OnUpdateTypeDouble( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeCustom( )
{
	ReplaceSelectedWithType( nt_custom );
}

void CChildView::OnUpdateTypeCustom( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeUnicode( void )
{
	ReplaceSelectedWithType( nt_unicode );
}

void CChildView::OnUpdateTypeUnicode( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeText( )
{
	ReplaceSelectedWithType( nt_text );
}

void CChildView::OnUpdateTypeText( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypePChar( )
{
	ReplaceSelectedWithType( nt_pchar );
}

void CChildView::OnTypePWChar( )
{
	ReplaceSelectedWithType( nt_pwchar );
}

void CChildView::OnUpdateTypePWChar( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnUpdateTypePChar( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeVtable( )
{
	ReplaceSelectedWithType( nt_vtable );
}

void CChildView::OnUpdateTypeVtable( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeFunction( )
{
	ReplaceSelectedWithType( nt_function );
}

void CChildView::OnUpdateTypeFunction( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeFunctionPtr( )
{
	ReplaceSelectedWithType( nt_functionptr );
}

void CChildView::OnUpdateTypeFunctionPtr( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypePointer( )
{
	ReplaceSelectedWithType( nt_pointer );
}

void CChildView::OnUpdateTypePointer( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeArray( )
{
	ReplaceSelectedWithType( nt_array );
}

void CChildView::OnUpdateTypeArray( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnTypeClass( )
{
	ReplaceSelectedWithType( nt_instance );
}

void CChildView::OnUpdateTypeClass( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnModifyDelete( )
{
	for (UINT i = 0; i < Selected.size( ); i++)
	{
		CNodeClass* pClass = (CNodeClass*)Selected[i].object->GetParent( );
		UINT idx = FindNodeIndex( Selected[i].object );
		if (idx != MAX_NODES)
		{
			pClass->DeleteNode( idx );
			g_ReClassApp.CalcAllOffsets( );
		}
	}
	Selected.clear( );
}

void CChildView::OnUpdateModifyDelete( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnModifyShow( )
{
	g_ReClassApp.ClearHidden( );
}

void CChildView::OnUpdateModifyShow( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnModifyHide( )
{
	for (UINT i = 0; i < Selected.size( ); i++)
		Selected[i].object->Hide( );
	Invalidate( FALSE );
}

void CChildView::OnUpdateModifyHide( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnButtonEditcode( )
{
	CDialogEdit dlg;
	dlg.Title.Format( _T( "Code for %s" ), m_pClass->GetName( ) );
	dlg.Text = m_pClass->Code;
	dlg.DoModal( );
	m_pClass->Code = dlg.Text;
}

BOOL CChildView::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	if (nCode == CN_UPDATE_COMMAND_UI)
	{
		if ((nID >= WM_CHANGECLASSMENU) && (nID < (WM_CHANGECLASSMENU + WM_MAXITEMS)))
		{
			((CCmdUI*)pExtra)->Enable( TRUE );
			return TRUE;
		}
	}
	else if (nCode == CN_COMMAND)
	{
		if ((nID >= WM_CHANGECLASSMENU) && (nID < (WM_CHANGECLASSMENU + WM_MAXITEMS)))
		{
			UINT idx = nID - WM_CHANGECLASSMENU;
			CNodeBase* pNode = ExchangeTarget.object;
			NodeType nodeType = pNode->GetType( );

			if (nodeType == nt_array)
			{
				((CNodeArray*)pNode)->SetClass( g_ReClassApp.m_Classes[idx] );
			}
			else if (nodeType == nt_instance)
			{
				((CNodeClassInstance*)pNode)->SetClass( g_ReClassApp.m_Classes[idx] );
			}
			else if (nodeType == nt_pointer)
			{
				((CNodePtr*)pNode)->SetClass( g_ReClassApp.m_Classes[idx] );
			}

			g_ReClassApp.CalcAllOffsets( );

			return TRUE;
		}
	}
	return CWnd::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

void CChildView::OnEditCopy( )
{
	m_Edit.Copy( );
}

void CChildView::OnEditPaste( )
{
	m_Edit.Paste( );
}

void CChildView::StandardTypeUpdate( CCmdUI *pCmdUI )
{
	if (Selected.size( ) > 0)
	{
		if (Selected[0].object->GetType( ) == nt_class)
			return pCmdUI->Enable( FALSE );
		return pCmdUI->Enable( TRUE );
	}
	pCmdUI->Enable( FALSE );
}

void CChildView::OnButtonZero( )
{
	CMemory mem;
	for (UINT i = 0; i < Selected.size( ); i++)
	{
		DWORD size = Selected[i].object->GetMemorySize( );
		mem.SetSize( size );
		SecureZeroMemory( mem.Data( ), size );
		ReClassWriteMemory( (LPVOID)Selected[i].Address, mem.Data( ), size );
	}
}

void CChildView::OnUpdateButtonZero( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnButtonOne( )
{
	CMemory mem;
	for (UINT i = 0; i < Selected.size( ); i++)
	{
		DWORD size = Selected[i].object->GetMemorySize( );
		mem.SetSize( size );
		memset( mem.Data( ), 1, size );
		ReClassWriteMemory( (LPVOID)Selected[i].Address, mem.Data( ), size );
	}
}

void CChildView::OnUpdateButtonOne( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnButtonRandom( )
{
	CMemory mem;
	srand( GetTickCount( ) );
	for (UINT i = 0; i < Selected.size( ); i++)
	{
		UCHAR* pMemory = NULL;
		DWORD size = Selected[i].object->GetMemorySize( );
		mem.SetSize( size );
		pMemory = mem.Data( );
		for (UINT i = 0; i < size; i++)
			pMemory[i] = rand( );
		ReClassWriteMemory( (LPVOID)Selected[i].Address, pMemory, size );
	}
}

void CChildView::OnUpdateButtonRandom( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}

void CChildView::OnButtonSwap( )
{
	CMemory mem;
	for (UINT i = 0; i < Selected.size( ); i++)
	{
		UCHAR* pMemory = NULL;
		DWORD size = Selected[i].object->GetMemorySize( );
		mem.SetSize( size );
		pMemory = mem.Data( );

		ReClassReadMemory( (LPVOID)Selected[i].Address, pMemory, size );

		std::reverse( pMemory, pMemory + size );

		ReClassWriteMemory( (LPVOID)Selected[i].Address, pMemory, size );
	}
}

void CChildView::OnUpdateButtonSwap( CCmdUI *pCmdUI )
{
	StandardTypeUpdate( pCmdUI );
}
