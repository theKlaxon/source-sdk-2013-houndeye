//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "bspfile.h"
#include "iscratchpad3d.h"


void ScratchPad_DrawWinding( IScratchPad3D* pPad, int nPoints, Vector* pPoints, Vector vColor, Vector vOffset = Vector( 0, 0, 0 ) );

void ScratchPad_DrawFace( IScratchPad3D* pPad, dface_t* f, int iFaceNumber = -1, const CSPColor& faceColor = CSPColor( 1, 1, 1, 1 ), const Vector& vOffset = Vector( 0, 0, 0 ) );
void ScratchPad_DrawWorld( IScratchPad3D* pPad, bool bDrawFaceNumbers, const CSPColor& faceColor = CSPColor( 1, 1, 1, 1 ) );
void ScratchPad_DrawWorld( bool bDrawFaceNumbers, const CSPColor& faceColor = CSPColor( 1, 1, 1, 1 ) );
