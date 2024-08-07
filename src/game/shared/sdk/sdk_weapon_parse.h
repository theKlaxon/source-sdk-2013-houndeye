//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "networkvar.h"
#include "weapon_parse.h"


//--------------------------------------------------------------------------------------------------------
class CSDKWeaponInfo : public FileWeaponInfo_t {
public:
	DECLARE_CLASS_GAMEROOT( CSDKWeaponInfo, FileWeaponInfo_t );

	CSDKWeaponInfo();

	virtual void Parse( ::KeyValues* pKeyValuesData, const char* szWeaponName );

	char m_szAnimExtension[ 16 ];// string used to generate player animations with this weapon

	// Parameters for FX_FireBullets:
	int m_iDamage;
	int m_iBullets;
	float m_flCycleTime;
};
