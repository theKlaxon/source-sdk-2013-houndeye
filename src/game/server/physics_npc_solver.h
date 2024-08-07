//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


extern CBaseEntity* NPCPhysics_CreateSolver( CAI_BaseNPC* pNPC, CBaseEntity* pPhysicsObject, bool disableCollisions, float separationDuration );
extern CBaseEntity* EntityPhysics_CreateSolver( CBaseEntity* pMovingEntity, CBaseEntity* pPhysicsBlocker, bool disableCollisions, float separationDuration );
bool NPCPhysics_SolverExists( CAI_BaseNPC* pNPC, CBaseEntity* pPhysicsObject );
