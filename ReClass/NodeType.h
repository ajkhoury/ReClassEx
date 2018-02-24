#pragma once

#include <tchar.h>

// Forward declarations for all node base types
class CNodeBase;
class CNodeIcon;
class CNodeClass;
class CNodeHex64;
class CNodeHex32;
class CNodeHex16;
class CNodeHex8;
class CNodeBits;
class CNodeVTable;
class CNodeFunction;
class CNodeFunctionPtr;
class CNodePtr;
class CNodeInt64;
class CNodeInt32;
class CNodeInt16;
class CNodeInt8;
class CNodeQWORD;
class CNodeDWORD;
class CNodeWORD;
class CNodeByte;
class CNodeText;
class CNodeCharPtr;
class CNodeWCharPtr;
class CNodeUnicode;
class CNodeFloat;
class CNodeDouble;
class CNodeVec2;
class CNodeVec3;
class CNodeQuat;
class CNodeMatrix;
class CNodeArray;
class CNodeClassInstance;
class CNodeCustom;

// Node type enumerator
enum NodeType {
    nt_none = -1,
    nt_base,
    nt_instance,
    nt_struct,
    nt_hidden,
    nt_hex32,
    nt_hex64,
    nt_hex16,
    nt_hex8,
    nt_pointer,
    nt_int64,
    nt_int32,
    nt_int16,
    nt_int8,
    nt_float,
    nt_double,
    nt_uint32, // dword
    nt_uint16, // word
    nt_uint8, // byte
    nt_text,
    nt_unicode,
    nt_functionptr,
    nt_custom,
    nt_vec2,
    nt_vec3,
    nt_quat,
    nt_matrix,
    nt_vtable,
    nt_array,
    nt_class,
    nt_pchar,
    nt_pwchar,
    nt_bits,
    nt_uint64, // qword
    nt_function,
    nt_ptrarray,
};

#define ISHEXTYPE(type) (type == nt_hex64 || type == nt_hex32 || type == nt_hex16 || type == nt_hex8 || type == nt_bits)

static const TCHAR* s_NodeTypes[] = { 
    _T( "nt_base" ),
    _T( "nt_instance" ),
    _T( "nt_struct" ), 
    _T( "nt_hidden" ), 
    _T( "nt_hex32" ), 
    _T( "nt_hex64" ), 
    _T( "nt_hex16" ), 
    _T( "nt_hex8" ),
    _T( "nt_pointer" ), 
    _T( "nt_int64" ), 
    _T( "nt_int32" ), 
    _T( "nt_int16" ), 
    _T( "nt_int8" ), 
    _T( "nt_float" ), 
    _T( "nt_double" ), 
    _T( "nt_uint32" ), 
    _T( "nt_uint16" ), 
    _T( "nt_uint8" ),
    _T( "nt_text" ), 
    _T( "nt_unicode" ),
    _T( "nt_functionptr" ), 
    _T( "nt_custom" ), 
    _T( "nt_vec2" ), 
    _T( "nt_vec3" ), 
    _T( "nt_quat" ), 
    _T( "nt_matrix" ), 
    _T( "nt_vtable" ), 
    _T( "nt_array" ),
    _T( "nt_class" ), 
    _T( "nt_pchar" ), 
    _T( "nt_pwchar" ), 
    _T( "nt_bits" ), 
    _T( "nt_uint64" ), 
    _T( "nt_function" ), 
    _T( "nt_ptrarray" ),
};

FORCEINLINE const TCHAR* NodeTypeToString( NodeType type )
{
    return s_NodeTypes[type];
}