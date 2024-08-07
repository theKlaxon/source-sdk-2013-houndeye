//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


#include "mathlib/vector.h"


// angles comes from the "angles" property
//
// yaw and pitch will override the values in angles if they are nonzero
// yaw comes from the (obsolete) "angle" property
// pitch comes from the "pitch" property
void SetupLightNormalFromProps( const QAngle& angles, float yaw, float pitch, Vector& output );
