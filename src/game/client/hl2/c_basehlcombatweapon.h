//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "basehlcombatweapon_shared.h"


class C_HLMachineGun : public C_BaseHLCombatWeapon {
public:
	DECLARE_CLASS( C_HLMachineGun, C_BaseHLCombatWeapon );
	DECLARE_CLIENTCLASS();
};

class C_HLSelectFireMachineGun : public C_HLMachineGun {
public:
	DECLARE_CLASS( C_HLSelectFireMachineGun, C_HLMachineGun );
	DECLARE_CLIENTCLASS();
};

class C_BaseHLBludgeonWeapon : public C_BaseHLCombatWeapon {
public:
	DECLARE_CLASS( C_BaseHLBludgeonWeapon, C_BaseHLCombatWeapon );
	DECLARE_CLIENTCLASS();
};
