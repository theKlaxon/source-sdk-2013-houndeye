//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include <hud_basechat.h>


class CHudChat : public CBaseHudChat {
	DECLARE_CLASS_SIMPLE( CHudChat, CBaseHudChat );
public:
	CHudChat( const char* pElementName );

	virtual void Init( void );

	void MsgFunc_SayText( bf_read& msg );
	void MsgFunc_SayText2( bf_read& msg );
	void MsgFunc_TextMsg( bf_read& msg );
};
