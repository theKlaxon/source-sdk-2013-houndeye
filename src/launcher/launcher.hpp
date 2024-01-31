//
// Created by ENDERZOMBI102 on 11/09/2023.
//
#pragma once

#include "appframework/IAppSystem.h"
#include "dbg.h"
#include "idedicatedexports.h"


SpewRetval_t LauncherSpewFunc( SpewType_t spewType, char const* pMsg );

// Server Runner
class CDedicatedExports : public CBaseAppSystem<IDedicatedExports> {
public:
	void Sys_Printf( char* text ) override;

	[[noreturn]] void RunServer() override;
};

void Init();
int SetupSearchPaths();
int Main();
void Shutdown();
