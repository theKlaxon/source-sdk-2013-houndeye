//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================//
#pragma once
#include <vgui/KeyCode.h>

// Convert from Windows scan codes to VGUI key codes.
vgui::KeyCode KeyCode_VirtualKeyToVGUI( int key );
int KeyCode_VGUIToVirtualKey( vgui::KeyCode keycode );
