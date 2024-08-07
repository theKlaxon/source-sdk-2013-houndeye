//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once


class CBaseEntity;

//-----------------------------------------------------------------------------
// Purpose: Allows an entity to access its owner regardless of entity type
//-----------------------------------------------------------------------------
class IHasOwner {
public:
	virtual CBaseEntity* GetOwnerViaInterface() = 0;
};
