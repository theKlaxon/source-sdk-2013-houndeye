//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "entityblocker.h"
#include "fourwheelvehiclephysics.h"
#include "iservervehicle.h"
#include "phys_controller.h"
#include "props.h"
#include "vehicle_baseserver.h"
#include "vehicle_sounds.h"
#include "vehicle_viewblend_shared.h"
#include "vphysics/vehicles.h"

class CNPC_VehicleDriver;
class CFourWheelVehiclePhysics;
class CPropVehicleDriveable;
class CSoundPatch;

// the tires are considered to be skidding if they have sliding velocity of 10 in/s or more
const float DEFAULT_SKID_THRESHOLD = 10.0f;

//-----------------------------------------------------------------------------
// Purpose: Four wheel physics vehicle server vehicle
//-----------------------------------------------------------------------------
class CFourWheelServerVehicle : public CBaseServerVehicle {
	DECLARE_CLASS( CFourWheelServerVehicle, CBaseServerVehicle );

	// IServerVehicle
public:
	virtual ~CFourWheelServerVehicle() {
	}

	CFourWheelServerVehicle();
	virtual bool IsVehicleUpright();
	virtual bool IsVehicleBodyInWater();
	virtual void GetVehicleViewPosition( int nRole, Vector* pOrigin, QAngle* pAngles, float* pFOV = NULL );
	IPhysicsVehicleController* GetVehicleController();
	const vehicleparams_t* GetVehicleParams();
	const vehicle_controlparams_t* GetVehicleControlParams();
	const vehicle_operatingparams_t* GetVehicleOperatingParams();

	// NPC Driving
	void NPC_SetDriver( CNPC_VehicleDriver* pDriver );
	void NPC_DriveVehicle();

	CPropVehicleDriveable* GetFourWheelVehicle();
	bool GetWheelContactPoint( int nWheelIndex, Vector& vecPos );

public:
	virtual void SetVehicle( CBaseEntity* pVehicle );
	void InitViewSmoothing( const Vector& vecStartOrigin, const QAngle& vecStartAngles );
	bool IsPassengerEntering();
	bool IsPassengerExiting();

	DECLARE_SIMPLE_DATADESC();

private:
	CFourWheelVehiclePhysics* GetFourWheelVehiclePhysics();

	ViewSmoothingData_t m_ViewSmoothing;
};

//-----------------------------------------------------------------------------
// Purpose: Base class for four wheel physics vehicles
//-----------------------------------------------------------------------------
class CPropVehicle : public CBaseProp, public CDefaultPlayerPickupVPhysics {
	DECLARE_CLASS( CPropVehicle, CBaseProp );

public:
	CPropVehicle();
	virtual ~CPropVehicle();

	void SetVehicleType( unsigned int nVehicleType ) { m_nVehicleType = nVehicleType; }
	unsigned int GetVehicleType() { return m_nVehicleType; }

	// CBaseEntity
	void Spawn();
	virtual int Restore( IRestore& restore );
	void VPhysicsUpdate( IPhysicsObject* pPhysics );
	void DrawDebugGeometryOverlays();
	int DrawDebugTextOverlays();
	void Teleport( const Vector* newPosition, const QAngle* newAngles, const Vector* newVelocity );
	virtual void Think();
	CFourWheelVehiclePhysics* GetPhysics() { return &m_VehiclePhysics; }
	CBasePlayer* HasPhysicsAttacker( float dt );
	void OnPhysGunPickup( CBasePlayer* pPhysGunUser, PhysGunPickup_t reason );

	Vector GetSmoothedVelocity();//Save and update our smoothed velocity for prediction

	virtual void DampenEyePosition( Vector& vecVehicleEyePos, QAngle& vecVehicleEyeAngles ) {}

	// Inputs
	void InputThrottle( inputdata_t& inputdata );
	void InputSteering( inputdata_t& inputdata );
	void InputAction( inputdata_t& inputdata );
	void InputHandBrakeOn( inputdata_t& inputdata );
	void InputHandBrakeOff( inputdata_t& inputdata );

	DECLARE_DATADESC();

#ifdef HL2_EPISODIC
	void AddPhysicsChild( CBaseEntity* pChild );
	void RemovePhysicsChild( CBaseEntity* pChild );
#endif//HL2_EPISODIC

protected:
	// engine sounds
	void SoundInit();
	void SoundShutdown();
	void SoundUpdate( const vehicle_operatingparams_t& params, const vehicleparams_t& vehicle );
	void CalcWheelData( vehicleparams_t& vehicle );
	void ResetControls();

	// Upright strength of the controller (angular limit)
	virtual float GetUprightStrength() { return 8.0f; }
	virtual float GetUprightTime() { return 5.0f; }

protected:
	CFourWheelVehiclePhysics m_VehiclePhysics;
	unsigned int m_nVehicleType;
	string_t m_vehicleScript;

	#ifdef HL2_EPISODIC
		CUtlVector<EHANDLE> m_hPhysicsChildren;// List of entities who wish to get physics callbacks from the vehicle
	#endif

private:
	Vector m_vecSmoothedVelocity;

	CHandle<CBasePlayer> m_hPhysicsAttacker;

	float m_flLastPhysicsInfluenceTime;
};

//=============================================================================
// NPC Passenger Carrier interface

class INPCPassengerCarrier {
public:
	virtual bool NPC_CanEnterVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) = 0;
	virtual bool NPC_CanExitVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) = 0;
	virtual bool NPC_AddPassenger( CAI_BaseNPC* pPassenger, string_t strRoleName, int nSeatID ) = 0;
	virtual bool NPC_RemovePassenger( CAI_BaseNPC* pPassenger ) = 0;
	virtual void NPC_FinishedEnterVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) = 0;
	virtual void NPC_FinishedExitVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) = 0;
};

//-----------------------------------------------------------------------------
// Purpose: Drivable four wheel physics vehicles
//-----------------------------------------------------------------------------
class CPropVehicleDriveable : public CPropVehicle, public IDrivableVehicle, public INPCPassengerCarrier {
	DECLARE_CLASS( CPropVehicleDriveable, CPropVehicle );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

public:
	CPropVehicleDriveable();
	~CPropVehicleDriveable();

	virtual void Precache();
	virtual void Spawn();
	virtual int Restore( IRestore& restore );
	virtual void OnRestore();
	virtual void CreateServerVehicle();
	virtual int ObjectCaps() { return BaseClass::ObjectCaps() | FCAP_IMPULSE_USE; };
	virtual void GetVectors( Vector* pForward, Vector* pRight, Vector* pUp ) const;
	virtual void VehicleAngleVectors( const QAngle& angles, Vector* pForward, Vector* pRight, Vector* pUp );
	virtual void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value );
	virtual void Think();
	virtual void TraceAttack( const CTakeDamageInfo& info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator );
	virtual void Event_KilledOther( CBaseEntity* pVictim, const CTakeDamageInfo& info );

	// Vehicle handling
	virtual void VPhysicsCollision( int index, gamevcollisionevent_t* pEvent );
	virtual int VPhysicsGetObjectList( IPhysicsObject** pList, int listMax );

	// Inputs
	void InputLock( inputdata_t& inputdata );
	void InputUnlock( inputdata_t& inputdata );
	void InputTurnOn( inputdata_t& inputdata );
	void InputTurnOff( inputdata_t& inputdata );

	// Locals
	void ResetUseKey( CBasePlayer* pPlayer );

	// Driving
	void DriveVehicle( CBasePlayer* pPlayer, CUserCmd* ucmd );                                             // Player driving entrypoint
	virtual void DriveVehicle( float flFrameTime, CUserCmd* ucmd, int iButtonsDown, int iButtonsReleased );// Driving Button handling

	virtual bool IsOverturned();
	virtual bool IsVehicleBodyInWater() { return false; }

	// Engine handling
	void StartEngine();
	void StopEngine();
	bool IsEngineOn();

	// IDrivableVehicle
public:
	virtual CBaseEntity* GetDriver();
	virtual void ItemPostFrame( CBasePlayer* pPlayer ) { }
	virtual void SetupMove( CBasePlayer* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move );
	virtual void ProcessMovement( CBasePlayer* pPlayer, CMoveData* pMoveData ) { }
	virtual void FinishMove( CBasePlayer* player, CUserCmd* ucmd, CMoveData* move ) { }
	virtual bool CanEnterVehicle( CBaseEntity* pEntity );
	virtual bool CanExitVehicle( CBaseEntity* pEntity );
	virtual void SetVehicleEntryAnim( bool bOn ) { m_bEnterAnimOn = bOn; }
	virtual void SetVehicleExitAnim( bool bOn, Vector vecEyeExitEndpoint ) {
		m_bExitAnimOn = bOn;
		if ( bOn ) m_vecEyeExitEndpoint = vecEyeExitEndpoint;
	}
	virtual void EnterVehicle( CBaseCombatCharacter* pPassenger );

	virtual bool AllowBlockedExit( CBaseCombatCharacter* pPassenger, int nRole ) { return true; }
	virtual bool AllowMidairExit( CBaseCombatCharacter* pPassenger, int nRole ) { return false; }
	virtual void PreExitVehicle( CBaseCombatCharacter* pPassenger, int nRole ) {}
	virtual void ExitVehicle( int nRole );
	virtual string_t GetVehicleScriptName() { return m_vehicleScript; }

	virtual bool PassengerShouldReceiveDamage( CTakeDamageInfo& info ) { return true; }

	// If this is a vehicle, returns the vehicle interface
	virtual IServerVehicle* GetServerVehicle() { return m_pServerVehicle; }

protected:
	virtual bool ShouldThink() { return ( GetDriver() != NULL ); }

	inline bool HasGun();
	void DestroyServerVehicle();

	// Contained IServerVehicle
	CFourWheelServerVehicle* m_pServerVehicle;

	COutputEvent m_playerOn;
	COutputEvent m_playerOff;

	COutputEvent m_pressedAttack;
	COutputEvent m_pressedAttack2;

	COutputFloat m_attackaxis;
	COutputFloat m_attack2axis;

	CNetworkHandle( CBasePlayer, m_hPlayer );

public:
	CNetworkVar( int, m_nSpeed );
	CNetworkVar( int, m_nRPM );
	CNetworkVar( float, m_flThrottle );
	CNetworkVar( int, m_nBoostTimeLeft );
	CNetworkVar( int, m_nHasBoost );

	CNetworkVector( m_vecEyeExitEndpoint );
	CNetworkVector( m_vecGunCrosshair );
	CNetworkVar( bool, m_bUnableToFire );
	CNetworkVar( bool, m_bHasGun );

	CNetworkVar( bool, m_nScannerDisabledWeapons );
	CNetworkVar( bool, m_nScannerDisabledVehicle );

	// NPC Driver
	CHandle<CNPC_VehicleDriver> m_hNPCDriver;
	EHANDLE m_hKeepUpright;

	// --------------------------------
	// NPC Passengers
public:
	virtual bool NPC_CanEnterVehicle( CAI_BaseNPC* pPassenger, bool bCompanion );
	virtual bool NPC_CanExitVehicle( CAI_BaseNPC* pPassenger, bool bCompanion );
	virtual bool NPC_AddPassenger( CAI_BaseNPC* pPassenger, string_t strRoleName, int nSeatID );
	virtual bool NPC_RemovePassenger( CAI_BaseNPC* pPassenger );
	virtual void NPC_FinishedEnterVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) {}
	virtual void NPC_FinishedExitVehicle( CAI_BaseNPC* pPassenger, bool bCompanion ) {}

	// NPC Passengers
	// --------------------------------

	bool IsEnterAnimOn() { return m_bEnterAnimOn; }
	bool IsExitAnimOn() { return m_bExitAnimOn; }
	const Vector& GetEyeExitEndpoint() { return m_vecEyeExitEndpoint; }

protected:
	// Entering / Exiting
	bool m_bEngineLocked;// Mapmaker override on whether the vehicle's allowed to be turned on/off
	bool m_bLocked;
	float m_flMinimumSpeedToEnterExit;
	CNetworkVar( bool, m_bEnterAnimOn );
	CNetworkVar( bool, m_bExitAnimOn );

	// Used to turn the keepupright off after a short time
	float m_flTurnOffKeepUpright;
	float m_flNoImpactDamageTime;
};


inline bool CPropVehicleDriveable::HasGun() {
	return m_bHasGun;
}
