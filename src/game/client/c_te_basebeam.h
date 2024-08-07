//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "c_basetempentity.h"

//-----------------------------------------------------------------------------
// Purpose: Base entity for beam te's
//-----------------------------------------------------------------------------
class C_TEBaseBeam : public C_BaseTempEntity {
public:
	DECLARE_CLASS( C_TEBaseBeam, C_BaseTempEntity );
	DECLARE_CLIENTCLASS();

private:
public:
	C_TEBaseBeam( void );
	virtual ~C_TEBaseBeam( void );

	virtual void PreDataUpdate( DataUpdateType_t updateType );
	virtual void PostDataUpdate( DataUpdateType_t updateType );

public:
	int m_nModelIndex;
	int m_nHaloIndex;
	int m_nStartFrame;
	int m_nFrameRate;
	float m_fLife;
	float m_fWidth;
	float m_fEndWidth;
	int m_nFadeLength;
	float m_fAmplitude;
	int r, g, b, a;
	int m_nSpeed;
	int m_nFlags;
};

EXTERN_RECV_TABLE( DT_BaseBeam );
