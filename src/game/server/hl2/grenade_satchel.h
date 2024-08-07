//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Satchel Charge
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basegrenade_shared.h"
#include "hl2mp/weapon_slam.h"

class CSoundPatch;

class CSatchelCharge : public CBaseGrenade {
public:
	DECLARE_CLASS( CSatchelCharge, CBaseGrenade );

	void Spawn();
	void Precache();
	void BounceSound();
	void UpdateSlideSound();
	void KillSlideSound();
	void SatchelTouch( CBaseEntity* pOther );
	void SatchelThink();
	void SatchelUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value );

	CSoundPatch* m_soundSlide;
	float m_flSlideVolume;
	float m_flNextBounceSoundTime;
	bool m_bInAir;
	Vector m_vLastPosition;

public:
	CWeapon_SLAM* m_pMyWeaponSLAM;// Who shot me..
	bool m_bIsAttached;
	void Deactivate();

	CSatchelCharge();
	~CSatchelCharge();

	DECLARE_DATADESC();

private:
	void InitSlideSound();
};
