//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


//-----------------------------------------------------------------------------
// Purpose: Used for tracking many to one ground entity chains ( many ents can share a single ground entity )
//-----------------------------------------------------------------------------
struct groundlink_t {
	EHANDLE entity;
	groundlink_t* nextLink;
	groundlink_t* prevLink;
};
