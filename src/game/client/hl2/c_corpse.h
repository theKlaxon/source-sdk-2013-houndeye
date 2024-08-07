//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once


class C_Corpse : public C_BaseAnimating {
public:
	DECLARE_CLASS( C_Corpse, C_BaseAnimating );
	DECLARE_CLIENTCLASS();

	C_Corpse( void );

	virtual int DrawModel( int flags );

public:
	// The player whom we are copying our data from
	int m_nReferencePlayer;

private:
	C_Corpse( const C_Corpse& );
};
