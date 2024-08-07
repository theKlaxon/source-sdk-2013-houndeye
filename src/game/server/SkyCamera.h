//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Resource collection entity
//
// $NoKeywords: $
//=============================================================================//
#pragma once

class CSkyCamera;

//=============================================================================
//
// Sky Camera Class
//
class CSkyCamera : public CLogicalEntity {
	DECLARE_CLASS( CSkyCamera, CLogicalEntity );

public:
	DECLARE_DATADESC();
	CSkyCamera();
	~CSkyCamera();
	virtual void Spawn();
	virtual void Activate();

public:
	sky3dparams_t m_skyboxData;
	bool m_bUseAngles;
	CSkyCamera* m_pNext;
};


//-----------------------------------------------------------------------------
// Retrives the current skycamera
//-----------------------------------------------------------------------------
CSkyCamera* GetCurrentSkyCamera();
CSkyCamera* GetSkyCameraList();
