//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "iserverunknown.h"
#include "string_t.h"


struct Ray_t;
class ServerClass;
class ICollideable;
class IServerNetworkable;
class Vector;
class QAngle;

// This class is how the engine talks to entities in the game DLL.
// CBaseEntity implements this interface.
class IServerEntity : public IServerUnknown {
public:
	virtual ~IServerEntity() {}

	// Previously in pev
	virtual int GetModelIndex() const = 0;
	virtual string_t GetModelName() const = 0;

	virtual void SetModelIndex( int index ) = 0;
};
