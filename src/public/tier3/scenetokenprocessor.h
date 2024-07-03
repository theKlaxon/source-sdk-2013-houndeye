//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef SCENETOKENPROCESSOR_H
#define SCENETOKENPROCESSOR_H
#if IsWindows()
#pragma once
#endif

class ISceneTokenProcessor;

ISceneTokenProcessor *GetTokenProcessor();
void SetTokenProcessorBuffer( const char *buf );

#endif // SCENETOKENPROCESSOR_H
