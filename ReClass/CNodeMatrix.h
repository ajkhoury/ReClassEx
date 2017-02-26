#pragma once

class CNodeMatrix : public CNodeBase
{
public:
	CNodeMatrix( );

	virtual void Update( HotSpot& Spot );

	virtual ULONG GetMemorySize( ) { return 4 * 4 * sizeof( float ); }

	virtual NodeSize Draw( ViewInfo& View, int x, int y );
};