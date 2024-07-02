//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once

#include "team.h"
#include "team_control_point.h"
#include "team_control_point_round.h"
#include "team_objectiveresource.h"
#include "teamplay_gamerules.h"
#include "trigger_area_capture.h"
#include "utlmap.h"

#define CPM_THINK "CTeamControlPointMasterCPMThink"
#define CPM_POSTINITTHINK "CTeamControlPointMasterCPMPostInitThink"

//-----------------------------------------------------------------------------
// Purpose: One ControlPointMaster is spawned per level. Shortly after spawning it detects all the Control
// points in the map and puts them into the m_ControlPoints. From there it detects the state
// where all points are captured and resets them if necessary It gives points every time interval to
// the owners of the points
//-----------------------------------------------------------------------------
class CTeamControlPointMaster : public CBaseEntity {
	DECLARE_CLASS( CTeamControlPointMaster, CBaseEntity );

	// Derived, game-specific control point masters must override these functions
public:
	CTeamControlPointMaster();

	// Used to find game specific entities
	virtual const char* GetControlPointName() { return "team_control_point"; }
	virtual const char* GetControlPointRoundName() { return "team_control_point_round"; }

public:
	void Spawn() override;
	void UpdateOnRemove() override;
	bool KeyValue( const char* szKeyName, const char* szValue ) override;
	void Precache() override;
	void Activate() override;

	void RoundRespawn();
	void Reset();

	int GetNumPoints() { return m_ControlPoints.Count(); }
	int GetNumPointsOwnedByTeam( int iTeam );
	int CalcNumRoundsRemaining( int iTeam );

	[[nodiscard]]
	bool IsActive() const { return ( m_bDisabled == false ); }

	void FireTeamWinOutput( int iWinningTeam );

	bool IsInRound( CTeamControlPoint* pPoint );
	void CheckWinConditions();

	bool WouldNewCPOwnerWinGame( CTeamControlPoint* pPoint, int iNewOwner );

	int GetBaseControlPoint( int iTeam );
	bool IsBaseControlPoint( int iPointIndex );

	bool PlayingMiniRounds() { return ( m_ControlPointRounds.Count() > 0 ); }

	float PointLastContestedAt( int point );
	CTeamControlPoint* GetControlPoint( int point ) {
		Assert( point >= 0 );
		Assert( point < MAX_CONTROL_POINTS );

		for ( int i = 0; i < m_ControlPoints.Count(); i++ ) {
			CTeamControlPoint *pPoint = m_ControlPoints[i];
			if ( pPoint && pPoint->GetPointIndex() == point )
				return pPoint;
		}

		return nullptr;
	}

	CTeamControlPointRound* GetCurrentRound() {
		if ( !PlayingMiniRounds() || m_iCurrentRoundIndex == -1 ) {
			return nullptr;
		}

		return m_ControlPointRounds[ m_iCurrentRoundIndex ];
	}

	string_t GetRoundToUseAfterRestart() {
		int nCurrentPriority = -1;
		int nHighestPriority = -1;

		string_t nRetVal = NULL_STRING;

		if ( PlayingMiniRounds() && GetCurrentRound() ) {
			nCurrentPriority = GetCurrentRound()->GetPriorityValue();
			nHighestPriority = GetHighestRoundPriorityValue();

			// if the current round has the highest priority, then use it again
			if ( nCurrentPriority == nHighestPriority ) {
				nRetVal = GetCurrentRound()->GetEntityName();
			}
		}

		return nRetVal;
	}

	void FireRoundStartOutput();
	void FireRoundEndOutput();

	[[nodiscard]]
	bool ShouldScorePerCapture() const { return m_bScorePerCapture; }
	[[nodiscard]]
	bool ShouldPlayAllControlPointRounds() const { return m_bPlayAllRounds; }
	int NumPlayableControlPointRounds();// checks to see if there are any more rounds to play (but doesn't actually "get" one to play)

#ifdef STAGING_ONLY
	void ListRounds();
#endif

	[[nodiscard]]
	float GetPartialCapturePointRate() const;

	void SetLastOwnershipChangeTime( float m_flTime ) { m_flLastOwnershipChangeTime = m_flTime; }
	[[nodiscard]]
	float GetLastOwnershipChangeTime() const { return m_flLastOwnershipChangeTime; }

	[[nodiscard]]
	int GetCurrentRoundIndex() const { return m_iCurrentRoundIndex; }
	[[nodiscard]]
	bool ShouldSwitchTeamsOnRoundWin() const { return m_bSwitchTeamsOnWin; }
private:
	void EXPORT CPMThink();

	void SetBaseControlPoints();
	int TeamOwnsAllPoints( CTeamControlPoint* pOverridePoint = nullptr, int iOverrideNewTeam = TEAM_UNASSIGNED );

	bool FindControlPoints();         // look in the map to find active control points
	bool FindControlPointRounds();    // look in the map to find active control point rounds
	bool GetControlPointRoundToPlay();// gets the next round we should play
	bool SelectSpecificRound();       // selects a specific round to play

	int GetHighestRoundPriorityValue() {
		int nRetVal = -1;

		// rounds are sorted with the higher priority rounds first
		for ( int i = 0; i < m_ControlPointRounds.Count(); ++i ) {
			CTeamControlPointRound* pRound = m_ControlPointRounds[ i ];

			if ( pRound ) {
				if ( pRound->GetPriorityValue() > nRetVal ) {
					nRetVal = pRound->GetPriorityValue();
				}
			}
		}

		return nRetVal;
	}

	void RegisterRoundBeingPlayed();

	CUtlMap<int, CTeamControlPoint*> m_ControlPoints;

	bool m_bFoundPoints;// true when the control points have been found and the array is initialized

	CUtlVector<CTeamControlPointRound*> m_ControlPointRounds;
	int m_iCurrentRoundIndex;

	DECLARE_DATADESC();

	bool m_bDisabled;
	void InputEnable( inputdata_t& inputdata );
	void InputDisable( inputdata_t& inputdata );

	void InputRoundSpawn( inputdata_t& inputdata );
	void InputRoundActivate( inputdata_t& inputdata );
	void InputSetWinner( inputdata_t& inputdata );
	void InputSetWinnerAndForceCaps( inputdata_t& inputdata );
	void InputSetCapLayout( inputdata_t& inputdata );
	void InputSetCapLayoutCustomPositionX( inputdata_t& inputdata );
	void InputSetCapLayoutCustomPositionY( inputdata_t& inputdata );

	void InternalSetWinner( int iTeam );

	void HandleRandomOwnerControlPoints();

	string_t m_iszTeamBaseIcons[ MAX_TEAMS ];
	int m_iTeamBaseIcons[ MAX_TEAMS ];
	string_t m_iszCapLayoutInHUD;

	float m_flCustomPositionX;
	float m_flCustomPositionY;

	int m_iInvalidCapWinner;
	bool m_bSwitchTeamsOnWin;
	bool m_bScorePerCapture;
	bool m_bPlayAllRounds;

	bool m_bFirstRoundAfterRestart;

	COutputEvent m_OnWonByTeam1;
	COutputEvent m_OnWonByTeam2;

	float m_flPartialCapturePointsRate;
	float m_flLastOwnershipChangeTime;
};

extern CUtlVector<CHandle<CTeamControlPointMaster>> g_hControlPointMasters;
