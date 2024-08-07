//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "imovehelper.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

class CBasePlayer;
class CBaseEntity;


//-----------------------------------------------------------------------------
// Implementation of the movehelper on the server
//-----------------------------------------------------------------------------

abstract_class IMoveHelperServer : public IMoveHelper {
public:
	virtual void SetHost( CBasePlayer * host ) = 0;
};

//-----------------------------------------------------------------------------
// Singleton access
//-----------------------------------------------------------------------------

IMoveHelperServer* MoveHelperServer();
