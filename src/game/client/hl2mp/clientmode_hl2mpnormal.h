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
class ClientModeHL2MPNormal : public ClientModeShared {
public:
	DECLARE_CLASS( ClientModeHL2MPNormal, ClientModeShared );

	ClientModeHL2MPNormal();
	~ClientModeHL2MPNormal();

	virtual void Init();
	virtual int GetDeathMessageStartHeight( void );
};

extern IClientMode* GetClientModeNormal();
extern vgui::HScheme g_hVGuiCombineScheme;

extern ClientModeHL2MPNormal* GetClientModeHL2MPNormal();
