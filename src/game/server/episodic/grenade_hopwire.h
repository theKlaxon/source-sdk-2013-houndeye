//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "Sprite.h"
#include "basegrenade_shared.h"

extern ConVar hopwire_trap;

class CGravityVortexController;

class CGrenadeHopwire : public CBaseGrenade {
	DECLARE_CLASS( CGrenadeHopwire, CBaseGrenade );
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

public:
	void Spawn();
	void Precache();
	bool CreateVPhysics();
	void SetTimer( float timer );
	void SetVelocity( const Vector& velocity, const AngularImpulse& angVelocity );
	void Detonate();

	void EndThink();   // Last think before going away
	void CombatThink();// Makes the main explosion go off

protected:
	void KillStriders();

	CHandle<CGravityVortexController> m_hVortexController;
};

extern CBaseGrenade* HopWire_Create( const Vector& position, const QAngle& angles, const Vector& velocity, const AngularImpulse& angVelocity, CBaseEntity* pOwner, float timer );
