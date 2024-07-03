//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//

#ifndef REPLAYLIB_H
#define REPLAYLIB_H
#if IsWindows()
#pragma once
#endif

//----------------------------------------------------------------------------------------

class IClientReplayContext;

//----------------------------------------------------------------------------------------

bool ReplayLib_Init( const char *pGameDir, IClientReplayContext *pClientReplayContext );

//----------------------------------------------------------------------------------------

#endif // REPLAYLIB_H