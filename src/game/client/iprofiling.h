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

abstract_class IProfiling {
public:
	virtual void Create( vgui::VPANEL parent ) = 0;
	virtual void Destroy( void ) = 0;
};

extern IProfiling* profiling;
