//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Are tools enabled? 
//-----------------------------------------------------------------------------
#ifndef NO_TOOLFRAMEWORK
	bool ToolsEnabled();
#else
	#define ToolsEnabled() 0
#endif
