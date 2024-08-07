//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "clientsideeffects.h"
#include "mathlib/vector.h"

class IMaterial;

class CFXDiscreetLine : public CClientSideEffect {
public:
	CFXDiscreetLine( const char* name, const Vector& start, const Vector& direction, float velocity,
					 float length, float clipLength, float scale, float life, const char* shader );
	~CFXDiscreetLine( void );

	virtual void Draw( double frametime );
	virtual bool IsActive( void );
	virtual void Destroy( void );
	virtual void Update( double frametime );

protected:
	IMaterial* m_pMaterial;
	float m_fLife;
	Vector m_vecOrigin, m_vecDirection;
	float m_fVelocity;
	float m_fStartTime;
	float m_fClipLength;
	float m_fScale;
	float m_fLength;
};
