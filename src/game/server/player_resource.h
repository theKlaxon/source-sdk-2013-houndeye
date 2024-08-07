//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Entity that propagates general data needed by clients for every player.
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "shareddefs.h"

class CPlayerResource : public CBaseEntity {
	DECLARE_CLASS( CPlayerResource, CBaseEntity );

public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual void Spawn();
	virtual int ObjectCaps() { return BaseClass::ObjectCaps() | FCAP_DONT_SAVE; }
	virtual void ResourceThink();
	virtual void UpdatePlayerData();
	virtual int UpdateTransmitState();

protected:
	// Data for each player that's propagated to all clients
	// Stored in individual arrays so they can be sent down via datatables
	CNetworkArray( int, m_iPing, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_iScore, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_iDeaths, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_bConnected, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_iTeam, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_bAlive, MAX_PLAYERS + 1 );
	CNetworkArray( int, m_iHealth, MAX_PLAYERS + 1 );

	int m_nUpdateCounter;
};

extern CPlayerResource* g_pPlayerResource;
