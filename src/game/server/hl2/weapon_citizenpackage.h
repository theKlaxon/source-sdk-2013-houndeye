//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once
#include "basehlcombatweapon.h"

//=============================================================================
//
// Weapon - Citizen Package Class
//
class CWeaponCitizenPackage : public CBaseHLCombatWeapon {
	DECLARE_CLASS( CWeaponCitizenPackage, CBaseHLCombatWeapon );

public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_ACTTABLE();

	void ItemPostFrame();
	void Drop( const Vector& vecVelocity );
};
