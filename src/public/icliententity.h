//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "iclientnetworkable.h"
#include "iclientrenderable.h"
#include "iclientthinkable.h"

struct Ray_t;
class CGameTrace;
typedef CGameTrace trace_t;
class CMouthInfo;
class IClientEntityInternal;
struct SpatializationInfo_t;


//-----------------------------------------------------------------------------
// Purpose: All client entities must implement this interface.
//-----------------------------------------------------------------------------
abstract_class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable {
public:
	// Delete yourself.
	virtual void Release() = 0;

	// Network origin + angles
	virtual const Vector& GetAbsOrigin() const = 0;
	virtual const QAngle& GetAbsAngles() const = 0;

	virtual CMouthInfo* GetMouth() = 0;

	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	virtual bool GetSoundSpatialization( SpatializationInfo_t & info ) = 0;
};
