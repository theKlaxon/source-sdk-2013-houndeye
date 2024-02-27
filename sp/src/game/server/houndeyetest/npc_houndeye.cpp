#include "cbase.h"
#include "houndeyetest/npc_houndeye.h" // specified path here since theres one in hl2 also
#include "hl2_shareddefs.h"

#define NPC_HEYE_		"npc_houndeye"
#define NPC_HEYE_MDL	"models/houndeye.mdl"
#define NPC_HEYE_HEALTH 10

ConVar sv_houndeye_blinkdelay("sv_houndeye_blinkdelay", "2", 0, "Time between houndeye blinks");

LINK_ENTITY_TO_CLASS(npc_houndeye, CNPC_Houndeye);

BEGIN_DATADESC(CNPC_Houndeye)
END_DATADESC()

AI_BEGIN_CUSTOM_NPC(npc_houndeye, CNPC_Houndeye)

	// conditions
	DECLARE_CONDITION( COND_HEYE_HEALTH_LOW )
	DECLARE_CONDITION( COND_HEYE_LEADER_DEAD )
	DECLARE_CONDITION( COND_HEYE_HAS_WAYPOINT )
	
	// tasks
	DECLARE_TASK( TASK_HEYE_BEEP )
	DECLARE_TASK( TASK_HEYE_SLEEP )
	DECLARE_TASK( TASK_HEYE_OPEN_EYE )
	DECLARE_TASK( TASK_HEYE_CLOSE_EYE )
	DECLARE_TASK( TASK_HEYE_DO_SHOCKWAVE )

	// schedules
	DEFINE_SCHEDULE
	(
		SCHED_HEYE_ATTACK,

		"	Tasks"
		"	TASK_FACE_PLAYER		0"
		"	TASK_GET_PATH_TO_PLAYER	0"
		"	TASK_RUN_PATH			0"
		"	"
		"	Interrupts"
		"	COND_TASK_FAILED"
		"	COND_HEYE_HEALTH_LOW"
	)

AI_END_CUSTOM_NPC()

CNPC_Houndeye::CNPC_Houndeye() {

}

void CNPC_Houndeye::Precache() {
	BaseClass::Precache();

	PrecacheModel(NPC_HEYE_MDL);

	// npc_sounds_houndeye.txt
	PrecacheScriptSound(NPC_HEYE_ ".idle");
	PrecacheScriptSound(NPC_HEYE_ ".hunt");
	PrecacheScriptSound(NPC_HEYE_ ".alert");
	PrecacheScriptSound(NPC_HEYE_ ".die");
	PrecacheScriptSound(NPC_HEYE_ ".pain");
	PrecacheScriptSound(NPC_HEYE_ ".anger1");
	PrecacheScriptSound(NPC_HEYE_ ".anger2");
	PrecacheScriptSound(NPC_HEYE_ ".sonic");
	PrecacheScriptSound(NPC_HEYE_ ".warn");
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

	NPCInit();

	SetState(NPC_STATE_ALERT);
	SetSchedule(SCHED_IDLE_WANDER);
}

void CNPC_Houndeye::Activate() {
	
	BaseClass::Activate();
}

void CNPC_Houndeye::NPCThink() {

	BaseClass::NPCThink();

	// blink 
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
			m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat() / 2;
			m_nPrevBlink = BLINK_CLOSING;
			m_nBlinkState = BLINK_WAITING;
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
		COND_HEYE_LEADER_DEAD,
		COND_HEYE_HAS_WAYPOINT,
	};
	ClearConditions(nToClear, ARRAYSIZE(nToClear));

	// get new ones

	// check if the pack leader is dead
	
	// check if we have critical damage (< 1/3rd max health)
	if (m_iHealth < NPC_HEYE_HEALTH * 0.33f)
		SetCondition(COND_HEYE_HEALTH_LOW);
}

void CNPC_Houndeye::RunTask(const Task_t* pTask) {

	BaseClass::RunTask(pTask);
}

bool CNPC_Houndeye::CreateBehaviors() {
	
	return BaseClass::CreateBehaviors();
}

int CNPC_Houndeye::SelectSchedule() {

	// if we have low health and no leader, just run lol
	if (HasCondition(COND_HEYE_HEALTH_LOW) && 
		HasCondition(COND_HEYE_LEADER_DEAD) &&
		(HasCondition(COND_SEE_PLAYER) || HasCondition(COND_SEE_ENEMY)))
		return SCHED_RUN_FROM_ENEMY;

	if (HasCondition(COND_SEE_PLAYER))
		return SCHED_HEYE_ATTACK;

	// this must be done last, as some other conditions need to 
	// have precedent over this one.
	if (HasCondition(COND_HEYE_HAS_WAYPOINT))
		return SCHED_HEYE_WATCH_WAPOINT;

	return SCHED_IDLE_WANDER;
}

int CNPC_Houndeye::TranslateSchedule(int nType) {

	switch (nType) {
	case SCHED_IDLE_STAND:
	case SCHED_IDLE_WALK:
		return SCHED_IDLE_WANDER;
	}

	return BaseClass::TranslateSchedule(nType);
}