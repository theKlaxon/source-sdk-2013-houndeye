#include "cbase.h"
#include "houndeyetest/npc_houndeye.h" // specified path here since theres one in hl2 also
#include "hl2_shareddefs.h"
#include "npcevent.h"
#include "particle_parse.h"
#include "point_houndeye.h"

#define NPC_HEYE_MDL	"models/houndeye.mdl"
#define NPC_HEYE_HEALTH 10

static ConVar sv_houndeye_blinkdelay("sv_houndeye_blinkdelay", "2", 0, "Time between houndeye blinks");

LINK_ENTITY_TO_CLASS(npc_houndeye, CNPC_Houndeye);

BEGIN_DATADESC(CNPC_Houndeye)
END_DATADESC()

AI_BEGIN_CUSTOM_NPC(npc_houndeye, CNPC_Houndeye)

	// conditions
	DECLARE_CONDITION( COND_HEYE_HEALTH_LOW )
	DECLARE_CONDITION( COND_HEYE_LEADER_DEAD )
	DECLARE_CONDITION( COND_HEYE_HAS_WAYPOINT )
	DECLARE_CONDITION( COND_HEYE_EEPY )
	DECLARE_CONDITION( COND_HEYE_SPOOKED )
	DECLARE_CONDITION( COND_HEYE_ENEMY_TOO_FAR )
	
	// tasks
	DECLARE_TASK( TASK_HEYE_BEEP )
	DECLARE_TASK( TASK_HEYE_START_SNOOZE )
	DECLARE_TASK( TASK_HEYE_SPOOK_AWAKE )
	DECLARE_TASK( TASK_HEYE_OPEN_EYE )
	DECLARE_TASK( TASK_HEYE_CLOSE_EYE )
	DECLARE_TASK( TASK_HEYE_DO_SHOCKWAVE )

	DEFINE_SCHEDULE // the chillin' state
	(
		SCHED_HEYE_SNOOZE,

		"	Tasks"
		"	TASK_PLAY_SEQUENCE		ACTIVITY:ACT_CROUCHIDLE"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_START_SNOOZE,

		"	Tasks"
		"	TASK_STOP_MOVING		0"
		"	TASK_RESET_ACTIVITY		0"
		"	TASK_PLAY_SEQUENCE		ACTIVITY:ACT_CROUCH"
		"	TASK_SET_ACTIVITY		ACTIVITY:ACT_CROUCHIDLE"
		"	TASK_HEYE_START_SNOOZE	0"
		"	"
		"	Interrupts"
		"	COND_HEAR_COMBAT"
		"	COND_HEAR_PLAYER"
		"	COND_SEE_ENEMY"
		"	COND_SEE_PLAYER"

	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_ATTACK,

		"	Tasks"
		"	TASK_WAIT_FOR_MOVEMENT	0"
		"	TASK_STOP_MOVING		0"
		//"	TASK_FACE_ENEMY			0" // now taken care of in code
		
		"	TASK_HEYE_DO_SHOCKWAVE	0"
		"	TASK_SET_SCHEDULE		SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
		"	COND_HEYE_HEALTH_LOW"
		"	COND_LOST_ENEMY"
		"	COND_HEYE_ENEMY_TOO_FAR"
		"	COND_TOO_FAR_TO_ATTACK"
	)

	// schedules
	DEFINE_SCHEDULE
	(
		SCHED_HEYE_HUNT,

		"	Tasks"
		"	TASK_SET_FAIL_SCHEDULE					SCHEDULE:SCHED_HEYE_START_SNOOZE"
		"	TASK_SET_TOLERANCE_DISTANCE				175"
		"	TASK_SET_ROUTE_SEARCH_TIME				1"
		"	TASK_GET_FLANK_ARC_PATH_TO_ENEMY_LOS	30"		// works great! keep!
		//"	TASK_GET_FLANK_RADIUS_PATH_TO_ENEMY_LOS		0"	// mostly works
		//"	TASK_GET_PATH_TO_RANGE_ENEMY_LKP_LOS		0" // works with this!

		"	TASK_RUN_PATH							0"
		"	TASK_WAIT_FOR_MOVEMENT					0"
		"	TASK_STOP_MOVING						0"
		"	TASK_SET_SCHEDULE						SCHEDULE:SCHED_HEYE_ATTACK"
		"	"
		"	Interrupts"
		"	COND_TASK_FAILED"
		"	COND_HEYE_HEALTH_LOW"
		"	COND_LOST_ENEMY"
		"	COND_HEYE_ENEMY_TOO_FAR"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_SPOOKED_AWAKE,

		"	Tasks"
		"	TASK_HEYE_SPOOK_AWAKE	0"
		"	TASK_PLAY_SEQUENCE		ACTIVITY:ACT_HOP"

		"	TASK_SET_SCHEDULE		SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
	)	

AI_END_CUSTOM_NPC()

CNPC_Houndeye::CNPC_Houndeye() {
	
}

void CNPC_Houndeye::Precache() {
	BaseClass::Precache();

	PrecacheModel(NPC_HEYE_MDL);

	// npc_sounds_houndeye.txt
	PrecacheScriptSound("npc_houndeye.idle");
	PrecacheScriptSound("npc_houndeye.hunt");
	PrecacheScriptSound("npc_houndeye.alert");
	PrecacheScriptSound("npc_houndeye.die");
	PrecacheScriptSound("npc_houndeye.pain");
	PrecacheScriptSound("npc_houndeye.anger1");
	PrecacheScriptSound("npc_houndeye.anger2");
	PrecacheScriptSound("npc_houndeye.sonic");
	PrecacheScriptSound("npc_houndeye.warn");

	PrecacheParticleSystem("heye_aura_shockwave"); // re-purposed some ep2 particles for the shockwave
}

void CNPC_Houndeye::Spawn() {
	BaseClass::Spawn();
	Precache();

	SetModel(NPC_HEYE_MDL);
	SetBloodColor(BLOOD_COLOR_GREEN);
	
	SetHullType(HULL_WIDE_SHORT);
	SetHullSizeNormal();

	SetNavType(NAV_GROUND);

	SetSolid(SOLID_BBOX);
	AddSolidFlags(FSOLID_NOT_STANDABLE);
	SetMoveType(MOVETYPE_STEP);
	
	SetCollisionGroup(HL2COLLISION_GROUP_HOUNDEYE);
	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_TURN_HEAD | bits_CAP_INNATE_RANGE_ATTACK1);
	
	SetHealth(NPC_HEYE_HEALTH);

	m_flFieldOfView = 0.5f;
	m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat();
	m_nBlinkState = BLINK_WAITING;
	m_NPCState = NPC_STATE_NONE;
	m_takedamage = 1;

	AddClassRelationship(CLASS_PLAYER, Disposition_t::D_HT, 0);

	NPCInit();
}

void CNPC_Houndeye::Activate() {
	
	BaseClass::Activate();
}

void CNPC_Houndeye::NPCThink() {

	BaseClass::NPCThink();

	// TODO: make this code more elegant.
	switch (m_nBlinkState) {
	case BLINK_OPENING:

		if (m_nSkin > 0)
			m_nSkin--;

		if (gpGlobals->curtime >= m_flBlinkTime && m_nSkin == 0) {
			m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat();
			m_nPrevBlink = BLINK_OPENING;
			m_nBlinkState = BLINK_WAITING;
		}
		break;
	case BLINK_CLOSING:
		
		if (m_nSkin < 3)
			m_nSkin++;
		
		if (gpGlobals->curtime >= m_flBlinkTime && m_nSkin == 3) {
			
			m_nPrevBlink = BLINK_CLOSING;
			m_nBlinkState = BLINK_WAITING;
			m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat() / 2;
		}
		break;
	case BLINK_WAITING:

		if (gpGlobals->curtime >= m_flBlinkTime) {
			if (m_nPrevBlink == BLINK_CLOSING)
				m_nBlinkState = BLINK_OPENING;
			else
				m_nBlinkState = BLINK_CLOSING;

			m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat();
		}
		break;
	}
}

void CNPC_Houndeye::GatherConditions() {
	BaseClass::GatherConditions();

	// clear out old conditions
	static int nToClear[] = {
		COND_HEYE_HEALTH_LOW,
		COND_HEYE_EEPY,
		COND_HEYE_LEADER_DEAD,
		COND_HEYE_HAS_WAYPOINT,
		COND_HEYE_ENEMY_TOO_FAR,
	};
	ClearConditions(nToClear, ARRAYSIZE(nToClear));
	
	// check for enemies / danger nearby
	if (m_bIsSleeping == false) {

		if (HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT) || HasCondition(COND_SEE_ENEMY))
			SetCondition(COND_HEYE_SPOOKED);

		if (m_NPCState != NPC_STATE_COMBAT) {
			if (!HasCondition(COND_SEE_ENEMY)/* && !HasCondition(COND_SEE_PLAYER)*/)
				SetCondition(COND_HEYE_EEPY);
		}
		else {
		
			if (GetEnemy()) {
				if (EnemyDistance(GetEnemy()) > 500.0f)
					SetCondition(COND_HEYE_ENEMY_TOO_FAR);
			}

		}
	}
	else {

		if ((HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT)))
			SetCondition(COND_HEYE_SPOOKED);

	}
	
	// check if we have critical damage (< 1/3rd max health)
	if (m_iHealth < NPC_HEYE_HEALTH * 0.33f) {
		SetCondition(COND_HEYE_HEALTH_LOW);
		m_NPCState = NPC_STATE_NONE;
	}
		
}

void CNPC_Houndeye::StartTask(const Task_t* pTask) {

	switch (pTask->iTask) {
	case TASK_HEYE_START_SNOOZE:
		break;
	case TASK_HEYE_SPOOK_AWAKE:
		m_NPCState = NPC_STATE_COMBAT;
		break;
	case TASK_HEYE_DO_SHOCKWAVE:
		//DevMsg("CNPC_Houndeye::RunTask : TASK_HEYE_DO_SHOCKWAVE STARTED\n");
		SetIdealActivity(ACT_RANGE_ATTACK1);
		break;
	default:
		BaseClass::StartTask(pTask);
		break;
	}
}

void CNPC_Houndeye::RunTask(const Task_t* pTask) {

	switch (pTask->iTask) {
	case TASK_HEYE_START_SNOOZE:

		m_bIsSleeping = true;
		m_NPCState = NPC_STATE_NONE;
		TaskComplete();
		break;
	case TASK_HEYE_SPOOK_AWAKE:

		m_bIsSleeping = false;
		m_NPCState = NPC_STATE_COMBAT;
		WakeSound();

		if(IsSequenceFinished())
			TaskComplete();

		break;
	case TASK_HEYE_DO_SHOCKWAVE:
	{
		// always try to face enemy - curtosey of basenpc_schedule
		{
			// If the yaw is locked, this function will not act correctly
			Assert(GetMotor()->IsYawLocked() == false);

			Vector vecEnemyLKP = GetEnemyLKP();
			if (!FInAimCone(vecEnemyLKP))
			{
				GetMotor()->SetIdealYawToTarget(vecEnemyLKP);
				GetMotor()->SetIdealYaw(CalcReasonableFacing(true)); // CalcReasonableFacing() is based on previously set ideal yaw
			}
			else
			{
				float flReasonableFacing = CalcReasonableFacing(true);
				if (fabsf(flReasonableFacing - GetMotor()->GetIdealYaw()) > 1)
					GetMotor()->SetIdealYaw(flReasonableFacing);
			}

			GetMotor()->UpdateYaw();
		}

		if (IsSequenceFinished()) {
			DoShockwave();
			TaskComplete();

			//DevMsg("CNPC_Houndeye::RunTask : TASK_HEYE_DO_SHOCKWAVE FINISHED\n");
		}
	}
		break;
	default:
		BaseClass::RunTask(pTask);
		break;
	}
}

void CNPC_Houndeye::HandleAnimEvent(animevent_t* pEvent) {

	switch (pEvent->event) {
	case 1:
		EmitSound("npc_houndeye.warn");
	case AE_NPC_ITEM_PICKUP:
		break;
	case AE_NPC_BODYDROP_HEAVY:
		EmitSound("npc_houndeye.anger1");
	case AE_NPC_SWISHSOUND:
		EmitSound("npc_houndeye.anger2");
		return;
	case AE_NPC_BODYDROP_LIGHT:
		return;
	case AE_NPC_RIGHTFOOT: // '2' is the warmup sound turns out
		EmitSound("npc_houndeye.warn");
		return;
	}

	BaseClass::HandleAnimEvent(pEvent);
}

void CNPC_Houndeye::WakeSound() {
	EmitSound("npc_houndeye.alert");
}

void CNPC_Houndeye::FoundEnemySound() {
	WakeSound();
}

void CNPC_Houndeye::DoShockwave() {

	// do our fx
	DispatchParticleEffect("heye_aura_shockwave", GetAbsOrigin(), QAngle());
	EmitSound("npc_houndeye.sonic");

	// hurt things
	//	TODO: find ents in the attack radius,
	//		: if they're'nt houndeyes, apply damage,
	//		: ensure that the pack number influences damage (just like hl1)
}

int CNPC_Houndeye::RangeAttack1Conditions(float flDot, float flDist) {

	// TODO: tweak this!
	if (flDist > 200.0f)
		return COND_TOO_FAR_TO_ATTACK;

	return COND_CAN_RANGE_ATTACK1;
}

int CNPC_Houndeye::SelectSchedule() {

	// if we have low health and no leader, just run lol
	if (HasCondition(COND_HEYE_HEALTH_LOW) && 
		HasCondition(COND_HEYE_LEADER_DEAD) &&
		HasCondition(COND_SEE_ENEMY))
		return SCHED_TAKE_COVER_FROM_ENEMY;
	
	if (m_NPCState != NPC_STATE_COMBAT) {

		// if we have a job, skip the sleep
		if (HasCondition(COND_HEYE_HAS_WAYPOINT))
			return SCHED_HEYE_WATCH_WAPOINT;
		
		// handle sleeping ONLY if we dont have a job
		if (m_bIsSleeping == true) {

			if (HasCondition(COND_HEYE_SPOOKED))
				return SCHED_HEYE_SPOOKED_AWAKE;
			else
				return SCHED_HEYE_SNOOZE; // keeb sleebin'

		}
		else if (HasCondition(COND_HEYE_EEPY)) // we can sleep to deactivate our combat state when we no longer see a player or danger
			return SCHED_HEYE_START_SNOOZE;

	}

	// always start hunting for what spooked us
	if (HasCondition(COND_HEYE_SPOOKED)) {

		m_bIsSleeping = false;
		ClearCondition(COND_HEYE_SPOOKED);
		return SCHED_HEYE_HUNT;
	}
	
	return BaseClass::SelectSchedule();
}

int CNPC_Houndeye::SelectFailSchedule(int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode) {

	return BaseClass::SelectFailSchedule(failedSchedule, failedTask, taskFailCode);
}

int CNPC_Houndeye::TranslateSchedule(int nType) {

	switch (nType) {
	case SCHED_IDLE_STAND:
	case SCHED_IDLE_WALK:
	case SCHED_IDLE_WANDER:

		if (m_bIsSleeping == false)
			return SCHED_HEYE_START_SNOOZE;
		else
			return SCHED_HEYE_SNOOZE;

		break;
	}	

	return BaseClass::TranslateSchedule(nType);
}