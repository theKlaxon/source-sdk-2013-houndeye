//
// Created by ENDERZOMBI102 on 03/09/2024.
//
#pragma once
#include "appframework/IAppSystemGroup.h"
#include "idedicatedexports.h"


class CGameAppLoader : public CSteamAppSystemGroup {
public:  // IAppSystem
	bool Create() override;
	bool PreInit() override;
	int Main() override;
	void PostShutdown() override;
	void Destroy() override;
private:
	bool m_Dedicated{};
};

// Server Runner
class CDedicatedExports : public CBaseAppSystem<IDedicatedExports> {
public:
	void Sys_Printf( char* text ) override;

	[[noreturn]]
	void RunServer() override;
};
