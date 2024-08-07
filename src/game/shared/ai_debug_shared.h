//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "tier0/vprof.h"


// This uses VPROF to profile
//#define VPROF_AI 1


#ifdef VPROF_AI
inline void AI_TraceLine( const Vector& vecAbsStart, const Vector& vecAbsEnd, uint32 mask, const IHandleEntity* ignore, int32 collisionGroup, trace_t* ptr ) {
	VPROF( "AI_TraceLine" );
	UTIL_TraceLine( vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr );
}

inline void AI_TraceLine( const Vector& vecAbsStart, const Vector& vecAbsEnd, uint32 mask, ITraceFilter* pFilter, trace_t* ptr ) {
	VPROF( "AI_TraceLine" );
	UTIL_TraceLine( vecAbsStart, vecAbsEnd, mask, pFilter, ptr );
}

inline void AI_TraceHull( const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
						  const Vector& hullMax, uinte32 mask, const IHandleEntity* ignore,
						  int32 collisionGroup, trace_t* ptr ) {
	VPROF( "AI_TraceHull" );
	UTIL_TraceHull( vecAbsStart, vecAbsEnd, hullMin, hullMax, mask, ignore, collisionGroup, ptr );
}

inline void AI_TraceHull( const Vector& vecAbsStart, const Vector& vecAbsEnd, const Vector& hullMin,
						  const Vector& hullMax, uint32 mask, ITraceFilter* pFilter, trace_t* ptr ) {
	VPROF( "AI_TraceHull" );
	UTIL_TraceHull( vecAbsStart, vecAbsEnd, hullMin, hullMax, mask, pFilter, ptr );
}

inline void AI_TraceEntity( CBaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd, uint32 mask, trace_t* ptr ) {
	VPROF( "AI_TraceEntity" );
	UTIL_TraceEntity( pEntity, vecAbsStart, vecAbsEnd, mask, ptr );
}
#else
	#define AI_TraceLine UTIL_TraceLine
	#define AI_TraceHull UTIL_TraceHull
	#define AI_TraceEntity UTIL_TraceEntity
#endif
