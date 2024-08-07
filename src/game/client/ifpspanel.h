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
#include <vgui/VGUI.h>

namespace vgui {
	class Panel;
}

abstract_class IFPSPanel {
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
};

abstract_class IShowBlockingPanel {
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
};


extern IFPSPanel* fps;
extern IShowBlockingPanel* iopanel;
