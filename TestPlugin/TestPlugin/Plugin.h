#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <sstream>
#include <memory>

#include "ReClassAPI.h"

BOOL 
PLUGIN_CC 
WriteCallback( 
    IN LPVOID Address, 
    IN LPVOID Buffer, 
    IN SIZE_T Size, 
    OUT PSIZE_T BytesWritten 
    );

BOOL 
PLUGIN_CC
ReadCallback( 
    IN LPVOID Address, 
    IN LPVOID Buffer, 
    IN SIZE_T Size, 
    OUT PSIZE_T BytesRead 
    );