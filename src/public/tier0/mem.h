//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Memory allocation!
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include <stddef.h>

#if IsLinux()
	#undef offsetof
	#define offsetof(s,m)	(size_t)&(((s*)0)->m)
#endif

#include "tier0/platform.h"

#if !defined(STATIC_TIER0) && !defined(_STATIC_LINKED)
	#ifdef TIER0_DLL_EXPORT
		#define MEM_INTERFACE DLL_EXPORT
	#else
		#define MEM_INTERFACE DLL_IMPORT
	#endif
#else // BUILD_AS_DLL
	#define MEM_INTERFACE extern
#endif // BUILD_AS_DLL



//-----------------------------------------------------------------------------
// DLL-exported methods for particular kinds of memory
//-----------------------------------------------------------------------------
MEM_INTERFACE void *MemAllocScratch( int nMemSize );
MEM_INTERFACE void MemFreeScratch();

#if IsLinux()
	MEM_INTERFACE void ZeroMemory( void *mem, size_t length );
#endif
