//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Engine voice interface
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basetypes.h"
#include "platform.h"

#define IENGINEVOICE_INTERFACE_VERSION "IEngineVoice001"

abstract_class IEngineVoice {
public:
	virtual bool IsHeadsetPresent( int iController ) = 0;
	virtual bool IsLocalPlayerTalking( int iController ) = 0;

	virtual void AddPlayerToVoiceList( XUID xPlayer, int iController ) = 0;
	virtual void RemovePlayerFromVoiceList( XUID xPlayer, int iController ) = 0;

	virtual void GetRemoteTalkers( int* pNumTalkers, XUID* pRemoteTalkers ) = 0;

	virtual bool VoiceUpdateData( int iController ) = 0;
	virtual void GetVoiceData( int iController, const byte** ppvVoiceDataBuffer, unsigned int* pnumVoiceDataBytes ) = 0;
	virtual void VoiceResetLocalData( int iController ) = 0;

	virtual void SetPlaybackPriority( XUID remoteTalker, int iController, int iAllowPlayback ) = 0;
	virtual void PlayIncomingVoiceData( XUID xuid, const byte* pbData, unsigned int dwDataSize, const bool* bAudiblePlayers = NULL ) = 0;

	virtual void RemoveAllTalkers() = 0;
};
