//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include "vrad.h"


// MPI uses these.
struct transfer_t;
transfer_t* BuildVisLeafs_Start();

// If PatchCB is non-null, it is called after each row is generated (used by MPI).
void BuildVisLeafs_Cluster( int threadnum,  transfer_t *transfers, int iCluster );

void BuildVisLeafs_End( transfer_t *transfers );
