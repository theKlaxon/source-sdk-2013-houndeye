//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SPARK_H
#define SPARK_H
#if IsWindows()
#pragma once
#endif

void DoSpark( CBaseEntity *ent, const Vector &location, int nMagnitude, int nTrailLength, bool bPlaySound, const Vector &vecDir );
#endif // SPARK_H
