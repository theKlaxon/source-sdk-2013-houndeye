//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once

class CViewSetup;

//-----------------------------------------------------------------------------
// Purpose: Implements the debugging elements of view rendering
//-----------------------------------------------------------------------------
class CDebugViewRender {
	DECLARE_CLASS_NOBASE( CDebugViewRender );
public:
	// Draws all the debugging info
	static void Draw3DDebuggingInfo( const CViewSetup& view );
	static void Draw2DDebuggingInfo( const CViewSetup& view );
	static void GenerateOverdrawForTesting();
};
