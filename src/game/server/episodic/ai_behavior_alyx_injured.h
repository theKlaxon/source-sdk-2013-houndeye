//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: FIXME: This will ultimately become a more generic implementation
//
//=============================================================================
#pragma once
#include "utlmap.h"

extern bool IsAlyxInInjuredMode();

class CAI_InjuredFollowGoal : public CAI_FollowGoal {
	DECLARE_CLASS( CAI_InjuredFollowGoal, CAI_FollowGoal );

public:
	virtual void EnableGoal( CAI_BaseNPC* pAI );
	virtual void DisableGoal( CAI_BaseNPC* pAI );

	DECLARE_DATADESC();
};

class CAI_BehaviorAlyxInjured : public CAI_FollowBehavior {
	DECLARE_CLASS( CAI_BehaviorAlyxInjured, CAI_FollowBehavior );
	DECLARE_DATADESC();

public:
	CAI_BehaviorAlyxInjured();

	virtual const char* GetName() { return "AlyxInjuredFollow"; }
	virtual Activity NPC_TranslateActivity( Activity nActivity );
	virtual int TranslateSchedule( int scheduleType );
	virtual void Spawn();
	virtual void OnRestore();
	virtual void StartTask( const Task_t* pTask );
	virtual int SelectFailSchedule( int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode );
	virtual void GatherConditions();
	virtual Activity GetFlinchActivity( bool bHeavyDamage, bool bGesture );

	enum {
		// Schedules
		SCHED_INJURED_COWER = BaseClass::NEXT_SCHEDULE,
		SCHED_INJURED_FEAR_FACE,
		SCHED_INJURED_RUN_FROM_ENEMY,
		NEXT_SCHEDULE,

		// Tasks
		TASK_FIND_INJURED_COVER_FROM_ENEMY = BaseClass::NEXT_TASK,
		NEXT_TASK,

		// Conditions
		COND_INJURED_TOO_FAR_FROM_PLAYER = BaseClass::NEXT_CONDITION,
		COND_INJURED_OVERWHELMED,
		NEXT_CONDITION
	};

	bool IsReadinessCapable() { return ( IsInjured() == false ); }// Never use the readiness system when injured
	bool IsInjured() const;

private:
	void SpeakIfAllowed( AIConcept_t concept );
	bool ShouldRunToCover();
	bool ShouldRunToFollowGoal();
	bool FindThreatDirection2D( const Vector& vecSource, Vector* vecOut );
	bool FindCoverFromEnemyBehindTarget( CBaseEntity* pTarget, float flRadius, Vector* vecOut );
	void PopulateActivityMap();
	int NumKnownEnemiesInRadius( const Vector& vecSource, float flRadius );

	CUtlMap<Activity, Activity> m_ActivityMap;

	float m_flNextWarnTime;

protected:
	DEFINE_CUSTOM_SCHEDULE_PROVIDER;
};
