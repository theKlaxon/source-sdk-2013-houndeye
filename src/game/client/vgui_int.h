//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "interface.h"

#include <vgui/VGUI.h>

namespace vgui {
	class Panel;
}

bool VGui_Startup( CreateInterfaceFn appSystemFactory );
void VGui_Shutdown( void );
void VGui_CreateGlobalPanels( void );
vgui::VPANEL VGui_GetClientDLLRootPanel( void );
void VGUI_CreateClientDLLRootPanel( void );
void VGUI_DestroyClientDLLRootPanel( void );
void VGui_PreRender();
void VGui_PostRender();
