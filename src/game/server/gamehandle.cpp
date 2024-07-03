//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: returns the module handle of the game dll
//			this is in its own file to protect it from tier0 PROTECTED_THINGS
//=============================================================================//


#if IsWindows()
#include "winlite.h"
extern HMODULE win32DLLHandle;
#elif IsPosix()
#include <stdio.h>
#include "tier0/dbg.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void *GetGameModuleHandle()
{
#if IsWindows()
	return (void *)win32DLLHandle;
#elif IsPosix()
	Assert(0);
	return NULL; // NOT implemented
#else
#error "GetGameModuleHandle() needs to be implemented"
#endif
}

