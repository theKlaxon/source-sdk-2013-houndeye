//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "weapon_basesdkgrenade.h"


#ifdef CLIENT_DLL
	#define CSDKGrenade C_SDKGrenade
#endif

//-----------------------------------------------------------------------------
// Fragmentation grenades
//-----------------------------------------------------------------------------
class CSDKGrenade : public CBaseSDKGrenade {
public:
	DECLARE_CLASS( CSDKGrenade, CBaseSDKGrenade );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CSDKGrenade() {}

	virtual SDKWeaponID GetWeaponID() const { return WEAPON_GRENADE; }

	#ifndef CLIENT_DLL
		DECLARE_DATADESC();

		virtual void EmitGrenade( Vector vecSrc, QAngle vecAngles, Vector vecVel, AngularImpulse angImpulse, CBasePlayer* pPlayer );
	#endif

	CSDKGrenade( const CSDKGrenade& ) {}
};
