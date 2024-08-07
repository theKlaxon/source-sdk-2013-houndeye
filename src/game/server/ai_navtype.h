//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once

// ---------------------------
//  Navigation Type Bits
// ---------------------------
enum Navigation_t {
	NAV_NONE = -1, // error condition
	NAV_GROUND = 0,// walk/run
	NAV_JUMP,      // jump/leap
	NAV_FLY,       // can fly, move all around
	NAV_CLIMB,     // climb ladders
};
