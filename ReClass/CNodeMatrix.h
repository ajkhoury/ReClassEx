#pragma once

class CNodeMatrix : public CNodeBase
{
public:
	CNodeMatrix( );

	virtual void Update( const PHOTSPOT Spot );

	virtual ULONG GetMemorySize( ) { return 4 * 4 * sizeof( float ); }

	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};