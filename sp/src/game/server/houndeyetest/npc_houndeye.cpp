#include "cbase.h"
#include "houndeyetest/npc_houndeye.h" // specified path here since theres one in hl2 also
#include "hl2_shareddefs.h"
#include "npcevent.h"
#include "particle_parse.h"
#include "point_houndeye.h"
#include "point_houndeye_manager.h"
#include "ai_squad.h"

#define NPC_HEYE_MDL	"models/houndeye.mdl"
#define NPC_HEYE_HEALTH 80

ConVar sk_houndeye_health("sk_houndeye_health", "0");
static ConVar sv_houndeye_squadmax("sv_houndeye_squadmax", "5", FCVAR_CHEAT, "Max number of houneyes in a squad");
static ConVar sv_houndeye_blinkdelay("sv_houndeye_blinkdelay", "2", 0, "Time between houndeye blinks");
static ConVar sv_houndeye_squadattackdmg("sv_houndeye_squadattackdmg", "6.0f", 0, "The multiplier of houndeye attack damage per each pack member.");
static ConVar sv_houndeye_squadattackbonus("sv_houndeye_squadattackbonus", "1.2f", 0, "The multiplier of houndeye attack damage per each pack member.");
static ConVar sv_houndeye_huntrange("sv_houndeye_huntrange", "800.0f", FCVAR_CHEAT, "The range in which the houndeye will hunt down it's enemy.");

// custom acts
int ACT_INSPECT_WALL;
int ACT_EAT;

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
	DECLARE_CONDITION( COND_HEYE_SQUAD_ALERT )
	
	// tasks
	DECLARE_TASK( TASK_HEYE_BEEP )
	DECLARE_TASK( TASK_HEYE_START_SNOOZE )
	DECLARE_TASK( TASK_HEYE_SPOOK_AWAKE )
	DECLARE_TASK( TASK_HEYE_PLOT_ATTACK )
	DECLARE_TASK( TASK_HEYE_DO_SHOCKWAVE )
	DECLARE_TASK( TASK_HEYE_CHECK_FOR_SQUAD )
	DECLARE_TASK( TASK_HEYE_ANIM_WATCH )
	DECLARE_TASK( TASK_HEYE_ANIM_INSPECT )
	DECLARE_TASK( TASK_HEYE_ANIM_COWER )
	DECLARE_TASK( TASK_HEYE_SET_AT_POINT )

	// acts
	DECLARE_ACTIVITY( ACT_INSPECT_WALL )
	DECLARE_ACTIVITY( ACT_EAT )
	
	DEFINE_SCHEDULE // the chillin' state
	(
		SCHED_HEYE_SNOOZE,

		"	Tasks"
		"	TASK_SET_FAIL_SCHEDULE	SCHEDULE:SCHED_HEYE_SPOOKED_AWAKE"
		"	"
		"	Interrupts"
		"	COND_HEAR_COMBAT"
		"	COND_HEAR_PLAYER"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_START_SNOOZE,

		"	Tasks"
		"	TASK_SET_FAIL_SCHEDULE	SCHEDULE:SCHED_HEYE_SPOOKED_AWAKE"
		"	TASK_STOP_MOVING		0"
		
		"	TASK_RESET_ACTIVITY		0"
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
		"	TASK_SET_FAIL_SCHEDULE	SCHEDULE:SCHED_HEYE_CANCEL_ATTACK"
		"	TASK_HEYE_DO_SHOCKWAVE	0"
		"	TASK_SET_SCHEDULE		SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
		"	COND_HEYE_HEALTH_LOW"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_CANCEL_ATTACK,

		"	Tasks"
		"	TASK_HEYE_BEEP		0"
		"	TASK_SET_SCHEDULE	SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_RETREAT,

		"	Tasks"
		"	TASK_FIND_BACKAWAY_FROM_SAVEPOSITION	1024" // give a large range of distances to choose from for retreat
		//"	TASK_GET_PATH_TO_RANDOM_NODE	4096" // give a large range of distances to choose from for retreat
		//"	TASK_FIND_FAR_NODE_COVER_FROM_ENEMY	4096" // give a large range of distances to choose from for retreat
		"	TASK_RUN_PATH					0"
		"	TASK_WAIT_FOR_MOVEMENT			0"
		"	TASK_STOP_MOVING				0"
		"	TASK_SET_SCHEDULE				SCHEDULE:SCHED_HEYE_COWER"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_COWER,

		"	Tasks"
		"	TASK_HEYE_ANIM_COWER	0"
		"	TASK_SET_SCHEDULE		SCHEDULE:SCHED_HEYE_COWER"
		"	"
		"	Interrupts"

	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_SPOOKED_AWAKE,

		"	Tasks"
		"	TASK_STORE_POSITION_IN_SAVEPOSITION 0"
		"	TASK_HEYE_SPOOK_AWAKE				0"
		"	TASK_SET_SCHEDULE					SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_PLOTTING,

		"	Tasks"
		"	TASK_HEYE_CHECK_FOR_SQUAD	0" // check if we're in a squad, if not then just attack.
		"	TASK_SET_FAIL_SCHEDULE		SCHEDULE:SCHED_HEYE_HUNT"
		"	TASK_WAIT_FOR_MOVEMENT		0"
		"	TASK_STOP_MOVING			0"
		"	TASK_RESET_ACTIVITY			0"
		"	TASK_HEYE_PLOT_ATTACK		0"
		"	TASK_SET_SCHEDULE			SCHEDULE:SCHED_HEYE_HUNT"
		"	"
		"	Interrupts"
		"	COND_TASK_FAILED"
		"	COND_TOO_FAR_TO_ATTACK"
		"	COND_LOST_ENEMY"
		//"	COND_NO_HEAR_DANGER"
		"	COND_ENEMY_WENT_NULL"
		"	COND_HEYE_ENEMY_TOO_FAR"
	)
	
	// schedules
	DEFINE_SCHEDULE
	(
		SCHED_HEYE_HUNT,

		"	Tasks"
		"	TASK_STORE_POSITION_IN_SAVEPOSITION		0"
		//"	TASK_SET_FAIL_SCHEDULE					SCHEDULE:SCHED_IDLE_WANDER"
		"	TASK_SET_TOLERANCE_DISTANCE				200"
		"	TASK_GET_FLANK_ARC_PATH_TO_ENEMY_LOS	30"		// works great! keep!
		//"	TASK_GET_FLANK_RADIUS_PATH_TO_ENEMY_LOS		0"	// mostly works
		//"	TASK_GET_PATH_TO_RANGE_ENEMY_LKP_LOS		0" // works with this!

		"	TASK_RUN_PATH							0"
		"	TASK_WAIT_FOR_MOVEMENT					0"
		"	TASK_STOP_MOVING						0"
		"	TASK_RESET_ACTIVITY						0"
		"	TASK_SET_SCHEDULE						SCHEDULE:SCHED_HEYE_PLOTTING"
		"	"
		"	Interrupts"
		"	COND_TASK_FAILED"
		"	COND_HEYE_HEALTH_LOW"
		"	COND_LOST_ENEMY"
		"	COND_HEYE_ENEMY_TOO_FAR"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_MOVE_TO_WAYPOINT,

		"	Tasks"
		"	TASK_GET_PATH_TO_TARGET		0"
		"	TASK_RUN_PATH				0"
		"	TASK_WAIT_FOR_MOVEMENT		0"
		"	TASK_GET_PATH_TO_RANDOM_NODE	48"
		"	TASK_RUN_PATH					0"
		"	TASK_WAIT_FOR_MOVEMENT			0"
		"	TASK_STOP_MOVING			0"
		"	TASK_HEYE_SET_AT_POINT		0"	// this task intentionally fails to 
											// make SelectFailSchedule do the work 
											// of finding the heye_point's desired task schedule
		"	"
		"	Interrupts"
		"	COND_TASK_FAILED"
		"	COND_HEYE_SQUAD_ALERT"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_WATCH_WAPOINT,

		"	Tasks"
		"	TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_HEYE_HUNT"
		"	TASK_FACE_TARGET				0"
		
		"	TASK_RESET_ACTIVITY				0"
		"	TASK_HEYE_ANIM_WATCH			0"
		"	TASK_SET_SCHEDULE				SCHEDULE:SCHED_HEYE_WATCH_WAPOINT"
		"	"
		"	Interrupts"
		"	COND_HEYE_SQUAD_ALERT"
	)

	DEFINE_SCHEDULE
	(
		SCHED_HEYE_INSPECT_WAYPOINT,

		"	Tasks"
		"	TASK_SET_FAIL_SCHEDULE			SCHEDULE:SCHED_HEYE_HUNT"
		"	TASK_GET_PATH_TO_RANDOM_NODE	4"
		"	TASK_WALK_PATH					0"
		"	TASK_WAIT_FOR_MOVEMENT			0"
		"	TASK_FACE_TARGET				0"
		"	TASK_STOP_MOVING				0"
		"	TASK_RESET_ACTIVITY				0"
		"	TASK_HEYE_ANIM_INSPECT			0"
		"	TASK_SET_SCHEDULE				SCHEDULE:SCHED_HEYE_INSPECT_WAYPOINT"
		"	"
		"	Interrupts"
		"	COND_HEYE_SQUAD_ALERT"
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

	PrecacheParticleSystem("heye_aura_shockwave0"); // re-purposed some ep2 particles for the shockwave
	PrecacheParticleSystem("heye_aura_shockwave1");
	PrecacheParticleSystem("heye_aura_shockwave2");
	PrecacheParticleSystem("heye_aura_shockwave3");
	PrecacheParticleSystem("advisor_object_charge_glow"); // for attack charge up
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
	
	ClearEffects();

	SetCollisionGroup(HL2COLLISION_GROUP_CROW);
	//SetCollisionGroup(HL2COLLISION_GROUP_HOUNDEYE);
	CapabilitiesClear();
	CapabilitiesAdd(bits_CAP_MOVE_GROUND | bits_CAP_TURN_HEAD | bits_CAP_INNATE_RANGE_ATTACK1 | bits_CAP_SQUAD);
	CapabilitiesAdd(bits_CAP_MOVE_JUMP);
	
	m_flFieldOfView = 0.5f;
	m_flBlinkTime = gpGlobals->curtime + sv_houndeye_blinkdelay.GetFloat();
	m_nBlinkState = BLINK_WAITING;
	m_nSleepState = SLEEP_NOT_SLEEPING;
	m_NPCState = NPC_STATE_NONE;
	m_iHealth = sk_houndeye_health.GetFloat();

	m_pWaypoint = nullptr;
	m_bSitting = false;
	m_bPlotting = false;
	m_bAtPoint = false;
	m_bCanAttack = false;
	m_bIsCowering = false;

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
		COND_HEYE_SQUAD_ALERT,
	};
	ClearConditions(nToClear, ARRAYSIZE(nToClear));

	// check if we have critical damage (< 1/3rd max health)
	if (m_iHealth < NPC_HEYE_HEALTH * 0.33f) {
		SetCondition(COND_HEYE_HEALTH_LOW);
		m_NPCState = NPC_STATE_ALERT;
	}

	// stuff from the squad / leader takes priority (sorta)
	if (m_pSquad) {

		if (m_pSquad->IsLeader(this))
		{
			if (HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT) || HasCondition(COND_SEE_ENEMY) || HasCondition(COND_HEAR_PLAYER) || HasCondition(COND_HEYE_HEALTH_LOW))
				SetCondition(COND_HEYE_SQUAD_ALERT);

		}
		else if (m_pSquad->GetLeader()->HasCondition(COND_HEYE_SQUAD_ALERT)) {
			SetCondition(COND_HEYE_SQUAD_ALERT);
		}

	}
	else {

		if (HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_COMBAT) || HasCondition(COND_SEE_ENEMY) && m_nSleepState != SLEEP_NOT_SLEEPING)
			SetCondition(COND_HEYE_SPOOKED);

	}
	
	// check enemy distance
	if (GetEnemy()) {
		if (EnemyDistance(GetEnemy()) > 400.0f)
			SetCondition(COND_HEYE_ENEMY_TOO_FAR);
		else
			ClearCondition(COND_HEYE_ENEMY_TOO_FAR);
	}
	else
		ClearCondition(COND_HEYE_ENEMY_TOO_FAR);

}

void CNPC_Houndeye::StartTask(const Task_t* pTask) {

	switch (pTask->iTask) {
	case TASK_HEYE_BEEP:
		break;

	case TASK_HEYE_START_SNOOZE:
		m_nSleepState = SLEEP_STARTED_SLEEPING;
		m_NPCState = NPC_STATE_IDLE;
		m_bPlotting = false;
		SetIdealActivity(ACT_CROUCH);
		break;

	case TASK_HEYE_SPOOK_AWAKE:
		WakeSound();
		SetIdealActivity(ACT_HOP);
		m_nSleepState = SLEEP_NOT_SLEEPING;
		m_bIsCowering = false;
		break;

	case TASK_HEYE_PLOT_ATTACK:
		m_bPlotting = true;
		m_bIsCowering = false;
		SetIdealActivity(ACT_IDLE_ANGRY);
		break;

	case TASK_HEYE_DO_SHOCKWAVE:
		DevMsg("CNPC_Houndeye::RunTask : TASK_HEYE_DO_SHOCKWAVE STARTED\n");

		m_bPlotting = false;
		SetIdealActivity(ACT_RANGE_ATTACK1);

		// do some attack charging particles
		DispatchParticleEffect("advisor_object_charge_glow", WorldSpaceCenter(), GetAbsAngles());
		break;
	
	case TASK_HEYE_CHECK_FOR_SQUAD:
		break;

	case TASK_HEYE_ANIM_WATCH:
		m_bAtPoint = false;
		SetIdealActivity((Activity)ACT_INSPECT_WALL);
		break;
	case TASK_HEYE_ANIM_INSPECT:
		m_bAtPoint = false;
		SetIdealActivity((Activity)ACT_EAT);
		break;

	case TASK_HEYE_SET_AT_POINT:
		break;

	case TASK_HEYE_ANIM_COWER:
		m_bCanAttack = false;
		m_bPlotting = false;
		m_bAtPoint = false;
		m_bIsCowering = true;
		m_nSleepState = SLEEP_NOT_SLEEPING;
		SetIdealActivity(ACT_COWER);
		break;

	default:
		BaseClass::StartTask(pTask);
		break;
	}
}

void CNPC_Houndeye::RunTask(const Task_t* pTask) {

	switch (pTask->iTask) {
	case TASK_HEYE_BEEP:
		EmitSound("npc_houndeye.hunt");
		TaskComplete();
		break;

	case TASK_HEYE_START_SNOOZE:

		if (IsSequenceFinished()) {
			m_nSleepState = SLEEP_IS_SLEEPING;
			SetIdealActivity(ACT_CROUCHIDLE);
			TaskComplete();
		}
		break;

	case TASK_HEYE_SPOOK_AWAKE:

		if (IsSequenceFinished()) {
			
			m_nSleepState = SLEEP_NOT_SLEEPING;
			TaskComplete();
		}
		break;

	case TASK_HEYE_PLOT_ATTACK:

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

		if(IsSequenceFinished())
			TaskComplete();

		break;
		
	case TASK_HEYE_DO_SHOCKWAVE:
		
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

		if (HasCondition(COND_HEYE_ENEMY_TOO_FAR)) {
			VacateStrategySlot();
			TaskFail(FAIL_NO_ENEMY);
			break;
		}

		if (IsSequenceFinished()) {
			DoShockwave();
			VacateStrategySlot();
			TaskComplete();
			DevMsg("CNPC_Houndeye::RunTask : TASK_HEYE_DO_SHOCKWAVE FINISHED\n");
		}
		break;

	case TASK_HEYE_CHECK_FOR_SQUAD:
		
		if (m_pSquad != nullptr)
			TaskComplete();
		else
			TaskFail(FAIL_HEYE_NO_SQUAD);
		break;

	case TASK_HEYE_ANIM_WATCH:
	case TASK_HEYE_ANIM_INSPECT:

		if (IsSequenceFinished())
			TaskComplete();

		break;

	case TASK_HEYE_SET_AT_POINT:

		m_bAtPoint = true;
		TaskComplete();

		break;

	case TASK_HEYE_ANIM_COWER:
		
		if (IsSequenceFinished())
			TaskComplete();

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
		return;

	case AE_NPC_ITEM_PICKUP:
		return;

	case AE_NPC_BODYDROP_HEAVY:
		EmitSound("npc_houndeye.anger1");
		return;

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
	if (m_pSquad) {
		switch (m_pSquad->NumMembers()) {
		case 1:
			DispatchParticleEffect("heye_aura_shockwave0", GetAbsOrigin(), QAngle());
			break;
		case 2:
			DispatchParticleEffect("heye_aura_shockwave1", GetAbsOrigin(), QAngle());
			break;
		case 3:
			DispatchParticleEffect("heye_aura_shockwave2", GetAbsOrigin(), QAngle());
			break;
		case 4:
		case 5:
		default:
			DispatchParticleEffect("heye_aura_shockwave3", GetAbsOrigin(), QAngle());
			break;
		}
	}
	else
		DispatchParticleEffect("heye_aura_shockwave0", GetAbsOrigin(), QAngle());

	EmitSound("npc_houndeye.sonic");

	// hurt things
	//	TODO: find ents in the attack radius,
	//		: if they're'nt houndeyes, apply damage,
	//		: ensure that the pack number influences damage (just like hl1)

	CBaseEntity* pEnts[20];
	int cnt = UTIL_EntitiesInSphere(pEnts, 20, WorldSpaceCenter(), 200.0f, 0);

	float baseDmg = sv_houndeye_squadattackdmg.GetFloat();
	float packDmg = baseDmg;
	packDmg += baseDmg + (m_pSquad != nullptr ? m_pSquad->NumMembers() : 1) * sv_houndeye_squadattackbonus.GetFloat();

	for (int i = 0; i < cnt; i++) {

		if (pEnts[i] == nullptr)
			continue;

		if (!V_strcmp(pEnts[i]->GetClassname(), "npc_houndeye") || pEnts[i]->m_takedamage == DAMAGE_NO)
			continue;

		CTakeDamageInfo dmg(this, this, packDmg, DMG_SONIC);
		pEnts[i]->TakeDamage(dmg);
	}
}

int CNPC_Houndeye::RangeAttack1Conditions(float flDot, float flDist) {

	// TODO: tweak this!
	if (flDist < 200.0f)
		return COND_TOO_FAR_TO_ATTACK;

	return COND_CAN_RANGE_ATTACK1;
}

int CNPC_Houndeye::SelectSchedule() {

	// TODO: create fail conditions to make the cowering houndeye keep running from the player

	if (HasCondition(COND_HEYE_HEALTH_LOW) && 
		(HasCondition(COND_SEE_ENEMY) || HasCondition(COND_HEAR_COMBAT) || HasCondition(COND_HEAR_DANGER) || HasCondition(COND_HEAR_PLAYER) ))
		return SCHED_HEYE_RETREAT;

	// only cower if we dont see the enemy, and we have low health
	if (m_bIsCowering)
		return SCHED_HEYE_COWER;
	
	if (m_bPlotting) {

		// if we're plotting our attack, and we're too far away, move closer
		//if (HasCondition(COND_HEYE_ENEMY_TOO_FAR)) {
		//	VacateStrategySlot();
		//	Msg("CNPCHoundeye: Enemy too far! hunting again!\n");
		//	return SCHED_HEYE_HUNT;
		//}

		if (m_pSquad) {
			if (OccupyStrategySlotRange(SQUAD_SLOT_ATTACK1, SQUAD_SLOT_ATTACK2) || !m_pSquad->GetLeader()->IsAlive()) // to help with heyes acting erratic when leader dead
				return SCHED_HEYE_ATTACK;
			else
				return SCHED_HEYE_PLOTTING;
		}
		else
			return SCHED_HEYE_ATTACK; // if we're not part of a squad, we should just attack.
	}

	if (m_bAtPoint) {

		if (m_pSquad) {

			if (m_pSquad->IsLeader(this)) {
				if (!m_pWaypoint)
					return SCHED_IDLE_WANDER;

				switch (m_pWaypoint->GetBehavior()) {
				case HEYE_POINT_WATCH_TV:
					return SCHED_HEYE_WATCH_WAPOINT;

				case HEYE_POINT_INSPECT_DEADGUY:
					return SCHED_HEYE_INSPECT_WAYPOINT;
				}
			}
			else {

				CHoundeyePoint* pPoint = static_cast<CHoundeyePoint*>(m_pSquad->GetLeader()->GetTarget());
				if (pPoint) {
					m_pWaypoint = pPoint;

					switch (pPoint->GetBehavior()) {
					case HEYE_POINT_WATCH_TV:
						return SCHED_HEYE_WATCH_WAPOINT;

					case HEYE_POINT_INSPECT_DEADGUY:
						return SCHED_HEYE_INSPECT_WAYPOINT;
					}
				}
			}

		}
		else {

			if (!m_pWaypoint)
				return SCHED_IDLE_WANDER;

			switch (m_pWaypoint->GetBehavior()) {
			case HEYE_POINT_WATCH_TV:
				return SCHED_HEYE_WATCH_WAPOINT;

			case HEYE_POINT_INSPECT_DEADGUY:
				return SCHED_HEYE_INSPECT_WAYPOINT;
			}
		}

	}
	
	if (HasCondition(COND_HEYE_ENEMY_TOO_FAR))
		return SCHED_IDLE_WANDER;

	if (HasCondition(COND_HEYE_SPOOKED) && m_nSleepState != SLEEP_NOT_SLEEPING)
		return SCHED_HEYE_SPOOKED_AWAKE;
	
	return BaseClass::SelectSchedule();
}

int CNPC_Houndeye::SelectFailSchedule(int failedSchedule, int failedTask, AI_TaskFailureCode_t taskFailCode) {
	
	switch (failedTask) {
	case TASK_HEYE_PLOT_ATTACK:		
		break;

	case TASK_HEYE_CHECK_FOR_SQUAD:
		return SCHED_HEYE_ATTACK;

	case TASK_GET_FLANK_ARC_PATH_TO_ENEMY_LOS:
		return SCHED_HEYE_PLOTTING;
	//case TASK_HEYE_DO_SHOCKWAVE:
	//	return SCHED_HEYE_HUNT;
	}
	
	return SCHED_IDLE_WANDER;
}

int CNPC_Houndeye::TranslateSchedule(int nType) {

	switch (nType) {
	case SCHED_IDLE_STAND:
	case SCHED_IDLE_WALK:
	case SCHED_IDLE_WANDER:

		// move to the waypoint if we have one
		if (m_pSquad) {
			
			if (m_pSquad->IsLeader(this)) {

				if (HasCondition(COND_SEE_ENEMY) || HasCondition(COND_HEAR_COMBAT) || HasCondition(COND_HEAR_DANGER) || HasCondition(COND_SEE_PLAYER)) {
					m_bAtPoint = false;
					return SCHED_HEYE_HUNT;
				}
				else {
					CHoundeyePoint* pPoint = CHoundeyePointManager::GetInstance().GetPointClosestToPos(WorldSpaceCenter());

					if (pPoint != nullptr) {
						m_pWaypoint = pPoint;
						SetTarget(m_pWaypoint);
						return SCHED_HEYE_MOVE_TO_WAYPOINT;
					}
				}
			}
			else {

				CAI_BaseNPC* pLeader = m_pSquad->GetLeader();
				CHoundeyePoint* pLeaderTarg = dynamic_cast<CHoundeyePoint*>(pLeader->GetTarget());

				if (pLeader->HasCondition(COND_SEE_ENEMY) || pLeader->HasCondition(COND_HEAR_COMBAT) || pLeader->HasCondition(COND_HEAR_DANGER) || pLeader->HasCondition(COND_SEE_PLAYER)) {
					m_bAtPoint = false;
					return SCHED_HEYE_HUNT;
				}

				if (pLeaderTarg != nullptr) {
					CBaseEntity* pTarg = pLeader->GetTarget();
					if (pTarg != nullptr) {
						SetTarget(pTarg);
						return SCHED_HEYE_MOVE_TO_WAYPOINT;
					}
				}
			}

		}
		else {
			CHoundeyePoint* pPoint = CHoundeyePointManager::GetInstance().GetPointClosestToPos(WorldSpaceCenter());

			if (pPoint != nullptr) {
				m_pWaypoint = pPoint;
				SetTarget(m_pWaypoint);
				return SCHED_HEYE_MOVE_TO_WAYPOINT;
			}
		}
		
		if (m_nSleepState == SLEEP_NOT_SLEEPING)
			return SCHED_HEYE_START_SNOOZE;
		else
			return SCHED_HEYE_SNOOZE;

		break;

	case SCHED_MOVE_TO_WEAPON_RANGE:
		//return SCHED_HEYE_HUNT; // fall (for now)

	case SCHED_RANGE_ATTACK1:
		return SCHED_HEYE_PLOTTING;

	case SCHED_COMBAT_FACE:

		// only need to take this over in the cowering mode
		if (m_bIsCowering)
			return SCHED_HEYE_COWER;

		break;
	}	

	return BaseClass::TranslateSchedule(nType);
}

int CNPC_Houndeye::OnTakeDamage_Alive(const CTakeDamageInfo& info) {

	// dont ever take sonic dmaage, if we're ammune to sonic damage 
	// from other houndeyes, things like thumpers shouldn't do
	// any damage at all either.
	if (info.GetDamageType() == DMG_SONIC)
		return 0;

	// TODO: flinch?

	// take damage
	float dmg = info.GetBaseDamage();

	if (dmg > 0.0f) {
		m_iHealth -= dmg;
		return dmg;
	}

	if (m_pSquad) {
		SetCondition(COND_HEYE_SQUAD_ALERT);
		m_pSquad->GetLeader()->SetCondition(COND_HEYE_SQUAD_ALERT);
	}
	
	if (m_nSleepState != SLEEP_NOT_SLEEPING && !m_bAtPoint && !HasCondition(COND_HEYE_HEALTH_LOW)) {
		SetSchedule(SCHED_HEYE_SPOOKED_AWAKE);

		// ensure we wake all of the squad members, incase one of them doesnt hear the combat / danger
		AISquadIter_t iter;
		for (CAI_BaseNPC* pMember = m_pSquad->GetFirstMember(&iter); pMember != nullptr; pMember = m_pSquad->GetNextMember(&iter)) {
			pMember->SetSchedule(SCHED_HEYE_SPOOKED_AWAKE);
		}

	}

	return BaseClass::OnTakeDamage_Alive(info);
}

int CNPC_Houndeye::OnTakeDamage_Dying(const CTakeDamageInfo& info) {
	return BaseClass::OnTakeDamage_Dying(info);
}

int CNPC_Houndeye::OnTakeDamage_Dead(const CTakeDamageInfo& info) {
	return BaseClass::OnTakeDamage_Dead(info);
}

void CNPC_Houndeye::Event_Killed(const CTakeDamageInfo& info) {
	return BaseClass::Event_Killed(info);
}

void CNPC_Houndeye::TraceAttack(const CTakeDamageInfo& info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator) {
	BaseClass::TraceAttack(info, vecDir, ptr, pAccumulator);
}