#pragma once

#include "CNodeBase.h"

class CNodeClass : public CNodeBase {
public:
	CNodeClass( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( );

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );

	inline void SetChildClassFrame( class CClassFrame* pChild ) { m_pChildClassFrame = pChild; }
	inline class CClassFrame* GetChildClassFrame( ) { return m_pChildClassFrame; }

    inline void SetCodeString( LPCTSTR CodeStr ) { m_Code.SetString( CodeStr ); }

public:
	size_t m_Idx;
	size_t m_RequestPosition;
	CString m_Code;
	class CClassFrame* m_pChildClassFrame;
};
