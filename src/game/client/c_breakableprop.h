//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class C_BreakableProp : public C_BaseAnimating {
	typedef C_BaseAnimating BaseClass;
public:
	DECLARE_CLIENTCLASS();

	C_BreakableProp();

	virtual void SetFadeMinMax( float fademin, float fademax );

	// Copy fade from another breakable prop
	void CopyFadeFrom( C_BreakableProp* pSource );
};
