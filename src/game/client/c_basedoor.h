//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "c_baseentity.h"

#if defined( CLIENT_DLL )
	#define CBaseDoor C_BaseDoor
#endif

class C_BaseDoor : public C_BaseEntity {
public:
	DECLARE_CLASS( C_BaseDoor, C_BaseEntity );
	DECLARE_CLIENTCLASS();

	C_BaseDoor( void );
	~C_BaseDoor( void );

public:
	float m_flWaveHeight;
};