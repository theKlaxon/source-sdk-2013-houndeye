//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once
#include "cbase.h"

//-----------------------------------------------------------------------------
// Purpose: An entity that spawns and controls a particle system
//-----------------------------------------------------------------------------
class CParticleSystem : public CBaseEntity {
	DECLARE_CLASS( CParticleSystem, CBaseEntity );

public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CParticleSystem();

	virtual void Precache();
	virtual void Spawn();
	virtual void Activate();
	virtual int UpdateTransmitState();

	void StartParticleSystem();
	void StopParticleSystem();

	void InputStart( inputdata_t& inputdata );
	void InputStop( inputdata_t& inputdata );
	void StartParticleSystemThink();

	enum { kMAXCONTROLPOINTS = 63 };///< actually one less than the total number of cpoints since 0 is assumed to be me

protected:
	/// Load up and resolve the entities that are supposed to be the control points
	void ReadControlPointEnts();

	bool m_bStartActive;
	string_t m_iszEffectName;

	CNetworkVar( bool, m_bActive );
	CNetworkVar( int, m_iEffectIndex )
		CNetworkVar( float, m_flStartTime );// Time at which this effect was started.  This is used after restoring an active effect.

	string_t m_iszControlPointNames[ kMAXCONTROLPOINTS ];
	CNetworkArray( EHANDLE, m_hControlPointEnts, kMAXCONTROLPOINTS );
	CNetworkArray( unsigned char, m_iControlPointParents, kMAXCONTROLPOINTS );
	CNetworkVar( bool, m_bWeatherEffect );
};
