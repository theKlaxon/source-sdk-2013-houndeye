//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#if !defined( __cplusplus )
	#error "Why are you not compiling with C++?"
#endif

#include "vphysics_interface.h"
class IPhysics;
class IPhysicsCollision;

extern CreateInterfaceFn GetPhysicsFactory();

extern "C" {
	// tools need to force the path
	void PhysicsDLLPath( const char *pPathname );
}
