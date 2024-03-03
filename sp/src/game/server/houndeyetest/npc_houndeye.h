#pragma once
#include "ai_basenpc.h"
#include "ai_blended_movement.h"
#include "ai_behavior_follow.h"
#include "npcevent.h"

// CAI_BlendingHost< CAI_BehaviorHost<CAI_BlendedNPC> >
static ConVar sk_heye_attack_range("sk_heye_attack_range", "175.0f", FCVAR_DEVELOPMENTONLY);

class CNPC_Houndeye : public CAI_BaseNPC {
	DECLARE_CLASS(CNPC_Houndeye, CAI_BaseNPC)
	DECLARE_DATADESC()
	DEFINE_CUSTOM_AI
public:

	CNPC_Houndeye();

	void Precache();
	void Spawn();
	void Activate();
	
	void NPCThink();
	void GatherConditions();
	void StartTask(const Task_t* pTask);
	void RunTask(const Task_t* pTask);
	void HandleAnimEvent(animevent_t* pEvent);

	int RangeAttack1Conditions(float flDot, float flDist);
	int SelectSchedule();

	void WakeSound();
	void FoundEnemySound();
	void DoShockwave();

	Class_T Classify() { return CLASS_HOUNDEYE; }
	virtual float		InnateRange1MaxRange(void) override { return sk_heye_attack_range.GetFloat(); }
	
	float m_flDistTooFar = 800.0f;

private:

	int SelectFailSchedule(int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode);
	int TranslateSchedule(int nType);

	// conditions
	enum {
		COND_HEYE_HEALTH_LOW = LAST_SHARED_CONDITION,
		COND_HEYE_EEPY,
		COND_HEYE_SPOOKED, // used to wake the houndeye by setting the spooked sched
		COND_HEYE_LEADER_DEAD,
		COND_HEYE_HAS_WAYPOINT,
		COND_HEYE_ENEMY_TOO_FAR,
	};

	// tasks
	enum {
		// doing anims
		TASK_HEYE_BEEP = LAST_SHARED_TASK,
		TASK_HEYE_SPOOK_AWAKE,
		TASK_HEYE_START_SNOOZE,
		TASK_HEYE_OPEN_EYE,
		TASK_HEYE_CLOSE_EYE,
		
		// attacking
		TASK_SURROUND,
		TASK_HEYE_DO_SHOCKWAVE,
	};

	// schedules
	enum {
		SCHED_HEYE_SNOOZE = LAST_SHARED_SCHEDULE,
		SCHED_HEYE_START_SNOOZE,
		SCHED_HEYE_SPOOKED_AWAKE,
		SCHED_HEYE_HUNT,
		SCHED_HEYE_ATTACK,
				
		SCHED_HEYE_WATCH_WAPOINT,		// watching the TV
		SCHED_HEYE_INSPECT_WAYPOINT,	// inspecting the body
	};

	// blinking
	enum {
		BLINK_OPENING,
		BLINK_CLOSING,
		BLINK_WAITING,
		BLINK_STOPPED
	} m_nBlinkState, m_nPrevBlink;

	float m_flBlinkTime;
	bool m_bIsSleeping;
};