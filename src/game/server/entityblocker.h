//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "cbase.h"

//==================================================================================================================
// Entity Blocker
//==================================================================================================================
class CEntityBlocker : public CBaseEntity {
	DECLARE_CLASS( CEntityBlocker, CBaseEntity );

public:
	static CEntityBlocker* Create( const Vector& origin, const Vector& mins, const Vector& maxs, CBaseEntity* pOwner = NULL, bool bBlockPhysics = false );

	void Spawn();
	bool TestCollision( const Ray_t& ray, unsigned int mask, trace_t& trace );
};
