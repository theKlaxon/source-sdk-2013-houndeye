#pragma once
#include "ai_basenpc.h"
#include "ai_behavior_follow.h"

class CNPC_Houndeye : public CAI_BehaviorHost<CAI_BaseNPC> {
	DECLARE_CLASS(CNPC_Houndeye, CAI_BehaviorHost<CAI_BaseNPC>)
	DECLARE_DATADESC()
	DEFINE_CUSTOM_AI
public:

	CNPC_Houndeye();

	void Precache();
	void Spawn();
	void Activate();
	
	void NPCThink();
	void GatherConditions();
	void RunTask(const Task_t* pTask);

	int SelectSchedule();
	int TranslateSchedule(int nType);

	bool CreateBehaviors();

	Class_T Classify() { return CLASS_HOUNDEYE; }
	
private:
	
	// conditions
	enum {
		COND_HEYE_HEALTH_LOW = LAST_SHARED_CONDITION,
		COND_HEYE_LEADER_DEAD,
		COND_HEYE_HAS_WAYPOINT,
	};

	// tasks
	enum {
		// doing anims
		TASK_HEYE_BEEP = LAST_SHARED_TASK,
		TASK_HEYE_SLEEP,
		TASK_HEYE_OPEN_EYE,
		TASK_HEYE_CLOSE_EYE,
		
		// attacking
		TASK_SURROUND,
		TASK_HEYE_DO_SHOCKWAVE,
	};

	// schedules
	enum {
		SCHED_HEYE_ATTACK = LAST_SHARED_SCHEDULE,
				
		SCHED_HEYE_WATCH_WAPOINT,		// watching the TV
		SCHED_HEYE_INSPECT_WAYPOINT,	// inspecting the body
	};

	// blinking
	enum {
		BLINK_OPENING,
		BLINK_CLOSING,
		BLINK_WAITING
	} m_nBlinkState, m_nPrevBlink;

	float m_flBlinkTime;

};