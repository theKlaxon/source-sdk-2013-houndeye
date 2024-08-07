//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once


// this is used to temporarily allow the vphysics shadow object to update the entity's position
// for entities that typically ignore those updates.
struct vphysicsupdateai_t {
	float startUpdateTime;
	float stopUpdateTime;
	float savedShadowControllerMaxSpeed;
};
