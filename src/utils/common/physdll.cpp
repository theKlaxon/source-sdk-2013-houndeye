//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#include "physdll.h"
#include "filesystem_tools.h"

static CSysModule* pPhysicsModule = nullptr;

CreateInterfaceFn GetPhysicsFactory() {
	if ( !pPhysicsModule ) {
		pPhysicsModule = g_pFullFileSystem->LoadModule( "vphysics" DLL_EXT_STRING );
		if ( !pPhysicsModule )
			return nullptr;
	}

	return Sys_GetFactory( pPhysicsModule );
}

void PhysicsDLLPath( const char* pPathname ) {
	if ( !pPhysicsModule ) {
		pPhysicsModule = g_pFullFileSystem->LoadModule( pPathname );
	}
}
