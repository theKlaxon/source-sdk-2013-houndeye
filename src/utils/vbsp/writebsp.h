//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "bspfile.h"
#include "utlmap.h"

struct node_t;

//-----------------------------------------------------------------------------
// Emits occluder faces
//-----------------------------------------------------------------------------
void EmitOccluderFaces( node_t* node );


//-----------------------------------------------------------------------------
// Purpose: Free the list of faces stored at the leaves
//-----------------------------------------------------------------------------
void FreeLeafFaces( face_t* pLeafFaceList );

//-----------------------------------------------------------------------------
// Purpose: Make sure that we have a water lod control entity if we have water in the map.
//-----------------------------------------------------------------------------
void EnsurePresenceOfWaterLODControlEntity();
