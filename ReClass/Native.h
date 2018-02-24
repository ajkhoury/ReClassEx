#pragma once

//
// Thanks to Microsoft's DDK and Process Hacker for this stuff!
//

#include "NativeCommon.h"
#include "NativeObjects.h"
#include "NativeProcess.h"
#include "NativeInformation.h"
#include "NativeRtl.h"
#include "NativeLoader.h"


typedef NTSTATUS( NTAPI *tNtOpenProcessToken )(
    IN HANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    OUT PHANDLE TokenHandle
    );

typedef NTSTATUS( NTAPI *tNtAdjustPrivilegesToken )(
    IN HANDLE TokenHandle,
    IN BOOLEAN DisableAllPrivileges,
    IN PTOKEN_PRIVILEGES NewState,
    IN ULONG BufferLength,
    OUT PTOKEN_PRIVILEGES PreviousState OPTIONAL,
    OUT PULONG ReturnLength OPTIONAL
    );


