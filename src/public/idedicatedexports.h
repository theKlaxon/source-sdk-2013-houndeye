//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "appframework/IAppSystem.h"
#include "interface.h"


abstract_class IDedicatedExports : public IAppSystem {
public:
	virtual void Sys_Printf( char* text ) = 0;
	virtual void RunServer() = 0;
};

#define VENGINE_DEDICATEDEXPORTS_API_VERSION "VENGINE_DEDICATEDEXPORTS_API_VERSION003"
