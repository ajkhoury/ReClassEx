#include "stdafx.h"

#include "CClassFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE( CClassFrame, CMDIChildWndEx )

BEGIN_MESSAGE_MAP( CClassFrame, CMDIChildWndEx )
	ON_COMMAND( ID_FILE_CLOSE, &CClassFrame::OnFileClose )
	ON_WM_SETFOCUS( )
	ON_WM_CREATE( )
END_MESSAGE_MAP( )

// CChildFrame construction/destruction

CClassFrame::CClassFrame( )
	: m_pClassView( NULL )
{
}

CClassFrame::~CClassFrame( )
{
	if (m_pClassView != NULL)
	{
		delete m_pClassView;
		m_pClassView = NULL;
	}
}

BOOL CClassFrame::PreCreateWindow( CREATESTRUCT& cs )
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
void CClassFrame::AssertValid( ) const
{
	CMDIChildWndEx::AssertValid( );
}

void CClassFrame::Dump( CDumpContext& dc ) const
{
	CMDIChildWndEx::Dump( dc );
}
#endif //_DEBUG

// CChildFrame message handlers
void CClassFrame::OnFileClose( )
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
	SendMessage( WM_CLOSE );
}

int CClassFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	//lpCreateStruct->cy += g_FontHeight;

	if (CMDIChildWndEx::OnCreate( lpCreateStruct ) == -1)
		return -1;

	// Create a view to occupy the client area of the frame
	m_pClassView = new CClassView;
	if (m_pClassView && m_pClassView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 0, 0 ), this, AFX_IDW_PANE_FIRST, NULL ))
		return 0;
	
	TRACE0( "Failed to create view window\n" );
	return -1;
}

void CClassFrame::OnSetFocus( CWnd* pOldWnd )
{
	CMDIChildWndEx::OnSetFocus( pOldWnd );
	m_pClassView->SetFocus( );
}

BOOL CClassFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	// Let the view have first crack at the command
	if (m_pClassView->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ))
		return TRUE;
	// Otherwise, do default handling
	return CMDIChildWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
