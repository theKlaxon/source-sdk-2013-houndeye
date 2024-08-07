//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "weapon_hl2mpbase.h"


#if defined( CLIENT_DLL )
	#define CHL2MPMachineGun C_HL2MPMachineGun
#endif

//=========================================================
// Machine gun base class
//=========================================================
class CHL2MPMachineGun : public CWeaponHL2MPBase {
public:
	DECLARE_CLASS( CHL2MPMachineGun, CWeaponHL2MPBase );
	DECLARE_DATADESC();

	CHL2MPMachineGun();

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	void PrimaryAttack();

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void ItemPostFrame();
	virtual void FireBullets( const FireBulletsInfo_t& info );
	virtual bool Deploy();

	virtual const Vector& GetBulletSpread();

	int WeaponSoundRealtime( WeaponSound_t shoot_type );

	// utility function
	static void DoMachineGunKick( CBasePlayer* pPlayer, float dampEasy, float maxVerticleKickAngle, float fireDurationTime, float slideLimitTime );

private:
	CHL2MPMachineGun( const CHL2MPMachineGun& );

protected:
	int m_nShotsFired;// Number of consecutive shots fired

	float m_flNextSoundTime;// real-time clock of when to make next sound
};
