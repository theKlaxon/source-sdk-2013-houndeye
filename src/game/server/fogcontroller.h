//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "igamesystem.h"
#include "playernet_vars.h"

// Spawn Flags
#define SF_FOG_MASTER 0x0001

//=============================================================================
//
// Class Fog Controller:
// Compares a set of integer inputs to the one main input
// Outputs true if they are all equivalant, false otherwise
//
class CFogController : public CBaseEntity {
public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();
	DECLARE_CLASS( CFogController, CBaseEntity );

	CFogController();
	~CFogController();

	// Parse data from a map file
	virtual void Activate();
	virtual int UpdateTransmitState();

	// Input handlers
	void InputSetStartDist( inputdata_t& data );
	void InputSetEndDist( inputdata_t& data );
	void InputTurnOn( inputdata_t& data );
	void InputTurnOff( inputdata_t& data );
	void InputSetColor( inputdata_t& data );
	void InputSetColorSecondary( inputdata_t& data );
	void InputSetFarZ( inputdata_t& data );
	void InputSetAngles( inputdata_t& inputdata );
	void InputSetMaxDensity( inputdata_t& inputdata );

	void InputSetColorLerpTo( inputdata_t& data );
	void InputSetColorSecondaryLerpTo( inputdata_t& data );
	void InputSetStartDistLerpTo( inputdata_t& data );
	void InputSetEndDistLerpTo( inputdata_t& data );

	void InputStartFogTransition( inputdata_t& data );

	int DrawDebugTextOverlays();

	void SetLerpValues();
	void Spawn();

	bool IsMaster() { return HasSpawnFlags( SF_FOG_MASTER ); }

public:
	CNetworkVarEmbedded( fogparams_t, m_fog );
	bool m_bUseAngles;
	int m_iChangedVariables;
};

//=============================================================================
//
// Fog Controller System.
//
class CFogSystem : public CAutoGameSystem {
public:
	// Creation/Init.
	CFogSystem( char const* name ) : CAutoGameSystem( name ) {
		m_pMasterController = nullptr;
	}

	~CFogSystem() {
		m_pMasterController = nullptr;
	}

	virtual void LevelInitPreEntity();
	virtual void LevelInitPostEntity();
	CFogController* GetMasterFogController() { return m_pMasterController; }

private:
	CFogController* m_pMasterController;
};

CFogSystem* FogSystem();
