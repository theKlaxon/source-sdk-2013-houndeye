//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Creates an avoidance sphere
//-----------------------------------------------------------------------------
CBaseEntity* CreateHelicopterAvoidanceSphere( CBaseEntity* pParent, int nAttachment, float flRadius, bool bAvoidBelow = false );

// Chopper gibbage
void Chopper_BecomeChunks( CBaseEntity* pChopper );
void Chopper_CreateChunk( CBaseEntity* pChopper, const Vector& vecChunkPos, const QAngle& vecChunkAngles, const char* pszChunkName, bool bSmall );
void Chopper_PrecacheChunks( CBaseEntity* pChopper );
