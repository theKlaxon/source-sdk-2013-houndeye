//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This file loads a KeyValues file containing material name mappings.
//			When the bsp is compiled, all materials listed in the file will
//			be replaced by the second material in the pair.
//
//=============================================================================
#pragma once


extern bool g_ReplaceMaterials;

// Setup / Cleanup
void LoadMaterialReplacementKeys( const char* gamedir, const char* mapname );
void DeleteMaterialReplacementKeys();

// Takes a material name and returns it's replacement, if there is one.
// If there isn't a replacement, it returns the original.
const char* ReplaceMaterialName( const char* name );
