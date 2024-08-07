//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "mathlib/vector.h"


// The macro texture looks for a TGA file with the same name as the BSP file and in
// the same directory. If it finds one, it maps this texture onto the world dimensions
// (in the worldspawn entity) and masks all lightmaps with it.
void InitMacroTexture( const char* pBSPFilename );
void ApplyMacroTextures( int iFace, const Vector& vWorldPos, Vector& outLuxel );
