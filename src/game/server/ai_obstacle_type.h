//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once

//-------------------------------------
// AI_MoveSuggType_t
//
// Purpose: Specifies the type of suggestion. Different types have different weights
//-------------------------------------
enum AI_MoveSuggType_t {
	// Positive suggestions
	AIMST_MOVE,

	// Negative suggestions
	AIMST_AVOID_DANGER,
	AIMST_AVOID_OBJECT,
	AIMST_AVOID_NPC,
	AIMST_AVOID_WORLD,

	AIMST_NO_KNOWLEDGE,
	AIMST_OSCILLATION_DETERRANCE,

	AIMS_INVALID
};
