//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A base class for all material proxies in the client dll
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include "materialsystem/imaterialproxy.h"


class IMaterialVar;

//-----------------------------------------------------------------------------
// Base class all material proxies should inherit from
//-----------------------------------------------------------------------------
abstract_class CEntityMaterialProxy : public IMaterialProxy {
public:
	virtual void Release();
	virtual void OnBind( void* pC_BaseEntity );

protected:
	// base classes should implement these
	virtual void OnBind( C_BaseEntity * pBaseEntity ) = 0;
};
