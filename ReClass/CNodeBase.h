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

	virtual NodeSize Draw( const ViewInfo& View, int x, int y ) = 0;
	virtual ULONG GetMemorySize( ) = 0;
	virtual void Update( const HotSpot& Spot ) = 0;

	inline NodeType GetType( ) const { return m_nodeType; }

	inline size_t GetOffset( ) const { return m_Offset; }
	inline void SetOffset( const size_t& offset ) { m_Offset = offset; }

	inline const CString& GetOffsetString( ) const { return m_strOffset; }
	inline void SetOffsetString( const CString& offsetStr ) { m_strOffset = offsetStr; }
	inline void SetOffsetString( LPCTSTR offsetStr ) { m_strOffset = offsetStr; }

	inline const CString& GetName( ) const { return m_strName; }
	inline void SetName( const CString& name ) { m_strName = name; }
	inline void SetName( LPCTSTR name ) { m_strName = name; }

	inline const CString& GetComment( ) const { return m_strComment; }
	inline void SetComment( const CString& comment ) { m_strComment = comment; }
	inline void SetComment( LPCTSTR comment ) { m_strComment = comment; }

	inline CNodeBase* GetParent( ) const { return m_pParentNode; }
	inline void SetParent( CNodeBase* newParentNode ) { m_pParentNode = newParentNode; }

	inline void AddNode( CNodeBase* newNode ) { m_ChildNodes.push_back( newNode ); }
	inline void InsertNode( int idx, CNodeBase* newNode ) { m_ChildNodes.insert( m_ChildNodes.begin( ) + idx, newNode ); }
	inline CNodeBase* GetNode( int idx ) { return (CNodeBase*)m_ChildNodes[idx]; }
	inline int FindNode( CNodeBase* pNode ) {
		auto found = std::find( m_ChildNodes.begin( ), m_ChildNodes.end( ), pNode );
		return (found != m_ChildNodes.end( )) ? (int)(found - m_ChildNodes.begin( )) : -1;
	}
	inline void SetNode( int idx, CNodeBase* newNode ) { m_ChildNodes[idx] = newNode; }
	inline void DeleteNode( int idx ) { if (m_ChildNodes[idx]) { delete(m_ChildNodes[idx]); RemoveNode( idx ); } }
	inline void RemoveNode( int idx ) { m_ChildNodes.erase( m_ChildNodes.begin( ) + idx ); }
	inline size_t NodeCount( ) const { return m_ChildNodes.size( ); }

	inline bool IsHidden( ) { return m_bHidden; }
	inline void Show( ) { m_bHidden = false; }
	inline void Hide( ) { m_bHidden = true; }
	inline void SetHidden( bool hidden ) { m_bHidden = hidden; }
	inline void ToggleHidden( ) { m_bHidden = !m_bHidden; }

	inline bool IsSelected( ) { return m_bSelected; }
	inline void SetSelected( bool selected ) { m_bSelected = selected; }
	inline void Select( ) { m_bSelected = true; }
	inline void Unselect( ) { m_bSelected = false; }
	inline void ToggleSelected( ) { m_bSelected = !m_bSelected; }

	inline bool IsLevelOpen( int idx ) { return m_LevelsOpen[idx]; }
	inline void ToggleLevelOpen( int idx ) { m_LevelsOpen[idx] = !m_LevelsOpen[idx]; }

	// Incorrect view.address
	void AddHotSpot( const ViewInfo& View, const CRect& Spot, CString Text, int ID, int Type );

	int AddText( const ViewInfo& View, int x, int y, DWORD color, int HitID, const wchar_t* fmt, ... );

	int AddText( const ViewInfo& View, int x, int y, DWORD color, int HitID, const char* fmt, ... );

	int AddAddressOffset( const ViewInfo& View, int x, int y );

	void AddSelection( const ViewInfo& View, int x, int y, int Height );

	int AddIcon( const ViewInfo& View, int x, int y, int idx, int ID, int Type );

	int AddOpenClose( const ViewInfo& View, int x, int y );

	void AddDelete( const ViewInfo& View, int x, int y );

	//void AddAdd(ViewInfo& View,int x,int y);

	void AddTypeDrop( const ViewInfo& View, int x, int y );

	int ResolveRTTI( ULONG_PTR Address, int &x, const ViewInfo& View, int y );

	int AddComment( const ViewInfo& View, int x, int y );

	void StandardUpdate( const HotSpot &Spot );

	NodeSize DrawHidden( const ViewInfo& View, int x, int y );

protected:
	NodeType m_nodeType;

	size_t m_Offset; // Can also be an address
	CString m_strOffset;

	CString m_strName;
	CString m_strComment;

	CNodeBase* m_pParentNode;
	std::vector<CNodeBase*> m_ChildNodes;

	bool m_bHidden;
	bool m_bSelected;

	std::vector<bool> m_LevelsOpen;
};

__forceinline CStringA GetStringFromMemoryA( char* pMemory, int Length )
{
	CStringA ascii;
	for (int i = 0; i < Length; i++)
	{
		ascii += (pMemory[i] > 0x1F && pMemory[i] < 0xFF && pMemory[i] != 0x7F) ? (char)pMemory[i] : '.';
	}
	return ascii;
}

__forceinline CStringW GetStringFromMemoryW( wchar_t* pMemory, int Length )
{
	CStringW widechar;
	for (int i = 0; i < Length; i++)
	{
		widechar += (pMemory[i] > 0x1F && pMemory[i] < 0xFF && pMemory[i] != 0x7F) ? (wchar_t)pMemory[i] : (wchar_t)(L'.');
	}
	return widechar;
}