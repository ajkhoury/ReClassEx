

#define _INC_MEMORY
#define _INC_STRING
#define _INC_STDDEF
#define _CRT_MEMORY_DEFINED

static
int strlen_r(const char *src)
{
	if(!src)
		return 0;
	const char *src0 = src;
	while(*src)
		src++;
	return int(src - src0);
}

static
void strcpy_r(char *dst, const char *src)
{
//#pragma unroll(8)
	while(*src)
		*(dst++) = *(src++);
	*dst = 0;
}

static
int strcmp_r(const char *dst, const char *src)
{
	if(!dst || !src)
		return -1;

	int f, l;

	do
	{
		f = *(dst++);
		l = *(src++);
	}
	while(f && f == l);

	return f - l;
}


#include <intrin.h>

#undef memcpy
#undef memset
#undef memcmp

#define memset(x,y,z) \
	__stosb((unsigned char *)(x), (unsigned char)(y), (z))


#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL

#include "beaengineSources\BeaEngine.c"

