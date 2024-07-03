//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef FONTABC_H
#define FONTABC_H
#if IsWindows()
#pragma once
#endif

typedef struct
{
	int		abcA, abcB, abcC;
	int		total;
} FONTABC;

#endif // FONTABC_H
