#pragma once
#include "ai_basenpc.h"
#include "ai_blended_movement.h"
#include "ai_behavior_follow.h"
#include "ai_squadslot.h"
#include "npcevent.h"
#include "point_houndeye.h"

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
		COND_HEYE_SQUAD_ALERT,
		TASK_HEYE_CHECK_FOR_SQUAD,
	};

	// tasks
	enum {
		// doing anims
		TASK_HEYE_BEEP = LAST_SHARED_TASK,
		TASK_HEYE_SPOOK_AWAKE,
		TASK_HEYE_START_SNOOZE,
		
		// attacking
		TASK_HEYE_PLOT_ATTACK,
		TASK_HEYE_DO_SHOCKWAVE,

		// waypoint tasks
		TASK_HEYE_GET_POINT_TASK,
		TASK_HEYE_SIT_AND_STAY
	};

	// task fails
	enum {
		FAIL_HEYE_NO_SQUAD = NUM_FAIL_CODES,
	};

	// schedules
	enum {
		// sleeping
		SCHED_HEYE_SNOOZE = LAST_SHARED_SCHEDULE,
		SCHED_HEYE_START_SNOOZE,
		SCHED_HEYE_SPOOKED_AWAKE,

		// attacking
		SCHED_HEYE_HUNT,
		SCHED_HEYE_PLOTTING,
		SCHED_HEYE_ATTACK,

		// waypoint schedules
		SCHED_HEYE_MOVE_TO_WAYPOINT,	// move to the waypoint	
		SCHED_HEYE_WATCH_WAPOINT,		// watching the TV
		SCHED_HEYE_INSPECT_WAYPOINT,	// inspecting the body
	};

	// squad // goal is to have 2 attacking while the others run around
	enum {
		SQUADSLOT_HEYE_ATTACK1 = LAST_SHARED_SQUADSLOT,
		SQUADSLOT_HEYE_ATTACK2,
	};

	// blinking
	enum {
		BLINK_OPENING,
		BLINK_CLOSING,
		BLINK_WAITING,
		BLINK_STOPPED
	} m_nBlinkState, m_nPrevBlink;

	// sleeping
	enum {
		SLEEP_NOT_SLEEPING,
		SLEEP_STARTED_SLEEPING,
		SLEEP_IS_SLEEPING,
	} m_nSleepState;

	float m_flBlinkTime;
	bool m_bSitting;
	bool m_bPlotting;

	CHoundeyePoint* m_pWaypoint;
};