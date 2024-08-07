//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Tripmine
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basegrenade_shared.h"

class CRopeKeyframe;

// ####################################################################
//   CTripwireHook
//
//		This is what the tripwire shoots out at the end of the rope
// ####################################################################
class CTripwireHook : public CBaseAnimating {
	DECLARE_DATADESC();

public:
	DECLARE_CLASS( CTripwireHook, CBaseAnimating );

	EHANDLE m_hGrenade;
	bool m_bAttached;

	void Spawn();
	void Precache();
	bool CreateVPhysics();
	void EndTouch( CBaseEntity* pOther );
	void SetVelocity( const Vector& velocity, const AngularImpulse& angVelocity );
};

class CTripwireGrenade : public CBaseGrenade {
public:
	DECLARE_CLASS( CTripwireGrenade, CBaseGrenade );

	CTripwireGrenade();
	void Spawn();
	void Precache();

	int OnTakeDamage_Alive( const CTakeDamageInfo& info );

	void WarningThink();
	void PowerupThink();
	void RopeBreakThink();
	void FireThink();
	void Event_Killed( const CTakeDamageInfo& info );
	void Attach();

	void MakeRope();
	void BreakRope();
	void ShakeRope();
	void FireMissile( const Vector& vTargetPos );

private:
	float m_flPowerUp;
	Vector m_vecDir;

	int m_nMissileCount;

	Vector m_vTargetPos;
	Vector m_vTargetOffset;

	CRopeKeyframe* m_pRope;
	CTripwireHook* m_pHook;

	DECLARE_DATADESC();
};
