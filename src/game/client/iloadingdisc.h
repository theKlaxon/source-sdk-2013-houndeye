//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include <vgui/VGUI.h>


namespace vgui {
	class Panel;
}

abstract_class ILoadingDisc {
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;

	// loading disc
	virtual void SetLoadingVisible( bool bVisible ) = 0;

	// paused disc
	virtual void SetPausedVisible( bool bVisible ) = 0;
};

extern ILoadingDisc* loadingdisc;
