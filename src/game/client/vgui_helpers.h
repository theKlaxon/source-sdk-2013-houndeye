//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include <vgui_controls/Label.h>
#include <vgui_controls/Panel.h>


inline int PanelTop( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return y;
}
inline int PanelLeft( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return x;
}
inline int PanelRight( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return x + w;
}
inline int PanelBottom( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return y + h;
}
inline int PanelWidth( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return w;
}
inline int PanelHeight( vgui::Panel* pPanel ) {
	int x, y, w, h;
	pPanel->GetBounds( x, y, w, h );
	return h;
}

// Places child at the requested position inside pParent. iAlignment is from Label::Alignment.
void AlignPanel( vgui::Panel* pChild, vgui::Panel* pParent, int alignment );
