//========= Copyright Valve Corporation, All rights reserved. ============//
#pragma once
#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
#endif
#include "weapon_hl2mpbase.h"


#if defined( CLIENT_DLL )
	#define CBaseHL2MPCombatWeapon C_BaseHL2MPCombatWeapon
#endif

class CBaseHL2MPCombatWeapon : public CWeaponHL2MPBase {
#if !defined( CLIENT_DLL )
	DECLARE_DATADESC();
#endif

	DECLARE_CLASS( CBaseHL2MPCombatWeapon, CWeaponHL2MPBase );

public:
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CBaseHL2MPCombatWeapon();

	virtual bool WeaponShouldBeLowered();

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

protected:
	bool m_bLowered;      // Whether the viewmodel is raised or lowered
	float m_flRaiseTime;  // If lowered, the time we should raise the viewmodel
	float m_flHolsterTime;// When the weapon was holstered

private:
	CBaseHL2MPCombatWeapon( const CBaseHL2MPCombatWeapon& );
};
