//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef HL2_GAMESTATS_H
#define HL2_GAMESTATS_H
#if IsWindows()
#pragma once
#endif

#include "gamestats.h"

class CHL2GameStats : public CBaseGameStats
{
	typedef CBaseGameStats BaseClass;

public:
	CHL2GameStats( void );
};

#endif // EP1_GAMESTATS_H
