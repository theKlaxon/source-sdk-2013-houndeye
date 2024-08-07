//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: CNPC_Hydra
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "ai_basenpc.h"
#include "soundenvelope.h"

class CNPC_Hydra;

class HydraBone {
public:
	HydraBone( void ) {
		vecPos = Vector( 0, 0, 0 );
		vecDelta = Vector( 0, 0, 0 );
		//vecBendDelta = Vector( 0, 0, 0 );
		//vecGoalDelta = Vector( 0, 0, 0 );
		//flBendTension = 0.0;
		flIdealLength = 1.0;
		flGoalInfluence = 0.0;
		bStuck = false;
		bOnFire = false;
	};

	Vector vecPos;
	Vector vecDelta;
	//float flBendTension;
	float flIdealLength;
	bool bStuck;
	bool bOnFire;

	float flActualLength;
	//Vector	vecBendDelta;
	//Vector	vecGoalDelta;
	// float	flAccumLength;

	Vector vecGoalPos;
	float flGoalInfluence;

	DECLARE_SIMPLE_DATADESC();
};

class CNPC_Hydra : public CAI_BaseNPC {
	DECLARE_CLASS( CNPC_Hydra, CAI_BaseNPC );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

public:
	CNPC_Hydra() { }

	void Spawn();
	void Precache();
	void Activate();

	Class_T Classify();

	void RunAI();

	float MaxYawSpeed();
	int TranslateSchedule( int scheduleType );
	int SelectSchedule();

	void PrescheduleThink();

	void HandleAnimEvent( animevent_t* pEvent );

	void StartTask( const Task_t* pTask );
	void RunTask( const Task_t* pTask );

#define CHAIN_LINKS 32

	CNetworkArray( Vector, m_vecChain, CHAIN_LINKS );
	int m_activeChain;

	bool m_bHasStuckSegments;
	float m_flCurrentLength;

	Vector m_vecHeadGoal;
	float m_flHeadGoalInfluence;
	CNetworkVector( m_vecHeadDir );

	CNetworkVar( float, m_flRelaxedLength );

	Vector m_vecOutward;

	CUtlVector<HydraBone> m_body;

	float m_idealLength;
	float m_idealSegmentLength;

	Vector TestPosition( float t );

	void CalcGoalForces();
	void MoveBody();

	void AdjustLength();
	void CheckLength();

	bool m_bExtendSoundActive;
	CSoundPatch* m_pExtendTentacleSound;

	void Nudge( CBaseEntity* pHitEntity, const Vector& vecContact, const Vector& vecSpeed );
	void Stab( CBaseEntity* pHitEntity, const Vector& vecSpeed, trace_t& ptr );
	void Kick( CBaseEntity* pHitEntity, const Vector& vecContact, const Vector& vecSpeed );
	void Splash( const Vector& vecSplashPos );

	// float		FreeNeckLength( void );

	virtual Vector EyePosition();
	virtual const QAngle& EyeAngles();

	virtual Vector BodyTarget( const Vector& posSrc, bool bNoisy );

	void AimHeadInTravelDirection( float flInfluence );

	float m_seed;

	// --------------------------------
	Vector m_vecTarget;
	Vector m_vecTargetDir;

	float m_flLastAdjustmentTime;
	float m_flTaskStartTime;
	float m_flTaskEndTime;

	float m_flLengthTime;// time of last successful length adjustment time

	// --------------------------------

	bool ContractFromHead();
	bool ContractBetweenStuckSegments();
	bool ContractFromRoot();

	int VirtualRoot();

	bool AddNodeBefore( int iNode );
	bool AddNodeAfter( int iNode );

	bool GrowFromVirtualRoot();
	bool GrowFromMostStretched();

	void CalcRelaxedLength();

	bool IsValidConnection( int iNode0, int iNode1 );

	void AttachStabbedEntity( CBaseAnimating* pAnimating, Vector vecForce, trace_t& tr );
	void UpdateStabbedEntity( void );
	void DetachStabbedEntity( bool playSound );
	void GetDesiredImpaledPosition( Vector* vecOrigin, QAngle* vecAngles );

	bool m_bStabbedEntity;

	DEFINE_CUSTOM_AI;

private:
};
