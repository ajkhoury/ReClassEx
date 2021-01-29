#include "stdafx.h"

#include "CNodeBase.h"
#include <regex>

CNodeBase::CNodeBase( ) :
    m_nodeType( nt_base ),
    m_pParentNode( nullptr ),
    m_Offset( 0 ),
    m_strOffset( _T( "0" ) ),
    m_bHidden( false ),
    m_bSelected( false )
{
    // Optimized
    m_LevelsOpen.resize( 32, false );
    m_LevelsOpen[0] = true;

    // This is the class name
    m_strName.Format( _T( "N%0.8X" ), g_NodeCreateIndex++ );
}

// Incorrect view.address
void CNodeBase::AddHotSpot( const PVIEWINFO View, const CRect& Spot, CString Text, int ID, int Type )
{
    if (Spot.top > View->ClientRect->bottom || Spot.bottom < 0)
        return;

    HOTSPOT Hotspot;
    Hotspot.Rect = Spot;
    Hotspot.Text = Text.GetString( );
    Hotspot.Address = View->Address + m_Offset;
    Hotspot.Id = ID;
    Hotspot.Type = Type;
    Hotspot.Object = this;
    Hotspot.Level = View->Level;
    View->Hotspots->push_back( Hotspot );
}

int CNodeBase::AddText( const PVIEWINFO View, int x, int y, DWORD color, int HitID, const wchar_t* fmt, ... )
{
    if (fmt == NULL)
        return x;

    wchar_t wcsbuf[1024] = { 0 };

    va_list va_alist;
    va_start( va_alist, fmt );
    _vsnwprintf_s( wcsbuf, ARRAYSIZE( wcsbuf ), fmt, va_alist );
    va_end( va_alist );

    int width = (int)(wcslen( wcsbuf )) * g_FontWidth;

    if ((y >= -(g_FontHeight)) && (y + g_FontHeight <= View->ClientRect->bottom + g_FontHeight))
    {
        CRect pos;
        if (HitID != HS_NONE)
        {
            if (width >= g_FontWidth * 2)
                pos.SetRect( x, y, x + width, y + g_FontHeight );
            else
                pos.SetRect( x, y, x + g_FontWidth * 2, y + g_FontHeight );

            AddHotSpot( View, pos, wcsbuf, HitID, HS_EDIT );
        }

        pos.SetRect( x, y, 0, 0 );

        View->Dc->SetTextColor( color );
        View->Dc->SetBkMode( TRANSPARENT );
        View->Dc->DrawText( wcsbuf, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX );
    }
    
    return x + width;
}

int CNodeBase::AddText( const PVIEWINFO View, int x, int y, DWORD color, int HitID, const char* fmt, ... )
{
    char buffer[1024] = { 0 };
    TCHAR finalBuffer[1024] = { 0 };

    if (fmt == NULL)
        return x;

    va_list va_alist;
    va_start( va_alist, fmt );
    _vsnprintf_s( buffer, 1024, fmt, va_alist );
    va_end( va_alist );

    #ifdef UNICODE
    size_t converted = 0; 
    mbstowcs_s( &converted, finalBuffer, buffer, 1024 );
    #else
    memcpy( &finalBuffer, buffer, 1024 );
    #endif

    int width = static_cast<int>(strlen( buffer )) * g_FontWidth;

    if ((y >= -g_FontHeight) && (y + g_FontHeight <= View->ClientRect->bottom + g_FontHeight))
    {
        CRect pos;
        if (HitID != HS_NONE)
        {
            if (width >= g_FontWidth * 2)
                pos.SetRect( x, y, x + width, y + g_FontHeight );
            else
                pos.SetRect( x, y, x + g_FontWidth * 2, y + g_FontHeight );

            AddHotSpot( View, pos, finalBuffer, HitID, HS_EDIT );
        }

        pos.SetRect( x, y, 0, 0 );

        View->Dc->SetTextColor( color );
        View->Dc->SetBkMode( TRANSPARENT );
        View->Dc->DrawText( finalBuffer, pos, DT_LEFT | DT_NOCLIP | DT_NOPREFIX );
    }

    return x + width;
}

int CNodeBase::AddAddressOffset( const PVIEWINFO View, int x, int y )
{
    if (g_bOffset)
    {
        #ifdef _WIN64
        // goto 722
        // just the left side 0000
        // TODO: fix the ghetto rig FontWidth * x
        // where x = characters over 8
        //x += FontWidth; // we need this either way
        //int numdigits = Utils::NumDigits(View->Address);
        //if (numdigits < 8 && numdigits > 4)
        //	x -= ((8 - numdigits) * FontWidth);
        //if (numdigits > 8)
        //	x += ((numdigits - 8) * FontWidth);

        x = AddText( View, x, y, g_clrOffset, HS_NONE, _T( "%0.4X" ), m_Offset ) + g_FontWidth;
        #else
        x = AddText( View, x, y, g_clrOffset, HS_NONE, _T( "%0.4X" ), m_Offset ) + g_FontWidth;
        #endif
    }

    if (g_bAddress)
    {
        #ifdef _WIN64
        x = AddText( View, x, y, g_clrAddress, HS_ADDRESS, _T( "%0.9I64X" ), View->Address + m_Offset ) + g_FontWidth;
        #else
        x = AddText( View, x, y, g_clrAddress, HS_ADDRESS, _T( "%0.8X" ), View->Address + m_Offset ) + g_FontWidth;
        #endif
    }

    return x;
}

void CNodeBase::AddSelection( const PVIEWINFO View, int x, int y, int Height )
{
    if ((y > View->ClientRect->bottom) || (y + Height < 0))
        return;

    if (m_bSelected)
        View->Dc->FillSolidRect( 0, y, View->ClientRect->right, Height, g_clrSelect );

    CRect pos( 0, y, INT_MAX, y + Height );
    AddHotSpot( View, pos, CString( ), 0, HS_SELECT );
}

int CNodeBase::AddIcon( const PVIEWINFO View, int x, int y, int idx, int ID, int Type )
{
    if ((y > View->ClientRect->bottom) || (y + 16 < 0))
        return x + 16;

    DrawIconEx( View->Dc->m_hDC, x, y, g_Icons[idx], 16, 16, 0, NULL, DI_NORMAL );

    if (ID != -1)
    {
        CRect pos( x, y, x + 16, y + 16 );
        AddHotSpot( View, pos, CString( ), ID, Type );
    }

    return x + 16;
}

int CNodeBase::AddOpenClose( const PVIEWINFO View, int x, int y )
{
    if ((y > View->ClientRect->bottom) || (y + 16 < 0))
        return x + 16;
    return m_LevelsOpen[View->Level] ? AddIcon( View, x, y, ICON_OPEN, 0, HS_OPENCLOSE ) : AddIcon( View, x, y, ICON_CLOSED, 0, HS_OPENCLOSE );
}

void CNodeBase::AddDelete( const PVIEWINFO View, int x, int y )
{
    if ((y > View->ClientRect->bottom) || (y + 16 < 0))
        return;

    if (m_bSelected)
        AddIcon( View, View->ClientRect->right - 16, y, ICON_DELETE, 0, HS_DELETE );
}

//void CNodeBase::AddAdd(const PVIEWINFO View, int x, int y)
//{
//	if ( (y > View->ClientRect->bottom) || (y+16 < 0) ) return;
//	if (m_bSelected)AddIcon(View,16,y,ICON_ADD,HS_NONE,HS_NONE);
//}

void CNodeBase::AddTypeDrop( const PVIEWINFO View, int x, int y )
{
    if (View->MultiSelected || ((y > View->ClientRect->bottom) || (y + 16 < 0)))
        return;

    if (m_bSelected)
        AddIcon( View, 0, y, ICON_DROPARROW, 0, HS_DROP );
}

static CString remangle(const CString& name) {
	CString manglemore = "??_7" + name.Mid(3) + "6B@";
    return manglemore;
}

int CNodeBase::ResolveRTTI( ULONG_PTR Address, int x, const PVIEWINFO View, int y )
{
#if defined(_M_AMD64)
    ULONG_PTR ModuleBase = 0;

    ULONG_PTR RTTIObjectLocatorPtr = 0;
    ULONG_PTR RTTIObjectLocator = 0;

    ULONG ClassHierarchyDescriptorOffset = 0;
    ULONG_PTR ClassHierarchyDescriptor = 0;

    ULONG NumBaseClasses = 0;

    DWORD BaseClassArrayOffset = 0;
    ULONG_PTR BaseClassArray = 0;

    CString RttiString;

    // Get module base that this address falls in
    int indirections = -1;
    while (++indirections < 8 && !((ModuleBase = GetModuleBaseFromAddress(Address)))) {
        uintptr_t nextAddress = 0;
        ReClassReadMemory((LPVOID)Address, &nextAddress, sizeof(ULONG_PTR));
        Address = nextAddress;
    }

    RTTIObjectLocatorPtr = Address - sizeof( ULONG64 ); // Address is Ptr to first VFunc, pRTTI is at -0x8
    if (!IsValidPtr( RTTIObjectLocatorPtr ))
        return x;

    ReClassReadMemory( (LPVOID)RTTIObjectLocatorPtr, &RTTIObjectLocator, sizeof( ULONG_PTR ) );

    ReClassReadMemory( (LPVOID)(RTTIObjectLocator + 0x10), &ClassHierarchyDescriptorOffset, sizeof( ULONG ) );

    ClassHierarchyDescriptor = ModuleBase + ClassHierarchyDescriptorOffset;
    if (!IsValidPtr( ClassHierarchyDescriptor ) || !ClassHierarchyDescriptorOffset)
        return x;

    ReClassReadMemory( (LPVOID)(ClassHierarchyDescriptor + 0x8), &NumBaseClasses, sizeof( ULONG ) );
    if (NumBaseClasses < 0 || NumBaseClasses > 25)
        NumBaseClasses = 0;

    ReClassReadMemory( (LPVOID)(ClassHierarchyDescriptor + 0xC), &BaseClassArrayOffset, sizeof( ULONG ) );

    BaseClassArray = ModuleBase + BaseClassArrayOffset;
    if (!IsValidPtr( BaseClassArray ) || !BaseClassArrayOffset)
        return x;

    if (!indirections)
        RttiString += "vtable for ";
	else for (int i=0; i<indirections; ++i)
        RttiString += "pointer to ";

    for (ULONG i = 0; i < NumBaseClasses; i++)
    {
        ULONG BaseClassDescriptorOffset = 0;
        ULONG_PTR BaseClassDescriptor = 0;

        ULONG TypeDescriptorOffset = 0;
        ULONG_PTR TypeDescriptor = 0;

        if (i == 1)
            RttiString += TEXT( ": " ); // Base class
        else if (i > 1)
            RttiString += TEXT( ", " ); // Parent classes


        ReClassReadMemory( (LPVOID)(BaseClassArray + (sizeof( ULONG ) * i)), &BaseClassDescriptorOffset, sizeof( ULONG ) );

        BaseClassDescriptor = ModuleBase + BaseClassDescriptorOffset;
        if (!IsValidPtr( BaseClassDescriptor ) || !BaseClassDescriptorOffset)
            continue;

        ReClassReadMemory( (LPVOID)BaseClassDescriptor, &TypeDescriptorOffset, sizeof( ULONG ) );

        TypeDescriptor = ModuleBase + TypeDescriptorOffset;
        if (!IsValidPtr( TypeDescriptor ) || !TypeDescriptorOffset)
            continue;

        CString RTTIName;
        BOOLEAN FoundEnd = FALSE;
        //CHAR LastChar = ' ';
        for (int j = 1; j < 512; j++)
        {
            CHAR RTTINameChar;
            ReClassReadMemory( (LPVOID)(TypeDescriptor + 0x10 + j), &RTTINameChar, 1 );

            if (!RTTINameChar) {
                if (RTTIName.Right(2) == "@@") FoundEnd = TRUE;
                break;
            }

            RTTIName += RTTINameChar;
        }

        // Did we find a valid RTTI name or did we just reach end of loop
        if (FoundEnd == TRUE)
        {
            TCHAR Demangled[MAX_PATH] = { 0 };
            if (_UnDecorateSymbolName(remangle(RTTIName), Demangled, MAX_PATH, UNDNAME_NAME_ONLY) != 0)
            {
                CString PostProcessing(Demangled);
                PostProcessing.Replace(L"::`vftable'", L"");
                RttiString += PostProcessing;
            }
            else
            {
                RttiString += RTTIName;
            }
        }
    }
#else   
    ULONG_PTR RTTIObjectLocatorPtr = 0;
    ULONG_PTR RTTIObjectLocator = 0;

    ULONG_PTR ClassHierarchyDescriptorPtr = 0;
    ULONG_PTR ClassHierarchyDescriptor = 0;

    ULONG NumBaseClasses = 0;

    ULONG_PTR BaseClassArrayPtr = 0;
    ULONG_PTR BaseClassArray = 0;

    CString RttiString;

    RTTIObjectLocatorPtr = Address - sizeof( ULONG );
    if (!IsValidPtr( RTTIObjectLocatorPtr ))
        return x;

    ReClassReadMemory( (LPVOID)RTTIObjectLocatorPtr, &RTTIObjectLocator, sizeof( ULONG_PTR ) );

    ClassHierarchyDescriptorPtr = RTTIObjectLocator + 0x10;
    if (!IsValidPtr( ClassHierarchyDescriptorPtr ))
        return x;

    ReClassReadMemory( (LPVOID)ClassHierarchyDescriptorPtr, &ClassHierarchyDescriptor, sizeof( ULONG_PTR ) );

    ReClassReadMemory( (LPVOID)(ClassHierarchyDescriptor + 0x8), &NumBaseClasses, sizeof( ULONG ) );
    if (NumBaseClasses < 0 || NumBaseClasses > 25)
        NumBaseClasses = 0;

    BaseClassArrayPtr = ClassHierarchyDescriptor + 0xC;
    if (!IsValidPtr( BaseClassArrayPtr ))
        return x;

    ReClassReadMemory( (LPVOID)BaseClassArrayPtr, &BaseClassArray, sizeof( ULONG_PTR ) );
    
    for (ULONG i = 0; i < NumBaseClasses; i++)
    {
        ULONG_PTR BaseClassDescriptorPtr = 0;
        ULONG_PTR BaseClassDescriptor = 0;

        ULONG_PTR TypeDescriptor = 0; // Ptr at 0x00 in BaseClassDescriptor

        if (i != 0 && i != NumBaseClasses)
        {
            RttiString += " : "; // Base class
        }

        BaseClassDescriptorPtr = BaseClassArray + (4 * i);
        if (!IsValidPtr( BaseClassDescriptorPtr ))
            continue;

        ReClassReadMemory( (LPVOID)BaseClassDescriptorPtr, &BaseClassDescriptor, sizeof( ULONG_PTR ) );
        if (!IsValidPtr( BaseClassDescriptor ))
            continue;

        ReClassReadMemory( (LPVOID)BaseClassDescriptor, &TypeDescriptor, sizeof( ULONG_PTR ) );

        CString RTTIName;
        BOOLEAN FoundEnd = FALSE;
        CHAR LastChar = ' ';

        for (int j = 1; j < 45; j++)
        {
            CHAR RTTINameChar;
            ReClassReadMemory( (LPVOID)(TypeDescriptor + 0x08 + j), &RTTINameChar, 1 );
            if (RTTINameChar == '@' && LastChar == '@') // Names seem to be ended with @@
            {
                FoundEnd = TRUE;
                RTTIName += RTTINameChar;
                break;
            }

            RTTIName += RTTINameChar;
            LastChar = RTTINameChar;
        }

        // Did we find a valid RTTI name or did we just reach end of loop
        if (FoundEnd == TRUE)
        {
            TCHAR Demangled[256] = { 0 };

            if (_UnDecorateSymbolName( RTTIName.GetString( ), Demangled, 256, UNDNAME_NAME_ONLY ) == 0)
            {
                RttiString += RTTIName.GetString( );
            }
            else
            {
                RttiString += Demangled;
            }
        }
    }
#endif

    x = AddText( View, x, y, g_clrOffset, HS_RTTI, _T( "%s" ), RttiString.GetString( ) );
    
    return x;
}

int CNodeBase::AddComment( const PVIEWINFO View, int x, int y )
{
    x = AddText( View, x, y, g_clrComment, HS_NONE, _T( "//" ) );
    // Need the extra whitespace in "%s " after the %s to edit.
    x = AddText( View, x, y, g_clrComment, HS_COMMENT, _T( "%s " ), m_strComment );

    if (m_nodeType == nt_hex64)
    {
        float flVal = *(float*)&View->Data[m_Offset];
        LONG_PTR intVal = *(LONG_PTR*)&View->Data[m_Offset];

        if (g_bFloat)
        {
            if (flVal > -99999.0 && flVal < 99999.0)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%0.3f)" ), flVal );
            else
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%0.3f)" ), 0.0f );
        }

        if (g_bInt)
        {
            if (intVal > 0x6FFFFFFF && intVal < 0x7FFFFFFFFFFF)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%I64d|0x%IX)" ), intVal, intVal );
            else if (intVal == 0)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%I64d)" ), intVal );
            else
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%I64d|0x%X)" ), intVal, intVal );
        }

        // *** this is probably broken, let's fix it after
        ULONG_PTR uintVal = (ULONG_PTR)intVal;
        CString strAddress( GetAddressName( uintVal, FALSE ) );
        if (strAddress.GetLength( ) > 0)
        {
            if (g_bPointers)
            {
                //printf( "<%p> here\n", Val );
                if (uintVal > 0x6FFFFFFF && uintVal < 0x7FFFFFFFFFFF)
                {
                    x = AddText( View, x, y, g_clrOffset, HS_EDIT, _T( "*->%s " ), strAddress.GetString( ) );
                    if (g_bRTTI)
                        x = ResolveRTTI( uintVal, x, View, y );

                    // Print out info from PDB at address
                    if (g_bSymbolResolution)
                    {
                        ULONG_PTR ModuleAddress = 0;
                        SymbolReader* pSymbols = NULL;

                        ModuleAddress = GetModuleBaseFromAddress( uintVal );
                        if (ModuleAddress != 0)
                        {
                            pSymbols = g_ReClassApp.m_pSymbolLoader->GetSymbolsForModuleAddress( ModuleAddress );
                            if (!pSymbols)
                            {
                                CString moduleName = GetModuleName( uintVal );
                                if (!moduleName.IsEmpty( ))
                                    pSymbols = g_ReClassApp.m_pSymbolLoader->GetSymbolsForModuleName( moduleName );
                            }

                            if (pSymbols != NULL)
                            {
                                CString SymbolOut;
                                SymbolOut.Preallocate( 1024 );
                                if (pSymbols->GetSymbolStringFromVA( uintVal, SymbolOut ))
                                {
                                    x = AddText( View, x, y, g_clrOffset, HS_EDIT, _T( "%s " ), SymbolOut.GetString( ) );
                                }
                            }
                        }
                    }
                }
            }

            if (g_bString)
            {
                bool bAddStr = true;
                char txt[64];
                ReClassReadMemory( (LPVOID)uintVal, txt, 64 );

                for (int i = 0; i < 8; i++)
                {
                    if (!(txt[i] > 0x1F && txt[i] < 0xFF && txt[i] != 0x7F))
                        bAddStr = false;
                }

                if (bAddStr)
                {
                    txt[63] = '\0';
                    x = AddText( View, x, y, g_clrChar, HS_NONE, _T( "'%hs'" ), txt );
                }
            }
        }
    }
    else if (m_nodeType == nt_hex32)
    {
        float flVal = *((float*)&View->Data[m_Offset]);
        int intVal = *((int*)&View->Data[m_Offset]);

        if (g_bFloat)
        {
            if (flVal > -99999.0 && flVal < 99999.0)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%0.3f)" ), flVal );
            else
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%0.3f)" ), 0.0f );
        }

        if (g_bInt)
        {
            #if defined(_M_AMD64)
            if (intVal > 0x140000000 && intVal < 0x7FFFFFFFFFFF) // in 64 bit address range
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i|0x%IX)" ), intVal, intVal );
            else if (intVal > 0x400000 && intVal < 0x140000000)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i|0x%X)" ), intVal, intVal );
            else if (intVal == 0)
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i)" ), intVal );
            else
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i|0x%X)" ), intVal, intVal );
            #else
            x = (intVal == 0) ? AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i)" ), intVal ) : AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i|0x%X)" ), intVal, intVal );
            #endif
        }

        // *** this is probably broken, let's fix it after
        size_t uintVal = (size_t)intVal;
        CString strAddress( GetAddressName( uintVal, FALSE ) );
        if (strAddress.GetLength( ) > 0)
        {
            if (g_bPointers)
            {
                //#ifdef _WIN64
                // If set max to 0x140000000 a bunch of invalid pointers come up
                // Set to 0x110000000 instead
                if (uintVal > 0x400000 && uintVal < 0x110000000)
                {
                    x = AddText( View, x, y, g_clrOffset, HS_EDIT, _T( "*->%s " ), strAddress.GetString( ) );
                    if (g_bRTTI)
                        x = ResolveRTTI( uintVal, x, View, y );

                    // Print out info from PDB at address
                    if (g_bSymbolResolution)
                    {
                        ULONG_PTR ModuleAddress = 0;
                        SymbolReader* pSymbols = NULL;

                        ModuleAddress = GetModuleBaseFromAddress( uintVal );
                        if (ModuleAddress != 0)
                        {
                            pSymbols = g_ReClassApp.m_pSymbolLoader->GetSymbolsForModuleAddress( ModuleAddress );
                            if (!pSymbols)
                            {
                                CString ModuleName = GetModuleName( uintVal );
                                if (!ModuleName.IsEmpty( ))
                                    pSymbols = g_ReClassApp.m_pSymbolLoader->GetSymbolsForModuleName( ModuleName );
                            }

                            if (pSymbols != NULL)
                            {
                                CString SymbolOut;
                                SymbolOut.Preallocate( 1024 );
                                if (pSymbols->GetSymbolStringFromVA( uintVal, SymbolOut ))
                                {
                                    x = AddText( View, x, y, g_clrOffset, HS_EDIT, _T( "%s " ), SymbolOut.GetString( ) );
                                }
                            }
                        }
                    }
                }
            }

            if (g_bString)
            {
                bool bAddStr = true;
                char txt[64] = { 0 };
                ReClassReadMemory( (LPVOID)uintVal, txt, 64 );

                for (int i = 0; i < 4; i++)
                {
                    if (!(txt[i] > 0x1F && txt[i] < 0xFF && txt[i] != 0x7F))
                        bAddStr = false;
                }

                if (bAddStr)
                {
                    txt[63] = '\0'; // null terminte (even though we prolly dont have to)
                    x = AddText( View, x, y, g_clrChar, HS_NONE, _T( "'%hs'" ), txt );
                }
            }
        }
    }
    else if (m_nodeType == nt_hex16)
    {
        if (g_bInt)
        {
            short intVal = *((short*)&View->Data[m_Offset]);
            if (intVal == 0)
            {
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(%i)" ), intVal );
            }
            else
            {
                x = AddText( View, x, y, g_clrValue, HS_NONE, _T( "(0x%X)" ), intVal );
            }
        }
    }

    return x;
}

void CNodeBase::StandardUpdate( const PHOTSPOT Spot )
{
    if (Spot->Id == HS_NAME)
    {
        m_strName = Spot->Text;
    }	
    else if (Spot->Id == HS_COMMENT)
    {
        m_strComment = Spot->Text;
    }	
}

NODESIZE CNodeBase::DrawHidden( const PVIEWINFO View, int x, int y )
{
    NODESIZE DrawSize;

    View->Dc->FillSolidRect( 0, y, View->ClientRect->right, 1, m_bSelected ? g_clrSelect : g_clrHidden );

    DrawSize.x = 0;
    DrawSize.y = y;

    return DrawSize;
}
