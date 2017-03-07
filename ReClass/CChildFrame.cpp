#include "stdafx.h"

#include "CChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE( CChildFrame, CMDIChildWndEx )

BEGIN_MESSAGE_MAP( CChildFrame, CMDIChildWndEx )
	ON_COMMAND( ID_FILE_CLOSE, &CChildFrame::OnFileClose )
	ON_WM_SETFOCUS( )
	ON_WM_CREATE( )
END_MESSAGE_MAP( )

// CChildFrame construction/destruction

CChildFrame::CChildFrame( )
	: m_pChildView( NULL )
{
}

CChildFrame::~CChildFrame( )
{
	if (m_pChildView != NULL)
	{
		delete m_pChildView;
		m_pChildView = NULL;
	}
}

BOOL CChildFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	//cs.cy += g_FontHeight;

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if (!CMDIChildWndEx::PreCreateWindow( cs ))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( 0 );
	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid( ) const
{
	CMDIChildWndEx::AssertValid( );
}

void CChildFrame::Dump( CDumpContext& dc ) const
{
	CMDIChildWndEx::Dump( dc );
}
#endif //_DEBUG

// CChildFrame message handlers
void CChildFrame::OnFileClose( )
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	SendMessage( WM_CLOSE );
}

int CChildFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	//lpCreateStruct->cy += g_FontHeight;

	if (CMDIChildWndEx::OnCreate( lpCreateStruct ) == -1)
		return -1;

	// Create a view to occupy the client area of the frame
	m_pChildView = new CChildView;
	if (m_pChildView && m_pChildView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 0, 0 ), this, AFX_IDW_PANE_FIRST, NULL ))
		return 0;
	
	TRACE0( "Failed to create view window\n" );
	return -1;
}

void CChildFrame::OnSetFocus( CWnd* pOldWnd )
{
	CMDIChildWndEx::OnSetFocus( pOldWnd );
	m_pChildView->SetFocus( );
}

BOOL CChildFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	// Let the view have first crack at the command
	if (m_pChildView->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ))
		return TRUE;
	// Otherwise, do default handling
	return CMDIChildWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
