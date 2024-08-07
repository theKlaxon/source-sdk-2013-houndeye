//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once


// If iTeam or iClass is -1, then a team or class is randomly chosen.
CBasePlayer* BotPutInServer( bool bFrozen, int iTeam, int iClass );

void Bot_RunAll();
