//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once


#define MAX_LUMPFILES 128

//-----------------------------------------------------------------------------
// Lump files
//-----------------------------------------------------------------------------
void GenerateLumpFileName( const char* bspfilename, char* lumpfilename, int iBufferSize, int iIndex );
