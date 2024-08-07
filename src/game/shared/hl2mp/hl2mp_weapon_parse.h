//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "networkvar.h"
#include "weapon_parse.h"


class CHL2MPSWeaponInfo : public FileWeaponInfo_t {
public:
	DECLARE_CLASS_GAMEROOT( CHL2MPSWeaponInfo, FileWeaponInfo_t );

	CHL2MPSWeaponInfo();

	virtual void Parse( ::KeyValues* pKeyValuesData, const char* szWeaponName );

public:
	int m_iPlayerDamage;
};
