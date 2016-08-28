#pragma once

#include <atltypes.h>
#include <tchar.h>

#define	NONE -1
#define	HS_NONE -1
#define HS_EDIT 0
#define HS_OPENCLOSE 1
#define HS_SELECT 2
#define HS_DROP 3
#define HS_CLICK 4
#define HS_CHANGE_A 5
#define HS_CHANGE_X 6
#define HS_DELETE 7
#define HS_RTTI 8
#define HS_ADDRESS 68
#define HS_NAME 69
#define HS_COMMENT 70

class CHotSpot
{
public:
	CRect Rect;
	CString Text;
	size_t Address;
	int ID;
	int Type;
	unsigned int Level;
	class CNodeBase* object;
};