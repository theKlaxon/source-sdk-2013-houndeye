//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "basecombatweapon_shared.h"

#if defined( CLIENT_DLL )
	#define CBaseHLCombatWeapon C_BaseHLCombatWeapon
#endif

class CBaseHLCombatWeapon : public CBaseCombatWeapon {
	#if !defined( CLIENT_DLL )
		DECLARE_DATADESC();
	#endif

	DECLARE_CLASS( CBaseHLCombatWeapon, CBaseCombatWeapon );

public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	virtual bool WeaponShouldBeLowered();

	bool CanLower();
	virtual bool Ready();
	virtual bool Lower();
	virtual bool Deploy();
	virtual bool Holster( CBaseCombatWeapon* pSwitchingTo );
	virtual void WeaponIdle();

	virtual void AddViewmodelBob( CBaseViewModel* viewmodel, Vector& origin, QAngle& angles );
	virtual float CalcViewmodelBob();

	virtual Vector GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float GetSpreadBias( WeaponProficiency_t proficiency );

	virtual const WeaponProficiencyInfo_t* GetProficiencyValues();
	static const WeaponProficiencyInfo_t* GetDefaultProficiencyValues();

	virtual void ItemHolsterFrame();

	int m_iPrimaryAttacks;  // # of primary attacks performed with this weapon
	int m_iSecondaryAttacks;// # of secondary attacks performed with this weapon

protected:
	bool m_bLowered;      // Whether the viewmodel is raised or lowered
	float m_flRaiseTime;  // If lowered, the time we should raise the viewmodel
	float m_flHolsterTime;// When the weapon was holstered
};
