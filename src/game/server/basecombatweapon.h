//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basecombatweapon_shared.h"
#include "entityoutput.h"

//-----------------------------------------------------------------------------
// Bullet types
//-----------------------------------------------------------------------------

// -----------------------------------------
//	Sounds
// -----------------------------------------

struct animevent_t;

extern void SpawnBlood( Vector vecSpot, const Vector& vecDir, int bloodColor, float flDamage );
