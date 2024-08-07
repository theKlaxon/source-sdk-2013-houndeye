//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once

//-----------------------------------------------------------------------------
// For networking this bad boy
//-----------------------------------------------------------------------------
#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_EntityParticleTrailInfo );
#else
EXTERN_SEND_TABLE( DT_EntityParticleTrailInfo );
#endif


//-----------------------------------------------------------------------------
// Particle trail info
//-----------------------------------------------------------------------------
struct EntityParticleTrailInfo_t {
	EntityParticleTrailInfo_t();

	DECLARE_CLASS_NOBASE( EntityParticleTrailInfo_t );
	DECLARE_SIMPLE_DATADESC();
	DECLARE_EMBEDDED_NETWORKVAR();

	string_t m_strMaterialName;
	CNetworkVar( float, m_flLifetime );
	CNetworkVar( float, m_flStartSize );
	CNetworkVar( float, m_flEndSize );
};
