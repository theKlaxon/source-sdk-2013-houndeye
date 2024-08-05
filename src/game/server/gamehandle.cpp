//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: returns the module handle of the game dll
//			this is in its own file to protect it from tier0 PROTECTED_THINGS
//=============================================================================//


#if defined( PLATFORM_WINDOWS )
	#include "winlite.h"
	extern HMODULE win32DLLHandle;
#elif defined( PLATFORM_POSIX )
	#include <cstdio>
#endif
#include "tier0/dbg.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void* GetGameModuleHandle() {
	#if IsWindows()
		return (void *) win32DLLHandle;
	#elif IsPosix()
		Assert( false );
		return nullptr; // NOT implemented
	#else
		#error "GetGameModuleHandle() needs to be implemented"
	#endif
}

