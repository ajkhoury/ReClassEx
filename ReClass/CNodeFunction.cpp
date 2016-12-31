#include "stdafx.h"
#include "CNodeFunction.h"

static ScintillaColors s_rgbSyntaxAsm[] =
{
	{ SCE_ASM_COMMENT,		green },
	{ SCE_ASM_NUMBER,		darkyellow },
	{ SCE_ASM_STRING,		orange },
	{ SCE_ASM_REGISTER,		lightred },
	{ SCE_ASM_DIRECTIVE,	magenta },
	{ SCE_ASM_OPERATOR,		blue },
	{ SCE_ASM_DIRECTIVE,	purple },
	{ -1,					0 }
};

CNodeFunction::CNodeFunction( ) :
	m_pEdit( NULL ),
	m_nLines( 0 ),
	m_nLongestLine( 0 ),
	m_iWidth( 0 ),
	m_iHeight( 0 ),
	m_bRedrawNeeded( FALSE )
{
	m_nodeType = nt_function;
	m_strName = _T( "" );
	m_dwMemorySize = sizeof( ULONG_PTR );
}

CNodeFunction::~CNodeFunction( )
{
	if (m_pEdit != NULL)
	{
		m_pEdit->Clear( );
		m_pEdit->ShowWindow( SW_HIDE );

		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CNodeFunction::Update( HotSpot& Spot )
{
	StandardUpdate( Spot );

	if (Spot.ID == 0)
	{
		// Re-read bytes at specified address
		DisassembleBytes( Spot.Address );
	}
}

int CNodeFunction::Draw( ViewInfo& View, int x, int y )
{
	if (m_bHidden)
		return DrawHidden( View, x, y );

	AddSelection( View, 0, y, g_FontHeight );
	AddDelete( View, x, y );
	AddTypeDrop( View, x, y );
	//AddAdd(View,x,y);

	int tx = x + TXOFFSET;
	int ax = 0;

	tx = AddIcon( View, tx, y, ICON_METHOD, -1, -1 );
	ax = tx;
	tx = AddAddressOffset( View, tx, y );

	tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "Function" ) );

	tx = AddIcon( View, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK );
	tx += g_FontWidth;

	tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
	tx += g_FontWidth;

	if (m_nLines > 0)
		tx = AddOpenClose( View, tx, y );

	tx += g_FontWidth;

	tx = AddComment( View, tx, y );

	if (m_LevelsOpen[View.Level])
	{
		y += g_FontHeight;

		if (m_bRedrawNeeded)
		{	
			m_pEdit->MoveWindow( ax, y, m_iWidth, m_iHeight );
			m_pEdit->ShowWindow( SW_SHOW );

			m_bRedrawNeeded = FALSE;
		}
		else
		{
			m_pEdit->MoveWindow( ax, y, m_iWidth, m_iHeight );
		}

		y += m_iHeight;
	}
	else
	{
		m_pEdit->ShowWindow( SW_HIDE );
		m_bRedrawNeeded = TRUE;

		y += g_FontHeight;
	}

	return y;
}

void CNodeFunction::Initialize( CChildView* pChild, ULONG_PTR Address )
{
	if (m_pEdit != NULL)
	{
		m_pEdit->Clear( );
		m_pEdit->ShowWindow( SW_HIDE );

		delete m_pEdit;
		m_pEdit = NULL;
	}

	m_pEdit = new CScintillaEdit;

	m_pEdit->Create( WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect( 0, 0, 0, 0 ), (CWnd*)pChild, 0 );

	m_pEdit->ShowWindow( SW_HIDE ); // Hide the window until we open the level
	//m_pEdit->EnableWindow( FALSE ); // Disables the ability to scroll
	m_pEdit->EnableScrollBarCtrl( SB_BOTH, FALSE );

	m_pEdit->SetLexer( SCLEX_ASM );
	m_pEdit->SetStyleBits( 5 );
	m_pEdit->SetTabWidth( 2 );
	m_pEdit->SetForeground( STYLE_DEFAULT, black );
	m_pEdit->SetBackground( STYLE_DEFAULT, g_crBackground );
	m_pEdit->SetSize( STYLE_DEFAULT, FONT_DEFAULT_SIZE );
	m_pEdit->SetHorizontalScrollVisible( FALSE );
	m_pEdit->SetVerticalScrollVisible( FALSE );
	m_pEdit->SetFont( STYLE_DEFAULT, g_ViewFontName );

	m_pEdit->SetAllStylesDefault( );

	// Set syntax colors
	for (int i = 0; s_rgbSyntaxAsm[i].iItem != -1; i++)
		m_pEdit->SetForeground( s_rgbSyntaxAsm[i].iItem, s_rgbSyntaxAsm[i].rgb );

	m_pEdit->SetMarginWidth( 0, 0 );
	m_pEdit->SetMarginWidth( 1, 0 );

	// Finally, disassemble the bytes to get the memsize, height, and width
	DisassembleBytes( Address );
}

void CNodeFunction::DisassembleBytes( ULONG_PTR Address )
{
	ULONG_PTR StartAddress = Address;
	UCHAR Code[2048] = { 0xCC }; // set max function size to 2048 bytes
	UIntPtr EndCode = (UIntPtr)(Code + 2048);
	
	// Clear old disassembly info
	if (m_pEdit)
	{
		m_pEdit->SetReadOnly( FALSE );
		m_pEdit->Clear( );
		m_pEdit->SetReadOnly( TRUE );
	}
	m_Assembly.clear( );
	m_dwMemorySize = 0;
	m_nLongestLine = 0;

	// Read in process bytes
	if (ReClassReadMemory( (LPVOID)StartAddress, (LPVOID)Code, 2048 ) == TRUE)
	{
		DISASM MyDisasm;
		BOOLEAN Error = FALSE;

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
					sprintf_s( szBytes + (i * 3), 4, "%02X ", *(UCHAR*)(MyDisasm.EIP + i) );
				}

				// Create full instruction string
				sprintf_s( szInstruction, 256, "%IX %-*s %s\r\n", (ULONG_PTR)MyDisasm.VirtualAddr, 20 /* change this l8r */, szBytes, MyDisasm.CompleteInstr );
				m_Assembly.emplace_back( szInstruction );

				// Increment instruction length
				m_dwMemorySize += disasmLen;
				MyDisasm.EIP += disasmLen;
				MyDisasm.VirtualAddr += disasmLen;

				if (MyDisasm.EIP >= EndCode)
					break;
			}
		}

		// Get rid of new line on last assembly instruction
		m_Assembly.back( ).Replace( "\r\n", "\0" );
	}
	else
	{
		m_Assembly.emplace_back( "ERROR: Could not read memory" );
		m_dwMemorySize = sizeof( void* );
	}

	// Get number of assembly lines
	m_nLines = (ULONG)m_Assembly.size( );

	// Clear any left over text
	m_pEdit->Clear( );

	// Make the edit window temporarily editable
	m_pEdit->SetReadOnly( FALSE );

	for (ULONG i = 0; i < m_nLines; i++)
	{
		ULONG nCurrentLineLength = 0;

		// Append text to window
		m_pEdit->AppendText( m_Assembly[i].GetString( ) );

		// Calculate width from longest assembly instruction		
		nCurrentLineLength = m_pEdit->LineLength( i );
		if (nCurrentLineLength > m_nLongestLine)
			m_nLongestLine = nCurrentLineLength;
	}

	// Back to read only
	m_pEdit->SetReadOnly( TRUE );

	// Set caret at the beginning of documents
	m_pEdit->SetSelection( 0, 0 );

	// Set the editor width and height
	m_iHeight = (m_pEdit->PointYFromPosition( m_pEdit->PositionFromLine( m_nLines ) ) - m_pEdit->PointYFromPosition( m_pEdit->PositionFromLine( 0 ) )) + g_FontHeight;
	m_iWidth = (m_nLongestLine * g_FontWidth) + g_FontWidth;

	// Force a redraw
	m_bRedrawNeeded = TRUE;
}

