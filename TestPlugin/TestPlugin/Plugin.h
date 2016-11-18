#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <sstream>
#include <memory>

#include "resource.h"
#include "ReClassAPI.h"

BOOL WINAPI WriteCallback( LPVOID write_address, LPVOID buffer_ptr, SIZE_T write_size, PSIZE_T num_write );
BOOL WINAPI ReadCallback( LPVOID read_address, LPVOID buffer_ptr, SIZE_T read_size, PSIZE_T num_read );