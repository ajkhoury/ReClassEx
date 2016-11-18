#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_M_X64)
unsigned __int64 ReadGS( unsigned __int64 );
#else
unsigned long __stdcall ReadFS( unsigned long );
#endif

#ifdef __cplusplus
}
#endif
