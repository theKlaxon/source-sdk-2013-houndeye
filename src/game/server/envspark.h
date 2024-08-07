//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A point entity that periodically emits sparks and "bzzt" sounds.
//
// $NoKeywords: $
//=============================================================================//
#pragma once


class CEnvSpark : public CPointEntity {
	DECLARE_CLASS( CEnvSpark, CPointEntity );
public:
	CEnvSpark();

	void Spawn();
	void Precache();
	void SparkThink();

	void StartSpark();
	void StopSpark();

	// Input handlers
	void InputStartSpark( inputdata_t& inputdata );
	void InputStopSpark( inputdata_t& inputdata );
	void InputToggleSpark( inputdata_t& inputdata );
	void InputSparkOnce( inputdata_t& inputdata );

	bool IsSparking() { return ( GetNextThink() != TICK_NEVER_THINK ); }

	DECLARE_DATADESC();

	float m_flDelay;
	int m_nGlowSpriteIndex;
	int m_nMagnitude;
	int m_nTrailLength;

	COutputEvent m_OnSpark;
};
