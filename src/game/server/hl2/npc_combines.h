//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "npc_combine.h"

//=========================================================
//	>> CNPC_CombineS
//=========================================================
class CNPC_CombineS : public CNPC_Combine {
	DECLARE_CLASS( CNPC_CombineS, CNPC_Combine );
	#if HL2_EPISODIC
		DECLARE_DATADESC();
	#endif
public:
	void Spawn();
	void Precache();
	void DeathSound( const CTakeDamageInfo& info );
	void PrescheduleThink();
	void BuildScheduleTestBits();
	int SelectSchedule();
	float GetHitgroupDamageMultiplier( int iHitGroup, const CTakeDamageInfo& info );
	void HandleAnimEvent( animevent_t* pEvent );
	void OnChangeActivity( Activity eNewActivity );
	void Event_Killed( const CTakeDamageInfo& info );
	void OnListened();

	void ClearAttackConditions();

	bool m_fIsBlocking;

	bool IsLightDamage( const CTakeDamageInfo& info );
	bool IsHeavyDamage( const CTakeDamageInfo& info );

	virtual bool AllowedToIgnite() { return true; }

private:
	bool ShouldHitPlayer( const Vector& targetDir, float targetDist );

	#if HL2_EPISODIC
		public:
			Activity NPC_TranslateActivity( Activity eNewActivity );

		protected:
			/// whether to use the more casual march anim in ep2_outland_05
			int m_iUseMarch;
	#endif
};
