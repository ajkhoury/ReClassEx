#pragma once

#include "NodeType.h"
#include "HotSpot.h"

#include "Debug.h"
#include "Symbols.h"

#include <afxwin.h>
#include <vector>

#define TXOFFSET 16

// Global node index
extern DWORD g_NodeCreateIndex;

struct ViewInfo
{
	// Temporarily added for testing
	#ifdef _DEBUG
	class CChildView* pChildView;
	#endif

	CDC* dc;
	CRect* client;
	std::vector<HotSpot>* HotSpots;
	std::vector<CNodeClass*>* Classes;
	ULONG_PTR Address;
	UCHAR* pData;
	UINT Level;
	bool bMultiSelected;
};

typedef struct _TYPE_INFO
{
	ULONG_PTR VirtualTable; // type_info class vftable
	ULONG_PTR Data;			// NULL until loaded at runtime
	CHAR Name[1];			// Mangled name (prefix: .?AV=classes, .?AU=structs)

	static BOOL IsValid( ULONG_PTR typeInfo );
	static BOOL IsTypeName( ULONG_PTR name );
	static int  GetName( ULONG_PTR typeInfo, __out LPSTR buffer, int bufferSize );
	static void DoStruct( ULONG_PTR typeInfo );
} TYPE_INFO, *PTYPE_INFO;
typedef struct _TYPE_INFO TYPE_DESCRIPTOR;
typedef TYPE_DESCRIPTOR* PTYPE_DESCRIPTOR;

typedef struct _TYPE_INFO_DISP_INFO
{
	LONG MemberDisp;				// 0x00 Member displacement
	LONG VirtualTableDisp;			// 0x04 Vftable displacement
	LONG InternalVirtualTableDisp;	// 0x08 Displacement inside vftable
} TYPE_INFO_DISP_INFO, *PTYPE_INFO_DISP_INFO;

typedef struct _RTTI_BASE_CLASS_DESCRIPTOR
{
	#if defined(_M_AMD64)
	ULONG TypeDescriptor;				// 0x00 Offset to TYPE_DESCRIPTOR of the class
	#else
	PTYPE_DESCRIPTOR TypeDescriptor;	// 0x00 TYPE_DESCRIPTOR of the class
	#endif
	ULONG ContainedBasesCount;			// 0x04 Number of nested classes in the RTTI_BASE_CLASS_ARRAY
	TYPE_INFO_DISP_INFO DispInfo;		// 0x08 Pointer-to-member displacement info
	ULONG Attributes;					// 0x14 Flags
} RTTI_BASE_CLASS_DESCRIPTOR, *PRTTI_BASE_CLASS_DESCRIPTOR;

#pragma warning (disable : 4200)
typedef struct _RTTI_BASE_CLASS_ARRAY
{
	PRTTI_BASE_CLASS_DESCRIPTOR* BaseClassDescriptors;
} RTTI_BASE_CLASS_ARRAY, *PRTTI_BASE_CLASS_ARRAY;
#pragma warning (default : 4200)

typedef struct _RTTI_CLASS_HIERARCHY_DESCRIPTOR
{
	ULONG Signature;			// 0x00 Zero until loaded
	ULONG Attributes;			// 0x04 Flags
	ULONG BaseClassesCount;		// 0x08 Number of classes in RTTI_BASE_CLASS_ARRAY
	#if defined(_M_AMD64)
	ULONG BaseClassArrayOffset; // 0x0C Offset to PRTTI_BASE_CLASS_ARRAY
	#else
	PRTTI_BASE_CLASS_ARRAY BaseClassArray; // 0x0C
	#endif
} RTTI_CLASS_HIERARCHY_DESCRIPTOR, *PRTTI_CLASS_HIERARCHY_DESCRIPTOR;

typedef struct _RTTI_COMPLETE_OBJECT_LOCATOR
{
	ULONG Signature;				// 0x00 '0' in x86, '1' in x86_64
	ULONG Offset;					// 0x04 Offset of this vftable in the complete class
	ULONG ConstructorDispOffset;	// 0x08 Constructor displacement offset
	#if defined(_M_AMD64)
	ULONG TypeDescriptorOffset;		// 0x0C Offset to PTYPE_INFO
	ULONG ClassDescriptorOffset;	// 0x10 Offset to PRTTI_CLASS_HIERARCHY_DESCRIPTOR which describes inheritance hierarchy
	ULONG ObjectBase;				// 0x14 Object base offset (base = ptr col - objectBase)
	#else
	PTYPE_DESCRIPTOR TypeDescriptor; // 0x0C PTYPE_INFO of the complete class
	PRTTI_CLASS_HIERARCHY_DESCRIPTOR ClassDescriptor; // 0x10
	#endif
} RTTI_COMPLETE_OBJECT_LOCATOR, *PRTTI_COMPLETE_OBJECT_LOCATOR;

struct NodeSize {
	int x;
	int y;
};

class CNodeBase
{
public:
	CNodeBase( );
	~CNodeBase( ) { }

	virtual NodeSize Draw( ViewInfo& View, int x, int y ) = 0;
	virtual ULONG GetMemorySize( ) = 0;
	virtual void Update( HotSpot& Spot ) = 0;

	NodeType GetType( ) { return m_nodeType; }

	size_t GetOffset( ) { return m_Offset; }
	void SetOffset( size_t offset ) { m_Offset = offset; }

	CString GetOffsetString( ) const { return m_strOffset; }
	void SetOffsetString( CString offsetStr ) { m_strOffset = offsetStr; }
	void SetOffsetString( LPCTSTR offsetStr ) { m_strOffset = offsetStr; }

	CString GetName( ) const { return m_strName; }
	void SetName( CString name ) { m_strName = name; }
	void SetName( LPCTSTR name ) { m_strName = name; }

	CString GetComment( ) const { return m_strComment; }
	void SetComment( CString comment ) { m_strComment = comment; }
	void SetComment( LPCTSTR comment ) { m_strComment = comment; }

	CNodeBase* GetParent( ) { return m_pParentNode; }
	void SetParent( CNodeBase* newParentNode ) { m_pParentNode = newParentNode; }

	void AddNode( CNodeBase* newNode ) { Nodes.emplace_back( newNode ); }
	void InsertNode( int idx, CNodeBase* newNode ) { Nodes.insert( Nodes.begin( ) + idx, newNode ); }
	CNodeBase* GetNode( int idx ) { return (CNodeBase*)Nodes[idx]; }
	int FindNode( CNodeBase* pNode );
	void SetNode( int idx, CNodeBase* newNode ) { Nodes[idx] = newNode; }
	void DeleteNode( int idx ) { if (Nodes[idx]) { delete(Nodes[idx]); RemoveNode( idx ); } }
	void RemoveNode( int idx ) { Nodes.erase( Nodes.begin( ) + idx ); }
	size_t NodeCount( ) { return Nodes.size( ); }

	BOOLEAN IsHidden( ) { return m_bHidden; }
	void Show( ) { m_bHidden = false; }
	void Hide( ) { m_bHidden = true; }
	void SetHidden( bool hidden ) { m_bHidden = hidden; }
	void ToggleHidden( ) { m_bHidden = !m_bHidden; }

	BOOLEAN IsSelected( ) { return m_bSelected; }
	void SetSelected( bool selected ) { m_bSelected = selected; }
	void Select( ) { m_bSelected = true; }
	void Unselect( ) { m_bSelected = false; }
	void ToggleSelected( ) { m_bSelected = !m_bSelected; }

	BOOLEAN IsLevelOpen( int idx ) { return m_LevelsOpen[idx]; }
	void ToggleLevelOpen( int idx ) { m_LevelsOpen[idx] = !m_LevelsOpen[idx]; }

	// Incorrect view.address
	void AddHotSpot( ViewInfo& View, CRect& Spot, CString Text, int ID, int Type );

	int AddText( ViewInfo& View, int x, int y, DWORD color, int HitID, const wchar_t* fmt, ... );

	int AddText( ViewInfo& View, int x, int y, DWORD color, int HitID, const char* fmt, ... );

	int AddAddressOffset( ViewInfo& View, int x, int y );

	void AddSelection( ViewInfo& View, int x, int y, int Height );

	int AddIcon( ViewInfo& View, int x, int y, int idx, int ID, int Type );

	int AddOpenClose( ViewInfo& View, int x, int y );

	void AddDelete( ViewInfo& View, int x, int y );

	//void AddAdd(ViewInfo& View,int x,int y);

	void AddTypeDrop( ViewInfo& View, int x, int y );

	int ResolveRTTI( ULONG_PTR Address, int &x, ViewInfo& View, int y );

	int AddComment( ViewInfo& View, int x, int y );

	void StandardUpdate( HotSpot &Spot );

	NodeSize DrawHidden( ViewInfo& View, int x, int y );

protected:
	NodeType m_nodeType;

	size_t m_Offset; // Can also be an address
	CString m_strOffset;

	CString m_strName;
	CString m_strComment;

	CNodeBase* m_pParentNode;
	std::vector<CNodeBase*> Nodes;

	BOOLEAN m_bHidden;
	BOOLEAN m_bSelected;

	std::vector<BOOLEAN> m_LevelsOpen;
};

FORCEINLINE CStringA GetStringFromMemoryA( char* pMemory, int Length )
{
	CStringA ascii;
	for (int i = 0; i < Length; i++)
	{
		ascii += (isprint( pMemory[i] & 0xFF )) ? (char)pMemory[i] : '.';
	}
	return ascii;
}

FORCEINLINE CStringW GetStringFromMemoryW( wchar_t* pMemory, int Length )
{
	CStringW widechar;
	for (int i = 0; i < Length; i++)
	{
		widechar += (iswprint( pMemory[i] ) > 0) ? (wchar_t)pMemory[i] : (wchar_t)(L'.');
	}
	return widechar;
}