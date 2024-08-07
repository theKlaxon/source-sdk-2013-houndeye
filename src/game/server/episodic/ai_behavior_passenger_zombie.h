//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Zombies on cars!
//
//=============================================================================
#pragma once
#include "ai_behavior_passenger.h"
#include "ai_utils.h"
#include "vehicle_base.h"

extern impactdamagetable_t gZombiePassengerImpactDamageTable;

class CAI_PassengerBehaviorZombie : public CAI_PassengerBehavior {
	DECLARE_CLASS( CAI_PassengerBehaviorZombie, CAI_PassengerBehavior );
	DECLARE_DATADESC()

public:
	CAI_PassengerBehaviorZombie();

	enum {
		// Schedules
		SCHED_PASSENGER_ZOMBIE_ENTER_VEHICLE = BaseClass::NEXT_SCHEDULE,
		SCHED_PASSENGER_ZOMBIE_EXIT_VEHICLE,
		SCHED_PASSENGER_ZOMBIE_MELEE_ATTACK1,
		SCHED_PASSENGER_ZOMBIE_RANGE_ATTACK1,
		SCHED_PASSENGER_ZOMBIE_ATTACH,
		SCHED_PASSENGER_ZOMBIE_RUN_TO_VEHICLE,
		NEXT_SCHEDULE,

		// Tasks
		TASK_PASSENGER_ZOMBIE_RANGE_ATTACK1 = BaseClass::NEXT_TASK,
		TASK_PASSENGER_ZOMBIE_DISMOUNT,
		TASK_PASSENGER_ZOMBIE_ATTACH,
		NEXT_TASK,

		// Conditions
		COND_PASSENGER_ZOMBIE_CAN_ATTACH_TO_VEHICLE = BaseClass::NEXT_CONDITION,
		NEXT_CONDITION
	};

	virtual const char* GetName() { return "ZombiePassenger"; }
	virtual string_t GetRoleName() { return MAKE_STRING( "passenger_zombie" ); }
	virtual int SelectSchedule();
	virtual int TranslateSchedule( int scheduleType );
	virtual void GatherConditions();
	virtual void Event_Killed( const CTakeDamageInfo& info );
	virtual void BuildScheduleTestBits();
	virtual void RunTask( const Task_t* pTask );
	virtual void StartTask( const Task_t* pTask );
	virtual bool CanEnterVehicle();
	virtual void ExitVehicle();
	virtual void HandleAnimEvent( animevent_t* pEvent );
	virtual Activity NPC_TranslateActivity( Activity activity );

	virtual bool AttachToVehicle();

	void SuppressAttack( float flDuration );

	DEFINE_CUSTOM_SCHEDULE_PROVIDER;

protected:
	int SelectOutsideSchedule();
	int SelectInsideSchedule();
	virtual int FindExitSequence();
	void StartDismount();
	void FinishDismount();
	virtual void CalculateBodyLean();
	virtual void GatherVehicleStateConditions();
	virtual int FindEntrySequence( bool bNearest = false );

private:
	void VehicleLeapAttackTouch( CBaseEntity* pOther );
	void VehicleLeapAttack();
	bool CanBeOnEnemyVehicle();
	float GetEntryPointCost( const Vector& vecEntryPos );
	bool EnemyInVehicle();
	void GetAttachmentPoint( Vector* vecPoint );
	bool CanJumpToAttachToVehicle();
	//bool				WithinAttachRange();

	float m_flLastLateralLean;
	float m_flLastVerticalLean;
	float m_flNextLeapTime;
};
