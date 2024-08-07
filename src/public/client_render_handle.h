//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Foward declarations
//-----------------------------------------------------------------------------
class IClientRenderable;


//-----------------------------------------------------------------------------
// Handle to an renderable in the client leaf system
//-----------------------------------------------------------------------------
typedef unsigned short ClientRenderHandle_t;

enum {
	INVALID_CLIENT_RENDER_HANDLE = (ClientRenderHandle_t) 0xffff,
};
