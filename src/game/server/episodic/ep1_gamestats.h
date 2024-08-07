//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once
#include "gamestats.h"

class CEP1GameStats : public CBaseGameStats {
	typedef CBaseGameStats BaseClass;

public:
	CEP1GameStats();

	virtual CBaseGameStats* OnInit( CBaseGameStats* pCurrentGameStats, char const* gamedir );

	virtual bool StatTrackingEnabledForMod() { return true; }
	virtual bool UserPlayedAllTheMaps();

	virtual const char* GetStatSaveFileName();
	virtual const char* GetStatUploadRegistryKeyName();
};
