//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "replay/genericclassbased_replay.h"
#include "replaybrowseritemmanager.h"
#include "vgui_controls/PropertyPage.h"


using namespace vgui;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CReplayListPanel;
class CExLabel;
class CReplayDetailsPanel;

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CReplayBrowserBasePage : public PropertyPage {
	DECLARE_CLASS_SIMPLE( CReplayBrowserBasePage, PropertyPage );

public:
	CReplayBrowserBasePage( Panel* pParent );
	virtual ~CReplayBrowserBasePage();

	void DeleteDetailsPanelAndShowReplayList();
	bool IsDetailsViewOpen();
	void GoBack();

	// Movie-only stuff
	void FreeDetailsPanelMovieLock();

	virtual void ApplySchemeSettings( IScheme* pScheme );
	virtual void OnCommand( const char* pCommand );
	virtual void PerformLayout();

	MESSAGE_FUNC( OnPageShow, "PageShow" );
	MESSAGE_FUNC( OnSelectionStarted, "SelectionStarted" );
	MESSAGE_FUNC( OnSelectionEnded, "SelectionEnded" );
	MESSAGE_FUNC( OnCancelSelection, "CancelSelection" );
	MESSAGE_FUNC_PARAMS( OnReplayItemDeleted, "ReplayItemDeleted", pParams );
	MESSAGE_FUNC_PARAMS( OnTextChanged, "TextChanged", data );

	void AddReplay( ReplayHandle_t hReplay );
	void DeleteReplay( ReplayHandle_t hReplay );

	void OnTick();

	virtual void CleanupUIForReplayItem( ReplayItemHandle_t hReplayItem );

	vgui::TextEntry* m_pSearchTextEntry;
	CReplayListPanel* m_pReplayList;
	DHANDLE<CReplayDetailsPanel> m_hReplayDetailsPanel;
};
