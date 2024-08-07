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
#include "viewport_panel_names.h"
#include <vgui/VGUI.h>

class KeyValues;

abstract_class IViewPortPanel {

public:
	virtual ~IViewPortPanel() { };

	virtual const char* GetName() = 0;           // return identifer name
	virtual void SetData( KeyValues * data ) = 0;// set ViewPortPanel data
	virtual void Reset() = 0;                    // clears internal state, deactivates it
	virtual void Update() = 0;                   // updates all (size, position, content, etc)
	virtual bool NeedsUpdate() = 0;              // query panel if content needs to be updated
	virtual bool HasInputElements() = 0;         // true if panel contains elments which accepts input

	virtual void ShowPanel( bool state ) = 0;// activate VGUI Frame

	// VGUI functions:
	virtual vgui::VPANEL GetVPanel() = 0;// returns VGUI panel handle
	virtual bool IsVisible() = 0;        // true if panel is visible
	virtual void SetParent( vgui::VPANEL parent ) = 0;
};

abstract_class IViewPort {
public:
	virtual void UpdateAllPanels() = 0;
	virtual void ShowPanel( const char* pName, bool state ) = 0;
	virtual void ShowPanel( IViewPortPanel * pPanel, bool state ) = 0;
	virtual void ShowBackGround( bool bShow ) = 0;
	virtual IViewPortPanel* FindPanelByName( const char* szPanelName ) = 0;
	virtual IViewPortPanel* GetActivePanel() = 0;
	virtual void PostMessageToPanel( const char* pName, KeyValues* pKeyValues ) = 0;
};

extern IViewPort* gViewPortInterface;
