//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once

// Default distance from lightsources that entities are considered visible
// NOTE!!!  This is bigger by a factor of to deal with fixing a bug from HL2.  See dlight_t.h
#define DARKNESS_LIGHTSOURCE_SIZE ( 256.0f * 1.2f )

void AddEntityToDarknessCheck( CBaseEntity* pEntity, float flLightRadius = DARKNESS_LIGHTSOURCE_SIZE );
void RemoveEntityFromDarknessCheck( CBaseEntity* pEntity );
bool LookerCouldSeeTargetInDarkness( CBaseEntity* pLooker, CBaseEntity* pTarget );
bool DarknessLightSourceWithinRadius( CBaseEntity* pLooker, float flRadius );
