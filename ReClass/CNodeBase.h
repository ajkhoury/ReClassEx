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
	class CChildView* pChildView;
	CDC* dc;
	CRect* client;
	std::vector<HotSpot>* HotSpots;
	std::vector<CNodeClass*>* Classes;
	size_t Address;
	UCHAR* pData;
	UINT Level;
	bool bMultiSelected;
};

class CNodeBase
{
public:
	CNodeBase( );
	~CNodeBase( ) { }

	virtual int Draw( ViewInfo& View, int x, int y ) = 0;
	virtual int GetMemorySize( ) = 0;
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

	bool IsHidden( ) { return m_bHidden; }
	void Show( ) { m_bHidden = false; }
	void Hide( ) { m_bHidden = true; }
	void SetHidden( bool hidden ) { m_bHidden = hidden; }
	void ToggleHidden( ) { m_bHidden = !m_bHidden; }

	bool IsSelected( ) { return m_bSelected; }
	void SetSelected( bool selected ) { m_bSelected = selected; }
	void Select( ) { m_bSelected = true; }
	void Unselect( ) { m_bSelected = false; }
	void ToggleSelected( ) { m_bSelected = !m_bSelected; }

	bool IsLevelOpen( int idx ) { return m_LevelsOpen[idx]; }
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

	int ResolveRTTI( size_t Val, int &x, ViewInfo& View, int y );

	int AddComment( ViewInfo& View, int x, int y );

	void StandardUpdate( HotSpot &Spot );

	int DrawHidden( ViewInfo& View, int x, int y );

protected:
	NodeType m_nodeType;

	size_t m_Offset;
	CString m_strOffset;

	CString m_strName;
	CString m_strComment;

	CNodeBase* m_pParentNode;
	std::vector<CNodeBase*> Nodes;

	bool m_bHidden;
	bool m_bSelected;

	std::vector<bool> m_LevelsOpen;
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