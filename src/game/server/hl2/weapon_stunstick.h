//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "basebludgeonweapon.h"

#define STUNSTICK_RANGE 75.0f
#define STUNSTICK_REFIRE 0.6f

class CWeaponStunStick : public CBaseHLBludgeonWeapon {
	DECLARE_CLASS( CWeaponStunStick, CBaseHLBludgeonWeapon );
	DECLARE_DATADESC();

public:
	CWeaponStunStick();

	DECLARE_SERVERCLASS();
	DECLARE_ACTTABLE();

	virtual void Precache();

	void Spawn();

	float GetRange() { return STUNSTICK_RANGE; }
	float GetFireRate() { return STUNSTICK_REFIRE; }

	int WeaponMeleeAttack1Condition( float flDot, float flDist );

	bool Deploy();
	bool Holster( CBaseCombatWeapon* pSwitchingTo = nullptr );

	void Drop( const Vector& vecVelocity );
	void ImpactEffect( trace_t& traceHit );
	void SecondaryAttack() { }
	void SetStunState( bool state );
	bool GetStunState();
	void Operator_HandleAnimEvent( animevent_t* pEvent, CBaseCombatCharacter* pOperator );

	float GetDamageForActivity( Activity hitActivity );

	bool CanBePickedUpByNPCs() { return false; }

private:
	CNetworkVar( bool, m_bActive );
};
