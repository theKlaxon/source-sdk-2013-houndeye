//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#pragma once


class CCommand;
class CUserCmd;
class CBasePlayer;


void ClientActive( edict_t* pEdict, bool bLoadGame );
void ClientPutInServer( edict_t* pEdict, const char* playername );
void ClientCommand( CBasePlayer* pSender, const CCommand& args );
void ClientPrecache();
// Game specific precaches
void ClientGamePrecache();
const char* GetGameDescription();
void Host_Say( edict_t* pEdict, bool teamonly );
