//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Team management class. Contains all the details for a specific team
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "team.h"
#include "utlvector.h"


//-----------------------------------------------------------------------------
// Purpose: Team Manager
//-----------------------------------------------------------------------------
class CSDKTeam : public CTeam {
	DECLARE_CLASS( CSDKTeam, CTeam );
	DECLARE_SERVERCLASS();

public:
	// Initialization
	virtual void Init( const char* pName, int iNumber );
};


extern CSDKTeam* GetGlobalSDKTeam( int iIndex );
