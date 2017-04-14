#include "stdafx.h"
#include "CNodePtrArray.h"

CNodePtrArray::CNodePtrArray( ) :
	m_PtrCount( 1 ),
	m_CurrentIndex( 0 ),
	m_NodePtr( new CNodePtr )
{ 
	m_nodeType = nt_ptrarray;
}

NodeSize CNodePtrArray::Draw( const ViewInfo& View, int x, int y )
{
	NodeSize draw_size = { 0 }, child_size = { 0 };
	if ( m_bHidden )
		return DrawHidden( View, x, y );
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_ARRAY, -1, -1 );

	int tx = x;
	tx = AddAddressOffset( View, tx, y );

	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "ArrayOfPointers " ) );
	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "[" ) );
	tx = AddText( View, tx, y, g_crIndex, HS_EDIT, _T( "%i" ), m_PtrCount );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "]" ) );

	tx = AddIcon( View, tx, y, ICON_LEFT, HS_SELECT, HS_CLICK );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( "(" ) );
	tx = AddText( View, tx, y, g_crIndex, 1, _T( "%i" ), m_CurrentIndex );
	tx = AddText( View, tx, y, g_crIndex, HS_NONE, _T( ")" ) );
	tx = AddIcon( View, tx, y, ICON_RIGHT, HS_DROP, HS_CLICK );

	tx = AddText( View, tx, y, g_crValue, HS_NONE, _T( "<%s* Size=%i>" ), m_NodePtr->GetClass()->GetName( ), GetMemorySize( ) );
	tx = AddIcon( View, tx, y, ICON_CHANGE, HS_CLICK, HS_CHANGE_X );

	tx += g_FontWidth;
	tx = AddComment( View, tx, y );
	
	y += g_FontHeight;
	if ( m_LevelsOpen[ View.Level ] && IsMemory( View.Address + m_Offset + sizeof( ULONG_PTR ) * m_CurrentIndex ) )
	{
		ULONG class_size = m_NodePtr->GetClass( )->GetMemorySize( );
		m_NodePtr->Memory( )->SetSize( class_size );

		ViewInfo NewView;
		NewView = View;
		NewView.pData = m_NodePtr->Memory( )->Data( );
		NewView.Address = ( (ULONG_PTR*) &View.pData[ m_Offset + sizeof( ULONG_PTR ) * m_CurrentIndex ] )[ 0 ];

		ReClassReadMemory( (LPVOID) NewView.Address, (LPVOID) NewView.pData, class_size );
		
		child_size = m_NodePtr->GetClass( )->Draw( NewView, x, y );
		
		y = child_size.y;
		if ( child_size.x > draw_size.x )
			draw_size.x = child_size.x;
	}

	draw_size.y = y;
	return draw_size;
}

ULONG CNodePtrArray::GetMemorySize( )
{
	return m_NodePtr->GetMemorySize( ) * m_PtrCount;
}

void CNodePtrArray::Update( const HotSpot& Spot )
{
	StandardUpdate( Spot );
	
	int v = _ttoi( Spot.Text.GetString( ) );
	if ( v < 0 ) return;

	if ( Spot.ID == 0 ) {
		if ( v == 0 ) return;
		m_PtrCount = (size_t) v;
	} else if ( Spot.ID == 1 ) {
		if ( v >= (int) m_PtrCount ) return;
		m_CurrentIndex = (int) v;
	} else if ( Spot.ID == 2 ) {
		if ( m_CurrentIndex > 0 )
			m_CurrentIndex--;
	} else if ( Spot.ID == 3 ) {
		if ( (size_t) m_CurrentIndex < m_PtrCount - 1 )
			m_CurrentIndex++;
	}
}

