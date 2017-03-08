#include "stdafx.h"
#include "CNodeClass.h"
#include "CClassFrame.h"

CNodeClass::CNodeClass( )
{
	TCHAR szOffset[128] = { 0 };

	m_nodeType = nt_class;
	m_Offset = GetBaseAddress( );

	#ifdef _WIN64
	_ui64tot_s( m_Offset, szOffset, 128, 16 );
	#else
	_ultot_s( m_Offset, szOffset, 128, 16 );
	#endif
	m_strOffset.SetString( szOffset );

	RequestPosition = -1;
	Idx = 0;
	pChildWindow = nullptr;
}

void CNodeClass::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID == 0)
	{
		m_strOffset.SetString( Spot.Text.GetString( ) );
		m_Offset = ConvertStrToAddress( m_strOffset );
	}
	else if (Spot.ID == 1)
	{
		RequestPosition = _tcstol( Spot.Text.GetString( ), NULL, 10 ); // RequestPosition = ConvertStrToAddress( Spot.Text );
	}
}

ULONG CNodeClass::GetMemorySize( )
{
	int size = 0;
	for (UINT i = 0; i < m_ChildNodes.size( ); i++)
		size += m_ChildNodes[i]->GetMemorySize( );
	return size;
}

NodeSize CNodeClass::Draw( const ViewInfo& View, int x, int y )
{
	NodeSize drawnSize = { 0 };
	NodeSize childDrawnSize;
	AddSelection( View, x, y, g_FontHeight );
	x = AddOpenClose( View, x, y );

	// Save tx here
	int tx = x;

	x = AddIcon( View, x, y, ICON_CLASS, -1, -1 );
	x = AddText( View, x, y, g_crOffset, 0, _T( "%s" ), m_strOffset ) + g_FontWidth;

	// x += ( NumDigits( m_Offset ) ) * FontWidth;
	// TODO, figure this out better
	// x += ( ( NumDigits( m_Offset ) - 7 ) * FontWidth ) / 2;
	// printf( "Print %s at %d\n", m_strOffset, x );

	x = AddText( View, x, y, g_crIndex, HS_NONE, _T( "(" ) );
	x = AddText( View, x, y, g_crIndex, HS_OPENCLOSE, _T( "%i" ), Idx );
	x = AddText( View, x, y, g_crIndex, HS_NONE, _T( ")" ) );

	x = AddText( View, x, y, g_crType, HS_NONE, _T( "Class " ) );
	x = AddText( View, x, y, g_crName, HS_NAME, m_strName ) + g_FontWidth;
	x = AddText( View, x, y, g_crValue, HS_NONE, _T( "[%i]" ), GetMemorySize( ) ) + g_FontWidth;
	x = AddComment( View, x, y );

	drawnSize.x = x;
	y += g_FontHeight;
	if (m_LevelsOpen[View.Level])
	{
		ViewInfo nv;
		nv = View;
		nv.Level++;

		for (UINT i = 0; i < m_ChildNodes.size( ); i++)
		{
			if (m_ChildNodes[i]->GetType( ) == nt_vtable)
			{
				CNodeVTable* pVTableNode = static_cast<CNodeVTable*>(m_ChildNodes[i]);
				if (!pVTableNode->IsInitialized( ))
				{
					pVTableNode->Initialize( static_cast<CWnd*>(pChildWindow->GetChildView( )) );
					//pVTableNode->Initialize(  );
				}
			}
			childDrawnSize = m_ChildNodes[i]->Draw( nv, tx, y );
			y = childDrawnSize.y;
			if (childDrawnSize.x > drawnSize.x)
			{
				drawnSize.x = childDrawnSize.x;
			}
		}
	}

	drawnSize.y = y;
	return drawnSize;
}
