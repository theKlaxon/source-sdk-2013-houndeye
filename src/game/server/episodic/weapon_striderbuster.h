//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Helper functions for the striderbuster weapon.
//
//=============================================================================
#pragma once


bool StriderBuster_IsAttachedStriderBuster( CBaseEntity* pEntity, CBaseEntity* pAttachedTo = NULL );
void StriderBuster_OnAddToCargoHold( CBaseEntity* pEntity );
bool StriderBuster_OnFlechetteAttach( CBaseEntity* pEntity, Vector& vecForceDir );
int StriderBuster_NumFlechettesAttached( CBaseEntity* pEntity );
float StriderBuster_GetPickupTime( CBaseEntity* pEntity );
bool StriderBuster_WasKnockedOffStrider( CBaseEntity* pEntity );
