//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//

#if !defined( INETGRAPHPANEL_H )
#define INETGRAPHPANEL_H
#if IsWindows()
#pragma once
#endif

#include <vgui/VGUI.h>

namespace vgui
{
	class Panel;
}

abstract_class INetGraphPanel
{
public:
	virtual void			Create( vgui::VPANEL parent ) = 0;
	virtual void			Destroy( void ) = 0;
};

extern INetGraphPanel *netgraphpanel;

#endif // INETGRAPHPANEL_H