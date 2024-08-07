//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "ispatialpartition.h"

//Enumator class for ragdolls being affected by explosive forces
class CRagdollExplosionEnumerator : public IPartitionEnumerator {
	DECLARE_CLASS_GAMEROOT( CRagdollExplosionEnumerator, IPartitionEnumerator );

public:
	//Forced constructor
	CRagdollExplosionEnumerator( Vector origin, float radius, float magnitude );
	~CRagdollExplosionEnumerator();

	//Actual work code
	virtual IterationRetval_t EnumElement( IHandleEntity* pHandleEntity );

public:
	//Data members
	CUtlVector<C_BaseEntity*> m_Entities;
	Vector m_vecOrigin;
	float m_flMagnitude;
	float m_flRadius;
};
