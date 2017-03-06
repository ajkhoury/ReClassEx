#pragma once

#include "CNodeBase.h"

class CNodeIcon : public CNodeBase {
public:
	virtual NodeSize Draw( const ViewInfo& View, int x, int y );
};

