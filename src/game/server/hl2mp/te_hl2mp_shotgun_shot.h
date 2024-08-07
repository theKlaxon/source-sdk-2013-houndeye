//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once


void TE_HL2MPFireBullets(
	int iPlayerIndex,
	const Vector& vOrigin,
	const Vector& vDir,
	int iAmmoID,
	int iSeed,
	int iShots,
	float flSpread,
	bool bDoTracers,
	bool bDoImpacts );
