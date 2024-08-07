//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#pragma once


class ISceneTokenProcessor;

ISceneTokenProcessor* GetTokenProcessor();
void SetTokenProcessorBuffer( const char* buf );
