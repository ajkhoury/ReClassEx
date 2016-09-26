#include "stdafx.h"
#include "CNodeFunctionPtr.h"

CNodeFunctionPtr::CNodeFunctionPtr( )
{
	m_nodeType = nt_functionptr;
	m_strName = _T( "" );
}

void CNodeFunctionPtr::Update( CHotSpot & Spot )
{
	StandardUpdate( Spot );
	if (Spot.ID == 0)
	{
		Assembly.clear( );

		size_t address = Spot.Address;
		ReClassReadMemory( (LPVOID)address, &address, sizeof( size_t ) );
		char* code[1536]; // max 1536 lines
		ReClassReadMemory( (LPVOID)address, code, 1536 );
		char** EndCodeSection = (code + 1536);

		DISASM MyDisasm;
		ZeroMemory( &MyDisasm, sizeof( DISASM ) );

		MyDisasm.EIP = (size_t)code;

		MyDisasm.VirtualAddr = (UInt64)address;
		#ifdef _WIN64
		MyDisasm.Archi = 64;
		#else
		MyDisasm.Archi = 0;
		#endif
		MyDisasm.Options = PrefixedNumeral;

		bool Error = 0;
		while (!Error)
		{
			MyDisasm.SecurityBlock = (UInt32)((size_t)EndCodeSection - (size_t)MyDisasm.EIP);

			int len = Disasm( &MyDisasm );
			if (len == OUT_OF_BLOCK)
				Error = 1;
			else if (len == UNKNOWN_OPCODE)
				Error = 1;
			else
			{
				char szInstruction[96];
				sprintf_s( szInstruction, "%p  %s", (void*)MyDisasm.VirtualAddr, MyDisasm.CompleteInstr );
				Assembly.emplace_back( szInstruction );

				MyDisasm.EIP = MyDisasm.EIP + len;
				MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
				if (MyDisasm.EIP >= (UIntPtr)EndCodeSection)
					break;

				if (MyDisasm.Instruction.Opcode == 0xCC) // INT3 instruction
					break;
			}
		}
	}
}

int CNodeFunctionPtr::Draw( ViewInfo & View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	tx = AddIcon( View, tx, y, ICON_METHOD, -1, -1 );
	int ax = tx;
	tx = AddAddressOffset( View, tx, y );

	if (m_pParentNode->GetType( ) != nt_vtable)
		tx = AddText( View, tx, y, crType, HS_NONE, _T( "FunctionPtr" ) );
	else
		tx = AddText( View, tx, y, crFunction, HS_NONE, _T( "(%i)" ), m_Offset / sizeof( size_t ) );

	tx = AddIcon( View, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK );
	tx += g_FontWidth;

	if (m_strName.IsEmpty( ))
		tx = AddText( View, tx, y, crName, HS_NAME, _T( "Function_%i" ), m_Offset / sizeof( size_t ) );
	else
		tx = AddText( View, tx, y, crName, HS_NAME, _T( "%s" ), m_strName );

	tx += g_FontWidth;

	if (Assembly.size( ) > 0)
		tx = AddOpenClose( View, tx, y );

	tx += g_FontWidth;

	tx = AddComment( View, tx, y );

	if (m_LevelsOpen[View.Level])
	{
		for (size_t i = 0; i < Assembly.size( ); i++)
		{
			y += g_FontHeight;
			AddText( View, ax, y, crHex, HS_EDIT, "%s", Assembly[i].GetBuffer( ) );
		}
	}

	return y += g_FontHeight;
}
