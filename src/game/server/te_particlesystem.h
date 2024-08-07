//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "basetempentity.h"


class CTEParticleSystem : public CBaseTempEntity {
public:
	DECLARE_CLASS( CTEParticleSystem, CBaseTempEntity );
	DECLARE_SERVERCLASS();

	CTEParticleSystem( const char* pName ) : BaseClass( pName ) {
		m_vecOrigin.GetForModify().Init();
	}

	CNetworkVector( m_vecOrigin );
};
