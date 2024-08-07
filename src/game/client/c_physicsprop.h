//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "c_breakableprop.h"


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class C_PhysicsProp : public C_BreakableProp {
	typedef C_BreakableProp BaseClass;

public:
	DECLARE_CLIENTCLASS();

	C_PhysicsProp();
	~C_PhysicsProp();

	virtual bool OnInternalDrawModel( ClientModelRenderInfo_t* pInfo );

protected:
	// Networked vars.
	bool m_bAwake;
	bool m_bAwakeLastTime;
};
