//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "ehandle.h"

class CBaseEntity;


class CBaseTransmitProxy {
public:
	CBaseTransmitProxy( CBaseEntity* pEnt );
	virtual ~CBaseTransmitProxy();

	// Override this to control the ShouldTransmit behavior of whatever entity the proxy is attached to.
	// bPrevShouldTransmitResult is what the proxy's entity's ShouldTransmit() returned.
	virtual int ShouldTransmit( const CCheckTransmitInfo* pInfo, int nPrevShouldTransmitResult );

	void AddRef();
	void Release();
private:
	EHANDLE m_hEnt;
	unsigned short m_refCount;
};
