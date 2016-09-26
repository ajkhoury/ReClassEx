#include "stdafx.h"

#include "CNodeFunction.h"

CNodeFunction::CNodeFunction( )
{
	memsize = 64;
}

void CNodeFunction::Update( CHotSpot & Spot )
{
	StandardUpdate( Spot );

	if (Spot.ID == 0)
	{
		Assembly.clear( );

		size_t address = Spot.Address;
		char* code[1536]; // max 1536 lines
		ReClassReadMemory( (LPVOID)address, code, 1536 );
	}
}

int CNodeFunction::Draw( ViewInfo & View, int x, int y )
{
	return 0;
}

