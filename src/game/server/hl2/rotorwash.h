//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef ROTORWASH_H
#define ROTORWASH_H
#if IsWindows()
#pragma once
#endif

CBaseEntity *CreateRotorWashEmitter( const Vector &localOrigin, const QAngle &localAngles, CBaseEntity *pOwner, float flAltitude );

#endif // ROTORWASH_H
