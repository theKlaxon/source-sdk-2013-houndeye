//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//

#ifndef IREPLAYERRORSYSYTEM_H
#define IREPLAYERRORSYSYTEM_H
#if IsWindows()
#pragma once
#endif

//----------------------------------------------------------------------------------------

#include "interface.h"

//----------------------------------------------------------------------------------------

class KeyValues;

//----------------------------------------------------------------------------------------

//
// Replay error system
//
class IReplayErrorSystem : public IBaseInterface
{
public:
	virtual void	AddErrorFromTokenName( const char *pToken ) = 0;
	virtual void	AddFormattedErrorFromTokenName( const char *pFormatToken, KeyValues *pFormatArgs ) = 0;
};

//----------------------------------------------------------------------------------------

#endif // IREPLAYERRORSYSYTEM_H