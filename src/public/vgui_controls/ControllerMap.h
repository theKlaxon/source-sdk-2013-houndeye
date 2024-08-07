//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "Panel.h"
#include "utlmap.h"
#include "utlsymbol.h"

class CControllerMap : public vgui::Panel {
	DECLARE_CLASS_SIMPLE( CControllerMap, vgui::Panel )

	virtual void OnKeyCodeTyped( vgui::KeyCode code );

public:
	CControllerMap( vgui::Panel* parent, const char* name );

	virtual void ApplySettings( KeyValues* inResourceData );

	int NumButtons() {
		return m_buttonMap.Count();
	}

	const char* GetBindingText( int idx );
	const char* GetBindingIcon( int idx );

private:
	struct button_t {
		CUtlSymbol cmd;
		CUtlSymbol text;
		CUtlSymbol icon;
	};
	CUtlMap<int, button_t> m_buttonMap;
};
