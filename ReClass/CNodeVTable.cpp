#include "stdafx.h"
#include "CNodeVTable.h"

CNodeVTable::CNodeVTable( )
	: m_pParentWindow( nullptr )
{
	m_nodeType = nt_vtable;
}

CNodeVTable::CNodeVTable( CWnd* pParentWindow )
	: CNodeVTable( )
{
	m_pParentWindow = pParentWindow;
}

void CNodeVTable::Update( const PHOTSPOT Spot )
{
	StandardUpdate( Spot );
}

NODESIZE CNodeVTable::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;
    NODESIZE ChildDrawSize;
    ULONG_PTR* Data;

	if (m_bHidden)
		return DrawHidden( View, x, y );

    Data = (ULONG_PTR*)(View->Data + m_Offset);
	
	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );

	x = AddOpenClose( View, x, y );
	x = AddIcon( View, x, y, ICON_VTABLE, -1, -1 );

	int tx = x;
	x = AddAddressOffset( View, x, y );
	x = AddText( View, x, y, g_crVTable, HS_NONE, _T( "VTable[%i]" ), m_ChildNodes.size( ) ) + g_FontWidth;

	//if (m_strName.IsEmpty())
	x = AddText( View, x, y, g_crName, HS_NAME, _T( "%s" ), m_strName ) + g_FontWidth;
	//else
	//	x = AddText(View, x, y, g_crName, HS_NONE, _T("%s_vtable"), pParent->m_strName) + FontWidth;

	x = AddComment( View, x, y );

	y += g_FontHeight;
    DrawSize.x = x;
    DrawSize.y = y;

	if (m_LevelsOpen[View->Level])
	{
		VIEWINFO NewView;

		DWORD NeededSize = (DWORD)m_ChildNodes.size( ) * sizeof( ULONG_PTR );
		m_Memory.SetSize( NeededSize );
		
		memcpy( &NewView, View, sizeof( NewView ) );
		NewView.Data = m_Memory.Data( );
		NewView.Address = *Data;

		ReClassReadMemory( (LPVOID)NewView.Address, NewView.Data, NeededSize );

		for (UINT i = 0; i < m_ChildNodes.size( ); i++)
		{
			CNodeFunctionPtr* FunctionPtrNode = static_cast<CNodeFunctionPtr*>(m_ChildNodes[i]);
			
			if (!FunctionPtrNode->IsInitialized( ))
                FunctionPtrNode->Initialize( m_pParentWindow, i * sizeof( ULONG_PTR ) );
			
            FunctionPtrNode->SetOffset( i * sizeof( ULONG_PTR ) );
			
            ChildDrawSize = FunctionPtrNode->Draw( &NewView, tx, y );
			
            DrawSize.y = ChildDrawSize.y;
			if (ChildDrawSize.x > DrawSize.x)
            {
                DrawSize.x = ChildDrawSize.x;
			}

			y = DrawSize.y;
		}
	}
	else
	{
		for (UINT i = 0; i < m_ChildNodes.size( ); i++)
			static_cast<CNodeFunctionPtr*>(m_ChildNodes[i])->HideAssemblyWindow( );
	}

	return DrawSize;
}

void CNodeVTable::Initialize( CWnd* pParentWindow )
{
	m_pParentWindow = pParentWindow;
}
