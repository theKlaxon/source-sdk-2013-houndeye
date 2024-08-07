//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "clientmode_shared.h"
#include <vgui/Cursor.h>
#include <vgui_controls/EditablePanel.h>

class CHudViewport;

namespace vgui {
	typedef unsigned long HScheme;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class ClientModeHLNormal : public ClientModeShared {
public:
	DECLARE_CLASS( ClientModeHLNormal, ClientModeShared );

	ClientModeHLNormal();
	~ClientModeHLNormal();

	virtual void Init();
	virtual bool ShouldDrawCrosshair( void );
};

extern IClientMode* GetClientModeNormal();
extern vgui::HScheme g_hVGuiCombineScheme;
