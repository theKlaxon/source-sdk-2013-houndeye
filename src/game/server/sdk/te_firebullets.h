//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef TE_FIREBULLETS_H
#define TE_FIREBULLETS_H
#if IsWindows()
#pragma once
#endif


void TE_FireBullets( 
	int	iPlayerIndex,
	const Vector &vOrigin,
	const QAngle &vAngles,
	int	iWeaponID,
	int	iMode,
	int iSeed,
	float flSpread 
	);

#endif // TE_FIREBULLETS_H
