//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "tier0/platform.h"
#include "userid.h"
#include <inetmsghandler.h>

class IServer;
class INetMessage;

abstract_class IClient : public INetChannelHandler {
public:
	virtual ~IClient() {}

	// connect client
	virtual void Connect( const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge ) = 0;

	// set the client in a pending state waiting for a new game
	virtual void Inactivate() = 0;

	// Reconnect without dropiing the netchannel
	virtual void Reconnect() = 0;// froce reconnect

	// disconnects a client with a given reason
	virtual void Disconnect( PRINTF_FORMAT_STRING const char* reason, ... ) = 0;

	virtual int GetPlayerSlot() const = 0;                           // returns client slot (usually entity number-1)
	virtual int GetUserID() const = 0;                               // unique ID on this server
	virtual const USERID_t GetNetworkID() const = 0;                 // network wide ID
	virtual const char* GetClientName() const = 0;                   // returns client name
	virtual INetChannel* GetNetChannel() = 0;                        // returns client netchannel
	virtual IServer* GetServer() = 0;                                // returns the object server the client belongs to
	virtual const char* GetUserSetting( const char* cvar ) const = 0;// returns a clients FCVAR_USERINFO setting
	virtual const char* GetNetworkIDString() const = 0;              // returns a human readable representation of the network id

	// set/get client data rate in bytes/second
	virtual void SetRate( int nRate, bool bForce ) = 0;
	virtual int GetRate() const = 0;

	// set/get updates/second rate
	virtual void SetUpdateRate( int nUpdateRate, bool bForce ) = 0;
	virtual int GetUpdateRate() const = 0;

	// clear complete object & free all memory
	virtual void Clear() = 0;

	// returns the highest world tick number acknowledge by client
	virtual int GetMaxAckTickCount() const = 0;

	// execute a client command
	virtual bool ExecuteStringCommand( const char* s ) = 0;
	// send client a network message
	virtual bool SendNetMsg( INetMessage & msg, bool bForceReliable = false ) = 0;
	// send client a text message
	virtual void ClientPrintf( PRINTF_FORMAT_STRING const char* fmt, ... ) = 0;

	// client has established network channels, nothing else
	virtual bool IsConnected() const = 0;
	// client is downloading signon data
	virtual bool IsSpawned() const = 0;
	// client active is ingame, receiving snapshots
	virtual bool IsActive() const = 0;
	// returns true, if client is not a real player
	virtual bool IsFakeClient() const = 0;
	// returns true, if client is a HLTV proxy
	virtual bool IsHLTV() const = 0;
	#if defined( REPLAY_ENABLED )
		// returns true, if client is a Replay proxy
		virtual bool IsReplay() const = 0;
	#else
		// !KLUDGE! Reduce number of #ifdefs required
		inline bool IsReplay() const { return false; }
	#endif
	// returns true, if client hears this player
	virtual bool IsHearingClient( int index ) const = 0;
	// returns true, if client hears this player by proximity
	virtual bool IsProximityHearingClient( int index ) const = 0;

	virtual void SetMaxRoutablePayloadSize( int nMaxRoutablePayloadSize ) = 0;
};
