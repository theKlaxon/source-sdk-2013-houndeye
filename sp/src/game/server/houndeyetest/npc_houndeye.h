#pragma once
#include "ai_basenpc.h"
#include "ai_blended_movement.h"
#include "ai_behavior_follow.h"
#include "ai_squadslot.h"
#include "npcevent.h"
#include "point_houndeye.h"

// CAI_BlendingHost< CAI_BehaviorHost<CAI_BlendedNPC> >
static ConVar sk_heye_attack_range("sk_heye_attack_range", "200.0f", FCVAR_DEVELOPMENTONLY); // TODO: redo name here to match the rest

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
	virtual float InnateRange1MaxRange(void) override { return sk_heye_attack_range.GetFloat(); }

	virtual int	OnTakeDamage_Alive(const CTakeDamageInfo& info);
	virtual int	OnTakeDamage_Dying(const CTakeDamageInfo& info);
	virtual int	OnTakeDamage_Dead(const CTakeDamageInfo& info);

	virtual void Event_Killed(const CTakeDamageInfo& info);

	void TraceAttack(const CTakeDamageInfo& info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator);

	//float m_flDistTooFar = 800.0f;

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
		TASK_HEYE_CHECK_FOR_SQUAD,

		// waypoint tasks / anims
		TASK_HEYE_SET_AT_POINT,
		TASK_HEYE_ANIM_WATCH,
		TASK_HEYE_ANIM_INSPECT,
		TASK_HEYE_ANIM_COWER,

		// misc.
		TASK_HEYE_RETREAT,
		TASK_HEYE_START_HUNT,
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
		SCHED_HEYE_TAUNT,
		SCHED_HEYE_ATTACK,
		SCHED_HEYE_CANCEL_ATTACK,
		SCHED_HEYE_RETREAT,
		SCHED_HEYE_RETREAT_RANDOM, // used as a fallback for retreat, incase we cant find a path away from the enemy
		SCHED_HEYE_COWER_CHECK_ENEMY_NEAR,
		SCHED_HEYE_COWER,

		// waypoint schedules
		SCHED_HEYE_MOVE_TO_WAYPOINT,	// move to the waypoint	
		SCHED_HEYE_WATCH_WAPOINT,		// watching the TV
		SCHED_HEYE_INSPECT_WAYPOINT,	// inspecting the body
		SCHED_HEYE_DECIDE_WAYPOINT_TASK
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
	float m_flCowerTime;
	
	// TODO: make this into some flagss
	bool m_bIsAtPoint;
	bool m_bIsPlotting;

	bool m_bCanCower;
	bool m_bIsCowering;

	CHoundeyePoint* m_pWaypoint;
};