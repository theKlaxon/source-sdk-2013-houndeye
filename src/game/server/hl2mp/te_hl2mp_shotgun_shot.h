//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef TE_HL2MP_SHOTGUN_SHOT_H
#define TE_HL2MP_SHOTGUN_SHOT_H
#if IsWindows()
#pragma once
#endif


void TE_HL2MPFireBullets( 
	int	iPlayerIndex,
	const Vector &vOrigin,
	const Vector &vDir,
	int	iAmmoID,
	int iSeed,
	int iShots,
	float flSpread, 
	bool bDoTracers,
	bool bDoImpacts );


#endif // TE_HL2MP_SHOTGUN_SHOT_H
