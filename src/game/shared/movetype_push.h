//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


const int MAX_PUSHED_ENTITIES = 32;
struct physicspushlist_t {
	float localMoveTime;
	Vector localOrigin;
	QAngle localAngles;
	int pushedCount;
	EHANDLE pushedEnts[ MAX_PUSHED_ENTITIES ];
	Vector pushVec[ MAX_PUSHED_ENTITIES ];
};
