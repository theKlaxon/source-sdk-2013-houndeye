//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basegrenade_shared.h"

class CBeam;


class CTripmineGrenade : public CBaseGrenade {
public:
	DECLARE_CLASS( CTripmineGrenade, CBaseGrenade );

	CTripmineGrenade();
	void Spawn();
	void Precache();

	int OnTakeDamage_Alive( const CTakeDamageInfo& info );

	void WarningThink();
	void PowerupThink();
	void BeamBreakThink();
	void DelayDeathThink();
	void Event_Killed( const CTakeDamageInfo& info );

	void MakeBeam();
	void KillBeam();

public:
	EHANDLE m_hOwner;

private:
	float m_flPowerUp;
	Vector m_vecDir;
	Vector m_vecEnd;
	float m_flBeamLength;

	CBeam* m_pBeam;
	Vector m_posOwner;
	Vector m_angleOwner;

	DECLARE_DATADESC();
};
