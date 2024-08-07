//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#pragma once
#include "particle_litsmokeemitter.h"
#include "particles_simple.h"


// Use these to fire and forget particles.
// pParticle should be ON THE STACK - ie: don't allocate it from a CSimpleEmitter or from the particle manager.
// Just make one on the stack, fill in its parameters, and pass it in here.
void AddSimpleParticle( const SimpleParticle* pParticle, PMaterialHandle hMaterial, bool bInSkybox = false );
void AddEmberParticle( const SimpleParticle* pParticle, PMaterialHandle hMaterial, bool bInSkybox = false );
void AddFireSmokeParticle( const SimpleParticle* pParticle, PMaterialHandle hMaterial, bool bInSkybox = false );
void AddFireParticle( const SimpleParticle* pParticle, PMaterialHandle hMaterial, bool bInSkybox = false );


// Called by the renderer to draw all the particles.
void DrawParticleSingletons( bool bInSkybox );
