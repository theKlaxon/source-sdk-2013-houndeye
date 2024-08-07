//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Teamplay game rules that manage a round based structure for you
//
//=============================================================================
#pragma once
#include "GameEventListener.h"
#include "teamplay_gamerules.h"
#include "teamplay_round_timer.h"

#ifdef GAME_DLL
	#include "team_control_point.h"
	extern ConVar mp_respawnwavetime;
	extern ConVar mp_showroundtransitions;
	extern ConVar mp_enableroundwaittime;
	extern ConVar mp_showcleanedupents;
	extern ConVar mp_bonusroundtime;
	extern ConVar mp_restartround;
	extern ConVar mp_winlimit;
	extern ConVar mp_maxrounds;
	extern ConVar mp_stalemate_timelimit;
	extern ConVar mp_stalemate_enable;
#else
	#define CTeamplayRoundBasedRules C_TeamplayRoundBasedRules
	#define CTeamplayRoundBasedRulesProxy C_TeamplayRoundBasedRulesProxy
#endif

extern ConVar tf_arena_use_queue;
extern ConVar mp_stalemate_meleeonly;
extern ConVar mp_forceautoteam;

class CTeamplayRoundBasedRules;

//-----------------------------------------------------------------------------
// Round states
//-----------------------------------------------------------------------------
enum gamerules_roundstate_t {
	// initialize the game, create teams
	GR_STATE_INIT = 0,

	//Before players have joined the game. Periodically checks to see if enough players are ready
	//to start a game. Also reverts to this when there are no active players
	GR_STATE_PREGAME,

	//The game is about to start, wait a bit and spawn everyone
	GR_STATE_STARTGAME,

	//All players are respawned, frozen in place
	GR_STATE_PREROUND,

	//Round is on, playing normally
	GR_STATE_RND_RUNNING,

	//Someone has won the round
	GR_STATE_TEAM_WIN,

	//Noone has won, manually restart the game, reset scores
	GR_STATE_RESTART,

	//Noone has won, restart the game
	GR_STATE_STALEMATE,

	//Game is over, showing the scoreboard etc
	GR_STATE_GAME_OVER,

	//Game is in a bonus state, transitioned to after a round ends
	GR_STATE_BONUS,

	//Game is awaiting the next wave/round of a multi round experience
	GR_STATE_BETWEEN_RNDS,

	GR_NUM_ROUND_STATES
};

enum {
	WINREASON_NONE = 0,
	WINREASON_ALL_POINTS_CAPTURED,
	WINREASON_OPPONENTS_DEAD,
	WINREASON_FLAG_CAPTURE_LIMIT,
	WINREASON_DEFEND_UNTIL_TIME_LIMIT,
	WINREASON_STALEMATE,
	WINREASON_TIMELIMIT,
	WINREASON_WINLIMIT,
	WINREASON_WINDIFFLIMIT,
#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )
	WINREASON_RD_REACTOR_CAPTURED,
	WINREASON_RD_CORES_COLLECTED,
	WINREASON_RD_REACTOR_RETURNED,
#endif
};

enum stalemate_reasons_t {
	STALEMATE_JOIN_MID,
	STALEMATE_TIMER,
	STALEMATE_SERVER_TIMELIMIT,

	NUM_STALEMATE_REASONS,
};


#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )

/// Info about a player in a PVE game or any other mode that we
/// might eventually decide to use the lobby system for.
struct LobbyPlayerInfo_t {
	int m_nEntNum;           //< Index of player (1...MAX_PLAYERS), or 0 if the guy is in the lobby but not yet known to us
	CUtlString m_sPlayerName;//< Player display name
	CSteamID m_steamID;      //< Steam ID of the player
	int m_iTeam;             //< Team selection.
	bool m_bInLobby;         //< Is this guy in the lobby?
	bool m_bConnected;       //< Is this a bot?
	bool m_bBot;             //< Is this a bot?
	bool m_bSquadSurplus;    //< Did he present a voucher to get surplus for his squad
};

#endif

//-----------------------------------------------------------------------------
// Purpose: Per-state data
//-----------------------------------------------------------------------------
class CGameRulesRoundStateInfo {
public:
	gamerules_roundstate_t m_iRoundState;
	const char* m_pStateName;

	void ( CTeamplayRoundBasedRules::*pfnEnterState )();// Init and deinit the state.
	void ( CTeamplayRoundBasedRules::*pfnLeaveState )();
	void ( CTeamplayRoundBasedRules::*pfnThink )();// Do a PreThink() in this state.
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CTeamplayRoundBasedRulesProxy : public CGameRulesProxy {
public:
	DECLARE_CLASS( CTeamplayRoundBasedRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();

#ifdef GAME_DLL
	DECLARE_DATADESC();
	void InputSetStalemateOnTimelimit( inputdata_t& inputdata );
#endif

	//----------------------------------------------------------------------------------
	// Client specific
#ifdef CLIENT_DLL
	void OnPreDataChanged( DataUpdateType_t updateType );
	void OnDataChanged( DataUpdateType_t updateType );
#endif// CLIENT_DLL
};

//-----------------------------------------------------------------------------
// Purpose: Teamplay game rules that manage a round based structure for you
//-----------------------------------------------------------------------------
class CTeamplayRoundBasedRules : public CTeamplayRules, public CGameEventListener {
	DECLARE_CLASS( CTeamplayRoundBasedRules, CTeamplayRules );

public:
	CTeamplayRoundBasedRules();

#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE();// This makes datatables able to access our private vars.

	void SetRoundState( int iRoundState );
#else
	DECLARE_SERVERCLASS_NOBASE();// This makes datatables able to access our private vars.
#endif

	float GetLastRoundStateChangeTime() const { return m_flLastRoundStateChangeTime; }
	float m_flLastRoundStateChangeTime;

	// Data accessors
	inline gamerules_roundstate_t State_Get() { return m_iRoundState; }
	bool IsInWaitingForPlayers() { return m_bInWaitingForPlayers; }
	virtual bool InRoundRestart() { return State_Get() == GR_STATE_PREROUND; }
	bool InStalemate() { return State_Get() == GR_STATE_STALEMATE; }
	bool RoundHasBeenWon() { return State_Get() == GR_STATE_TEAM_WIN; }

	virtual float GetNextRespawnWave( int iTeam, CBasePlayer* pPlayer );
	virtual bool HasPassedMinRespawnTime( CBasePlayer* pPlayer );
	virtual void LevelInitPostEntity();
	virtual float GetRespawnTimeScalar( int iTeam );
	virtual float GetRespawnWaveMaxLength( int iTeam, bool bScaleWithNumPlayers = true );
	virtual bool ShouldRespawnQuickly( CBasePlayer* pPlayer ) { return false; }
	float GetMinTimeWhenPlayerMaySpawn( CBasePlayer* pPlayer );

	// Return false if players aren't allowed to cap points at this time (i.e. in WaitingForPlayers)
	virtual bool PointsMayBeCaptured() { return ( ( State_Get() == GR_STATE_RND_RUNNING || State_Get() == GR_STATE_STALEMATE ) && !IsInWaitingForPlayers() ); }
	virtual void SetLastCapPointChanged( int iIndex ) { m_iLastCapPointChanged = iIndex; }
	int GetLastCapPointChanged() { return m_iLastCapPointChanged; }

	virtual int GetWinningTeam() { return m_iWinningTeam; }
	int GetWinReason() { return m_iWinReason; }

	bool InOvertime() { return m_bInOvertime; }
	void SetOvertime( bool bOvertime );

	bool InSetup() { return m_bInSetup; }

	void BalanceTeams( bool bRequireSwitcheesToBeDead );

	bool SwitchedTeamsThisRound() { return m_bSwitchedTeamsThisRound; }

	virtual bool ShouldBalanceTeams();
	bool IsInTournamentMode();
	bool IsInHighlanderMode();
	bool IsInPreMatch() { return ( IsInTournamentMode() && IsInWaitingForPlayers() ); }
	bool IsWaitingForTeams() { return m_bAwaitingReadyRestart; }
	bool IsInStopWatch() { return m_bStopWatch; }
	void SetInStopWatch( bool bState ) { m_bStopWatch = bState; }
	virtual void StopWatchModeThink() { }

	bool IsTeamReady( int iTeamNumber ) {
		return m_bTeamReady[ iTeamNumber ];
	}

	bool IsPlayerReady( int iIndex ) {
		return m_bPlayerReady[ iIndex ];
	}

	virtual void HandleTeamScoreModify( int iTeam, int iScore ) { }


	float GetRoundRestartTime() { return m_flRestartRoundTime; }

	//Arena Mode
	virtual bool IsInArenaMode() { return false; }

	//Koth Mode
	virtual bool IsInKothMode() { return false; }

	//Training Mode
	virtual bool IsInTraining() { return false; }
	virtual bool IsInItemTestingMode() { return false; }

	void SetMultipleTrains( bool bMultipleTrains ) { m_bMultipleTrains = bMultipleTrains; }
	bool HasMultipleTrains() { return m_bMultipleTrains; }

	virtual int GetBonusRoundTime( bool bFinal = false );

#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )

	// Get list of all the players, including those in the lobby but who have
	// not yet joined.
	void GetAllPlayersLobbyInfo( CUtlVector<LobbyPlayerInfo_t>& vecPlayers, bool bIncludeBots = false );

	// Get list of players who are on the defending team now, or are likely
	// to end up on the defending team (not yet connected or assigned a team)
	void GetPotentialPlayersLobbyPlayerInfo( CUtlVector<LobbyPlayerInfo_t>& vecLobbyPlayers, bool bIncludeBots = false );

#endif

	void SetAllowBetweenRounds( bool bValue ) { m_bAllowBetweenRounds = bValue; }

public:// IGameEventListener Interface
	virtual void FireGameEvent( IGameEvent* event );

	//----------------------------------------------------------------------------------
	// Server specific
	#ifdef GAME_DLL
		// Derived game rules class should override these
	public:
		// Override this to prevent removal of game specific entities that need to persist
		virtual bool RoundCleanupShouldIgnore( CBaseEntity* pEnt );
		virtual bool ShouldCreateEntity( const char* pszClassName );

		// Called when a new round is being initialized
		virtual void SetupOnRoundStart() { return; }

		// Called when a new round is off and running
		virtual void SetupOnRoundRunning() { return; }

		// Called before a new round is started (so the previous round can end)
		virtual void PreviousRoundEnd() { return; }

		// Send the team scores down to the client
		virtual void SendTeamScoresEvent() { return; }

		// Send the end of round info displayed in the win panel
		virtual void SendWinPanelInfo() { return; }

		// Setup spawn points for the current round before it starts
		virtual void SetupSpawnPointsForRound() { return; }

		// Called when a round has entered stalemate mode (timer has run out)
		virtual void SetupOnStalemateStart() { return; }
		virtual void SetupOnStalemateEnd() { return; }
		virtual void SetSetup( bool bSetup );

		virtual bool ShouldGoToBonusRound() { return false; }
		virtual void SetupOnBonusStart() { return; }
		virtual void SetupOnBonusEnd() { return; }
		virtual void BonusStateThink() { return; }

		virtual void BetweenRounds_Start() { return; }
		virtual void BetweenRounds_End() { return; }
		virtual void BetweenRounds_Think() { return; }

		virtual void PreRound_End() { return; }

		bool PrevRoundWasWaitingForPlayers() { return m_bPrevRoundWasWaitingForPlayers; }

		virtual bool ShouldScorePerRound() { return true; }

		bool CheckNextLevelCvar( bool bAllowEnd = true );

		virtual bool TimerMayExpire();

		virtual bool IsValveMap() { return false; }

		virtual void RestartTournament();

		virtual bool TournamentModeCanEndWithTimelimit() { return true; }

	public:
		void State_Transition( gamerules_roundstate_t newState );

		void RespawnPlayers( bool bForceRespawn, bool bTeam = false, int iTeam = TEAM_UNASSIGNED );

		void SetForceMapReset( bool reset );

		void SetRoundToPlayNext( string_t strName ) { m_iszRoundToPlayNext = strName; }
		string_t GetRoundToPlayNext() { return m_iszRoundToPlayNext; }
		void AddPlayedRound( string_t strName );
		bool IsPreviouslyPlayedRound( string_t strName );
		string_t GetLastPlayedRound();

		virtual void SetWinningTeam( int team, int iWinReason, bool bForceMapReset = true, bool bSwitchTeams = false, bool bDontAddScore = false, bool bFinal = false ) override;
		virtual void SetStalemate( int iReason, bool bForceMapReset = true, bool bSwitchTeams = false );

		virtual void SetRoundOverlayDetails() { return; }

		virtual float GetWaitingForPlayersTime() { return mp_waitingforplayers_time.GetFloat(); }
		void ShouldResetScores( bool bResetTeam, bool bResetPlayer ) {
			m_bResetTeamScores = bResetTeam;
			m_bResetPlayerScores = bResetPlayer;
		}
		void ShouldResetRoundsPlayed( bool bResetRoundsPlayed ) { m_bResetRoundsPlayed = bResetRoundsPlayed; }

		void SetFirstRoundPlayed( string_t strName ) { m_iszFirstRoundPlayed = strName; }
		string_t GetFirstRoundPlayed() { return m_iszFirstRoundPlayed; }

		void SetTeamRespawnWaveTime( int iTeam, float flValue );
		void AddTeamRespawnWaveTime( int iTeam, float flValue );
		virtual void FillOutTeamplayRoundWinEvent( IGameEvent* event ) {}// derived classes may implement to add fields to this event

		void SetStalemateOnTimelimit( bool bStalemate ) { m_bAllowStalemateAtTimelimit = bStalemate; }

		bool IsGameUnderTimeLimit();

		CTeamRoundTimer* GetActiveRoundTimer();

		void HandleTimeLimitChange();

		void SetTeamReadyState( bool bState, int iTeam ) {
			m_bTeamReady.Set( iTeam, bState );
		}

		void SetPlayerReadyState( int iIndex, bool bState ) {
			m_bPlayerReady.Set( iIndex, bState );
		}
		void ResetPlayerAndTeamReadyState();

		virtual void PlayTrainCaptureAlert( CTeamControlPoint* pPoint, bool bFinalPointInMap ) { return; }

		virtual void PlaySpecialCapSounds( int iCappingTeam, CTeamControlPoint* pPoint ) { return; }

		bool PlayThrottledAlert( int iTeam, const char* sound, float fDelayBeforeNext );

		void BroadcastSound( int iTeam, const char* sound, int iAdditionalSoundFlags = 0 );
		int GetRoundsPlayed() { return m_nRoundsPlayed; }

		virtual void RecalculateControlPointState() { return; }

		virtual bool ShouldSkipAutoScramble() { return false; }

		virtual bool ShouldWaitToStartRecording() { return IsInWaitingForPlayers(); }

	protected:
		virtual void Think();

		virtual void CheckChatText( CBasePlayer* pPlayer, char* pText );
		void CheckChatForReadySignal( CBasePlayer* pPlayer, const char* chatmsg );

		// Game beginning / end handling
		virtual void GoToIntermission();
		void SetInWaitingForPlayers( bool bWaitingForPlayers );
		void CheckWaitingForPlayers();
		virtual bool AllowWaitingForPlayers() { return true; }
		void CheckRestartRound();
		bool CheckTimeLimit( bool bAllowEnd = true );
		int GetTimeLeft();
		virtual bool CheckWinLimit( bool bAllowEnd = true );
		bool CheckMaxRounds( bool bAllowEnd = true );

		void CheckReadyRestart();
		#if defined( TF_CLIENT_DLL ) || defined( TF_DLL )
		bool AreLobbyPlayersOnTeamReady( int iTeam );
		bool AreLobbyPlayersConnected();
		#endif

		virtual bool CanChangelevelBecauseOfTimeLimit() { return true; }
		virtual bool CanGoToStalemate() { return true; }

		// State machine handling
		void State_Enter( gamerules_roundstate_t newState );                              // Initialize the new state.
		void State_Leave();                                                               // Cleanup the previous state.
		void State_Think();                                                               // Update the current state.
		static CGameRulesRoundStateInfo* State_LookupInfo( gamerules_roundstate_t state );// Find the state info for the specified state.

		// State Functions
		void State_Enter_INIT();
		void State_Think_INIT();

		void State_Enter_PREGAME();
		void State_Think_PREGAME();

		void State_Enter_STARTGAME();
		void State_Think_STARTGAME();

		void State_Enter_PREROUND();
		void State_Leave_PREROUND();
		void State_Think_PREROUND();

		void State_Enter_RND_RUNNING();
		void State_Think_RND_RUNNING();

		void State_Enter_TEAM_WIN();
		void State_Think_TEAM_WIN();

		void State_Enter_RESTART();
		void State_Think_RESTART();

		void State_Enter_STALEMATE();
		void State_Think_STALEMATE();
		void State_Leave_STALEMATE();

		void State_Enter_BONUS();
		void State_Think_BONUS();
		void State_Leave_BONUS();

		void State_Enter_BETWEEN_RNDS();
		void State_Leave_BETWEEN_RNDS();
		void State_Think_BETWEEN_RNDS();

		// mp_scrambleteams_auto
		void ResetTeamsRoundWinTracking();

	protected:
		virtual void InitTeams();
		virtual int CountActivePlayers();

		virtual void RoundRespawn();
		virtual void CleanUpMap();
		virtual void CheckRespawnWaves();
		void ResetScores();
		void ResetMapTime();

		void PlayStartRoundVoice();
		void PlayWinSong( int team );
		void PlayStalemateSong();
		void PlaySuddenDeathSong();

		virtual const char* GetStalemateSong( int nTeam ) { return "Game.Stalemate"; }
		virtual const char* WinSongName( int nTeam ) { return "Game.YourTeamWon"; }
		virtual const char* LoseSongName( int nTeam ) { return "Game.YourTeamLost"; }

		virtual void RespawnTeam( int iTeam ) { RespawnPlayers( false, true, iTeam ); }

		void HideActiveTimer();
		virtual void RestoreActiveTimer();

		virtual void InternalHandleTeamWin( int iWinningTeam ) { return; }

		bool MapHasActiveTimer();
		void CreateTimeLimitTimer();

		virtual float GetLastMajorEventTime() override { return m_flLastTeamWin; }

	protected:
		CGameRulesRoundStateInfo* m_pCurStateInfo;// Per-state data
		float m_flStateTransitionTime;            // Timer for round states

		float m_flWaitingForPlayersTimeEnds;
		CHandle<CTeamRoundTimer> m_hWaitingForPlayersTimer;

		float m_flNextPeriodicThink;
		bool m_bChangeLevelOnRoundEnd;

		bool m_bResetTeamScores;
		bool m_bResetPlayerScores;
		bool m_bResetRoundsPlayed;

		// Stalemate
		EHANDLE m_hPreviousActiveTimer;
		CHandle<CTeamRoundTimer> m_hStalemateTimer;
		float m_flStalemateStartTime;

		CHandle<CTeamRoundTimer> m_hTimeLimitTimer;

		bool m_bForceMapReset;                // should the map be reset when a team wins and the round is restarted?
		bool m_bPrevRoundWasWaitingForPlayers;// was the previous map reset after a waiting for players period
		bool m_bInitialSpawn;

		string_t m_iszRoundToPlayNext;
		CUtlVector<string_t> m_iszPreviousRounds;// we'll store the two previous rounds so we won't play them again right away if there are other rounds that can be played first
		string_t m_iszFirstRoundPlayed;          // store the first round played after a full restart so we can pick a different one next time if we have other options

		float m_flOriginalTeamRespawnWaveTime[ MAX_TEAMS ];

		bool m_bAllowStalemateAtTimelimit;
		bool m_bChangelevelAfterStalemate;

		float m_flRoundStartTime;       // time the current round started
		float m_flNewThrottledAlertTime;// time that we can play another throttled alert

		int m_nRoundsPlayed;
		bool m_bUseAddScoreAnim;

		gamerules_roundstate_t m_prevState;

		bool m_bPlayerReadyBefore[ MAX_PLAYERS + 1 ];// Test to see if a player has hit ready before

		float m_flLastTeamWin;

	private:
		CUtlMap<int, int> m_GameTeams;// Team index, Score
	#endif
	// End server specific
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	// Client specific
	#ifdef CLIENT_DLL
	public:
		virtual void OnPreDataChanged( DataUpdateType_t updateType );
		virtual void OnDataChanged( DataUpdateType_t updateType );
		virtual void HandleOvertimeBegin() {}
		virtual void GetTeamGlowColor( int nTeam, float& r, float& g, float& b ) {
			r = 0.76f;
			g = 0.76f;
			b = 0.76f;
		}

	private:
		bool m_bOldInWaitingForPlayers;
		bool m_bOldInOvertime;
		bool m_bOldInSetup;
	#endif// CLIENT_DLL

public:
	bool WouldChangeUnbalanceTeams( int iNewTeam, int iCurrentTeam );
	bool AreTeamsUnbalanced( int& iHeaviestTeam, int& iLightestTeam );
	virtual bool HaveCheatsBeenEnabledDuringLevel() { return m_bCheatsEnabledDuringLevel; }

protected:
	CNetworkVar( gamerules_roundstate_t, m_iRoundState );
	CNetworkVar( bool, m_bInOvertime );// Are we currently in overtime?
	CNetworkVar( bool, m_bInSetup );   // Are we currently in setup?
	CNetworkVar( bool, m_bSwitchedTeamsThisRound );

protected:
	CNetworkVar( int, m_iWinningTeam );// Set before entering GR_STATE_TEAM_WIN
	CNetworkVar( int, m_iWinReason );
	CNetworkVar( bool, m_bInWaitingForPlayers );
	CNetworkVar( bool, m_bAwaitingReadyRestart );
	CNetworkVar( float, m_flRestartRoundTime );
	CNetworkVar( float, m_flMapResetTime );                // Time that the map was reset
	CNetworkArray( float, m_flNextRespawnWave, MAX_TEAMS );// Minor waste, but cleaner code
	CNetworkArray( bool, m_bTeamReady, MAX_TEAMS );
	CNetworkVar( bool, m_bStopWatch );
	CNetworkVar( bool, m_bMultipleTrains );// two trains in this map?
	CNetworkArray( bool, m_bPlayerReady, MAX_PLAYERS );
	CNetworkVar( bool, m_bCheatsEnabledDuringLevel );

public:
	CNetworkArray( float, m_TeamRespawnWaveTimes, MAX_TEAMS );// Time between each team's respawn wave

private:
	float m_flStartBalancingTeamsAt;
	float m_flNextBalanceTeamsTime;
	bool m_bPrintedUnbalanceWarning;
	float m_flFoundUnbalancedTeamsTime;

	float m_flAutoBalanceQueueTimeEnd;
	int m_nAutoBalanceQueuePlayerIndex;
	int m_nAutoBalanceQueuePlayerScore;

protected:
	bool m_bAllowBetweenRounds;

public:
	float m_flStopWatchTotalTime;
	int m_iLastCapPointChanged;
};

// Utility function
bool FindInList( const char** pStrings, const char* pToFind );

inline CTeamplayRoundBasedRules* TeamplayRoundBasedRules() {
	return static_cast<CTeamplayRoundBasedRules*>( g_pGameRules );
}
