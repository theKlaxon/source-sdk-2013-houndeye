//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once
#include "team.h"
#include "team_control_point.h"
#include "team_objectiveresource.h"
#include "teamplay_gamerules.h"
#include "trigger_area_capture.h"
#include "utlmap.h"


class CTeamControlPointRound : public CBaseEntity {
	DECLARE_CLASS( CTeamControlPointRound, CBaseEntity );

public:
	virtual void Spawn();
	virtual void Activate();

	bool IsDisabled() { return m_bDisabled; }

	int GetPointOwner( int point );
	//	int CountAdvantageFlags( int team );
	bool WouldNewCPOwnerWinGame( CTeamControlPoint* pPoint, int iNewOwner );

	void FireTeamWinOutput( int iWinningTeam );

	void SelectedToPlay();

	int CheckWinConditions();// returns the team number of the team that's won, or returns -1 if no winner

	int GetPriorityValue() const { return m_nPriority; }

	bool IsPlayable();
	bool MakePlayable();
	bool IsControlPointInRound( CTeamControlPoint* pPoint );

	void FireOnStartOutput();
	void FireOnEndOutput();

	inline const char* GetName() { return STRING( m_iszPrintName ); }

	CHandle<CTeamControlPoint> GetPointOwnedBy( int iTeam );

	bool RoundOwnedByTeam( int iTeam ) { return ( TeamOwnsAllPoints() == iTeam ); }
	int GetInvalidCapWinner() { return m_iInvalidCapWinner; }

	CUtlVector<CHandle<CTeamControlPoint>> m_ControlPoints;

private:
	void FindControlPoints();//look in the map to find the control points for this round
	void SetupSpawnPoints();
	int TeamOwnsAllPoints( CTeamControlPoint* pOverridePoint = nullptr, int iOverrideNewTeam = TEAM_UNASSIGNED );

	DECLARE_DATADESC();

	bool m_bDisabled;
	void InputEnable( inputdata_t& inputdata );
	void InputDisable( inputdata_t& inputdata );

	void InputRoundSpawn( inputdata_t& inputdata );

	string_t m_iszCPNames;
	int m_nPriority;
	int m_iInvalidCapWinner;
	string_t m_iszPrintName;

	COutputEvent m_OnStart;
	COutputEvent m_OnEnd;
	COutputEvent m_OnWonByTeam1;
	COutputEvent m_OnWonByTeam2;
};
