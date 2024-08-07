//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Client side CTFTeam class
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "c_team.h"
#include "shareddefs.h"

class C_BaseEntity;
class C_BaseObject;
class CBaseTechnology;

//-----------------------------------------------------------------------------
// Purpose: TF's Team manager
//-----------------------------------------------------------------------------
class C_SDKTeam : public C_Team {
	DECLARE_CLASS( C_SDKTeam, C_Team );
	DECLARE_CLIENTCLASS();

public:
	C_SDKTeam();
	virtual ~C_SDKTeam();
};
