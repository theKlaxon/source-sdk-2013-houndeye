//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basecombatweapon.h"
#include "energy_wave.h"


//-----------------------------------------------------------------------------
// Purpose: Shield
//-----------------------------------------------------------------------------
class CEnergyWave : public CBaseEntity {
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CEnergyWave, CBaseEntity );
	DECLARE_SERVERCLASS();

public:
	void Spawn();
	void Precache();

public:
	static CEnergyWave* Create( CBaseEntity* pentOwner );
};
