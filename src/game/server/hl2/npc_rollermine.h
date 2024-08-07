//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once

//------------------------------------
// Spawnflags
//------------------------------------
#define SF_ROLLERMINE_FRIENDLY ( 1 << 16 )
#define SF_ROLLERMINE_PROP_COLLISION ( 1 << 17 )

bool NPC_Rollermine_IsRollermine( CBaseEntity* pEntity );
CBaseEntity* NPC_Rollermine_DropFromPoint( const Vector& originStart, CBaseEntity* pOwner, const char* pszTemplate );
