#include "stdafx.h"
#include "CNodeFunctionPtr.h"

static SCINTILLA_COLORS s_rgbSyntaxAsm[] = {
    { SCE_ASM_COMMENT,		green },
    { SCE_ASM_NUMBER,		darkyellow },
    { SCE_ASM_STRING,		orange },
    { SCE_ASM_REGISTER,		lightred },
    { SCE_ASM_DIRECTIVE,	magenta },
    { SCE_ASM_OPERATOR,		blue },
    { SCE_ASM_DIRECTIVE,	purple },
    { -1,					0 }
};

CNodeFunctionPtr::CNodeFunctionPtr( )
    : m_pAssemblyWindow( NULL )
    , m_pParentWindow( NULL )
    , m_nLines( 0 )
    , m_nLongestLine( 0 )
    , m_iWidth( 0 )
    , m_iHeight( 0 )
    , m_bRedrawNeeded( FALSE )
{
    m_nodeType = nt_functionptr;
    m_strName = _T( "" );
}

CNodeFunctionPtr::CNodeFunctionPtr( CWnd* pParentWindow, ULONG_PTR Address )
    : CNodeFunctionPtr( )
{
    Initialize( pParentWindow, Address );
}

CNodeFunctionPtr::~CNodeFunctionPtr( )
{
    if (m_pAssemblyWindow != NULL)
    {
        m_pAssemblyWindow->Clear( );
        m_pAssemblyWindow->ShowWindow( SW_HIDE );
        
        delete m_pAssemblyWindow;
        m_pAssemblyWindow = NULL;
    }
}

void CNodeFunctionPtr::Update( const PHOTSPOT Spot )
{
    StandardUpdate( Spot );

    if (Spot->Id == 0)
    {
        // Re-read bytes at specified address
        DisassembleBytes( Spot->Address );
    }
}

NODESIZE CNodeFunctionPtr::Draw( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;
    int tx, ax;

    if (IsHidden( ))
        return DrawHidden( View, x, y );

    AddSelection( View, 0, y, g_FontHeight );
    AddDelete( View, x, y );
    AddTypeDrop( View, x, y );
    //AddAdd(View,x,y);

    tx = x + TXOFFSET;
    tx = AddIcon( View, tx, y, ICON_METHOD, -1, -1 );
    ax = tx;
    tx = AddAddressOffset( View, tx, y );

    if (m_pParentNode->GetType( ) != nt_vtable)
    {
        tx = AddText( View, tx, y, g_crType, HS_NONE, _T( "FunctionPtr" ) );
    }
    else
    {
        tx = AddText( View, tx, y, g_crFunction, HS_NONE, _T( "(%i)" ), m_Offset / sizeof( ULONG_PTR ) );
    }

    tx = AddIcon( View, tx, y, ICON_CAMERA, HS_EDIT, HS_CLICK );
    tx += g_FontWidth;

    if (m_strName.IsEmpty( ))
    {
        tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "Function_%i" ), m_Offset / sizeof( ULONG_PTR ) );
    }
    else
    {
        tx = AddText( View, tx, y, g_crName, HS_NAME, _T( "%s" ), m_strName );
    }

    tx += g_FontWidth;

    if (m_nLines > 0)
        tx = AddOpenClose( View, tx, y );

    tx += g_FontWidth;
    tx = AddComment( View, tx, y );

    if (m_LevelsOpen[View->Level])
    {
        //for (size_t i = 0; i < m_Assembly.size( ); i++)
        //{
        //	y += g_FontHeight;
        //	AddText( View, ax, y, g_crHex, HS_EDIT, "%s", m_Assembly[i].GetBuffer( ) );
        //}

        y += g_FontHeight;

        if (m_pAssemblyWindow != NULL)
        {
            if (m_bRedrawNeeded)
            {
                m_pAssemblyWindow->MoveWindow( ax, y, m_iWidth, m_iHeight );
                m_pAssemblyWindow->ShowWindow( SW_SHOW );

                m_bRedrawNeeded = FALSE;
            }
            else
            {
                m_pAssemblyWindow->MoveWindow( ax, y, m_iWidth, m_iHeight );
            }

            y += m_iHeight;
        }
    }
    else
    {
        if (m_pAssemblyWindow != NULL)
        {
            m_pAssemblyWindow->ShowWindow( SW_HIDE );
            m_bRedrawNeeded = TRUE;
        }

        y += g_FontHeight;
    }

    DrawSize.x = tx;
    DrawSize.y = y;
    return DrawSize;
}

void CNodeFunctionPtr::Initialize( CWnd* pParentWindow, ULONG_PTR Address )
{
    if (m_pAssemblyWindow != NULL)
    {
        m_pAssemblyWindow->Clear( );
        m_pAssemblyWindow->ShowWindow( SW_HIDE );
        delete m_pAssemblyWindow;
    }

    m_pAssemblyWindow = new CScintillaEdit;

    m_pParentWindow = static_cast<CWnd*>(pParentWindow);

    m_pAssemblyWindow->Create( WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect( 0, 0, 0, 0 ), m_pParentWindow, 0 );
    m_pAssemblyWindow->ShowWindow( SW_HIDE ); // Hide the window until we open the level
    //m_pAssemblyWindow->EnableWindow( FALSE ); // Disables the ability to scroll
    m_pAssemblyWindow->EnableScrollBarCtrl( SB_BOTH, FALSE );

    m_pAssemblyWindow->SetLexer( SCLEX_ASM );
    m_pAssemblyWindow->SetStyleBits( 5 );
    m_pAssemblyWindow->SetTabWidth( 2 );
    m_pAssemblyWindow->SetForeground( STYLE_DEFAULT, black );
    m_pAssemblyWindow->SetBackground( STYLE_DEFAULT, g_crBackground );
    m_pAssemblyWindow->SetSize( STYLE_DEFAULT, FONT_DEFAULT_SIZE );
    m_pAssemblyWindow->SetHorizontalScrollVisible( FALSE );
    m_pAssemblyWindow->SetVerticalScrollVisible( FALSE );
    m_pAssemblyWindow->SetFont( STYLE_DEFAULT, g_ViewFontName );

    m_pAssemblyWindow->SetAllStylesDefault( );

    // Set syntax colors
    for (int i = 0; s_rgbSyntaxAsm[i].iItem != -1; i++)
        m_pAssemblyWindow->SetForeground( s_rgbSyntaxAsm[i].iItem, s_rgbSyntaxAsm[i].rgb );

    m_pAssemblyWindow->SetMarginWidth( 0, 0 );
    m_pAssemblyWindow->SetMarginWidth( 1, 0 );

    // Finally, disassemble the bytes to get the memsize, height, and width
    DisassembleBytes( Address );
}

void CNodeFunctionPtr::DisassembleBytes( ULONG_PTR Address )
{
    UCHAR Code[2048] = { 0xCC }; // max function length
    UIntPtr VirtualAddress = Address;

    // Clear old disassembly info
    m_pAssemblyWindow->SetReadOnly( FALSE );
    m_pAssemblyWindow->Clear( );
    m_pAssemblyWindow->SetReadOnly( TRUE );

    m_Assembly.clear( );
    m_nLongestLine = 0;

    // Number of assembly lines
    m_nLines = 0;

    // Read in process bytes
    if (ReClassReadMemory( (LPVOID)VirtualAddress, (LPVOID)&VirtualAddress, sizeof( UIntPtr ) ) && 
        ReClassReadMemory( (LPVOID)VirtualAddress, (LPVOID)Code, 2048 ))
    {
        DISASM MyDisasm;
        BOOL Error = FALSE;
        UIntPtr EndCodeSection = (UIntPtr)(Code + 2048);

        ZeroMemory( &MyDisasm, sizeof( DISASM ) );
        MyDisasm.EIP = (UIntPtr)Code;
        MyDisasm.VirtualAddr = (UInt64)VirtualAddress;
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

            MyDisasm.SecurityBlock = (UInt32)(EndCodeSection - MyDisasm.EIP);

            disasmLen = Disasm( &MyDisasm );
            if (disasmLen == OUT_OF_BLOCK || disasmLen == UNKNOWN_OPCODE)
            {
                Error = TRUE;
            }
            else
            {
                CHAR szInstruction[256] = { 0 };
                CHAR szBytes[128] = { 0 };

                // INT3 instruction usually indicates the end of a function
                if (MyDisasm.Instruction.Opcode == 0xCC)
                    break;

                // Generate instruction bytes
                for (int i = 0; i < disasmLen; i++)
                {
                    CHAR szByte[8];
                    sprintf_s( szByte, "%02X ", *(UCHAR*)(MyDisasm.EIP + i) );
                    strcat_s( szBytes, szByte );
                }

                // Create full instruction string
                sprintf_s( szInstruction, 256, "%IX %-*s %s\r\n", (ULONG_PTR)MyDisasm.VirtualAddr, 20 /* change this l8r */, szBytes, MyDisasm.CompleteInstr );
                m_Assembly.push_back( szInstruction );
                m_nLines++;

                // Increment instruction length
                MyDisasm.EIP = MyDisasm.EIP + disasmLen;
                MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + disasmLen;

                if (MyDisasm.EIP >= (UIntPtr)EndCodeSection)
                    break;
            }
        }

        // Get rid of new line on last assembly instruction
        if (m_nLines > 0)
            m_Assembly.back( ).Replace( "\r\n", "\0" );
    }
    else
    {
        m_Assembly.emplace_back( "ERROR: Could not read memory" );
        m_nLines++;
    }

    // Clear any left over text
    m_pAssemblyWindow->Clear( );

    // Make the edit window temporarily editable
    m_pAssemblyWindow->SetReadOnly( FALSE );

    for (ULONG i = 0; i < m_nLines; i++)
    {
        ULONG nCurrentLineLength = 0;

        // Append text to window
        m_pAssemblyWindow->AppendText( m_Assembly[i].GetString( ) );

        // Calculate width from longest assembly instruction		
        nCurrentLineLength = m_pAssemblyWindow->LineLength( i );
        if (nCurrentLineLength > m_nLongestLine)
            m_nLongestLine = nCurrentLineLength;
    }

    // Back to read only
    m_pAssemblyWindow->SetReadOnly( TRUE );

    // Set caret at the beginning of documents
    m_pAssemblyWindow->SetSelection( 0, 0 );

    // Set the editor width and height
    m_iHeight = (m_pAssemblyWindow->PointYFromPosition( m_pAssemblyWindow->PositionFromLine( m_nLines ) ) - 
        m_pAssemblyWindow->PointYFromPosition( m_pAssemblyWindow->PositionFromLine( 0 ) )) + g_FontHeight;
    m_iWidth = (m_nLongestLine * g_FontWidth) + g_FontWidth;

    // Force a redraw
    m_bRedrawNeeded = TRUE;
}
