//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "c_basecombatcharacter.h"

// NOTE: Moved all controller code into c_basestudiomodel
class C_AI_BaseNPC : public C_BaseCombatCharacter {
	DECLARE_CLASS( C_AI_BaseNPC, C_BaseCombatCharacter );

public:
	DECLARE_CLIENTCLASS();

	C_AI_BaseNPC();
	virtual unsigned int PhysicsSolidMaskForEntity() const;
	virtual bool IsNPC() { return true; }
	bool IsMoving() { return m_bIsMoving; }
	bool ShouldAvoidObstacle() { return m_bPerformAvoidance; }
	virtual bool AddRagdollToFadeQueue() { return m_bFadeCorpse; }

	virtual bool GetRagdollInitBoneArrays( matrix3x4_t* pDeltaBones0, matrix3x4_t* pDeltaBones1, matrix3x4_t* pCurrentBones, float boneDt ) override;

	int GetDeathPose() { return m_iDeathPose; }

	bool ShouldModifyPlayerSpeed() { return m_bSpeedModActive; }
	int GetSpeedModifyRadius() { return m_iSpeedModRadius; }
	int GetSpeedModifySpeed() { return m_iSpeedModSpeed; }

	void ClientThink();
	void OnDataChanged( DataUpdateType_t type );
	bool ImportantRagdoll() { return m_bImportanRagdoll; }

private:
	C_AI_BaseNPC( const C_AI_BaseNPC& );// not defined, not accessible
	float m_flTimePingEffect;
	int m_iDeathPose;
	int m_iDeathFrame;

	int m_iSpeedModRadius;
	int m_iSpeedModSpeed;

	bool m_bPerformAvoidance;
	bool m_bIsMoving;
	bool m_bFadeCorpse;
	bool m_bSpeedModActive;
	bool m_bImportanRagdoll;
};
