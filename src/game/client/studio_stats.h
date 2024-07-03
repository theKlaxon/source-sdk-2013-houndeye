//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef __STUDIO_STATS_H
#define __STUDIO_STATS_H

#if IsWindows()
#pragma once
#endif

void StudioStats_FindClosestEntity( CClientRenderablesList *pClientRenderablesList );

extern IClientRenderable	*g_pStudioStatsEntity;


#endif // __STUDIO_STATS_H
