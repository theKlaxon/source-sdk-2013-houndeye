//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include "builddisp.h"


// Blend the normals of neighboring displacement surfaces so they match at edges and corners.
void SmoothNeighboringDispSurfNormals( CCoreDispInfo **ppListBase, int listSize );
