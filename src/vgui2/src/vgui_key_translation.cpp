//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//
#if defined( PLATFORM_WINDOWS )
	#include <wtypes.h>
	#include <winuser.h>
	#include "xbox/xboxstubs.h"
#endif
#include "vgui_key_translation.h"
#include "tier0/dbg.h"
#if IsPosix()
	#define VK_RETURN -1
#endif

#include "inputsystem/iinputsystem.h"
#include "tier2/tier2.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

vgui::KeyCode KeyCode_VirtualKeyToVGUI( int key ) {
	// Some tools load vgui for localization and never use input
	if ( !g_pInputSystem ) {
		return KEY_NONE;
	}
	return g_pInputSystem->VirtualKeyToButtonCode( key );
}

int KeyCode_VGUIToVirtualKey( vgui::KeyCode code ) {
	// Some tools load vgui for localization and never use input
	if ( !g_pInputSystem ) {
		return VK_RETURN;
	}

	return g_pInputSystem->ButtonCodeToVirtualKey( code );
}
