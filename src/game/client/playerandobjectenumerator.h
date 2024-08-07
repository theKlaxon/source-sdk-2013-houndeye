//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "ehandle.h"
#include "ispatialpartition.h"
#include "utlvector.h"

class C_BaseEntity;
class C_BasePlayer;

// Enumator class for finding other players and objects close to the
//  local player
class CPlayerAndObjectEnumerator : public IPartitionEnumerator {
	DECLARE_CLASS_NOBASE( CPlayerAndObjectEnumerator );

public:
	//Forced constructor
	CPlayerAndObjectEnumerator( float radius );

	//Actual work code
	virtual IterationRetval_t EnumElement( IHandleEntity* pHandleEntity );

	int GetObjectCount();
	C_BaseEntity* GetObject( int index );

public:
	//Data members
	float m_flRadiusSquared;

	CUtlVector<CHandle<C_BaseEntity>> m_Objects;
	C_BasePlayer* m_pLocal;
};
