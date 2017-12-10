#pragma once

#include "CNodeBase.h"

class CNodeIcon : public CNodeBase {
public:
	virtual NODESIZE Draw( const PVIEWINFO View, int x, int y );
};

