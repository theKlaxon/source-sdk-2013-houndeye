//
// Created by ENDERZOMBI102 on 31/08/2024.
//
#include "appframework/AppFramework.h"
#include "tier0/icommandline.h"
#include "filesystem_init.h"

namespace {
	void* s_AppInstance{ nullptr };
}


void* GetAppInstance() {
	return s_AppInstance;
}

void SetAppInstance( void* hInstance ) {
	s_AppInstance = hInstance;
}

int AppMain( void* hInstance, void* hPrevInstance, const char* lpCmdLine, int nCmdShow, CAppSystemGroup* pAppSystemGroup ) {
	CommandLine()->CreateCmdLine( lpCmdLine );
	s_AppInstance = hInstance;
	return pAppSystemGroup->Run();
}
int AppMain( int argc, char** argv, CAppSystemGroup* pAppSystemGroup ) {
	CommandLine()->CreateCmdLine( argc, argv );
	return pAppSystemGroup->Run();
}

int AppStartup( void* hInstance, void* hPrevInstance, const char* lpCmdLine, int nCmdShow, CAppSystemGroup* pAppSystemGroup ) {
	CommandLine()->CreateCmdLine( lpCmdLine );
	s_AppInstance = hInstance;
	return pAppSystemGroup->Startup();
}
int AppStartup( int argc, char** argv, CAppSystemGroup* pAppSystemGroup ) {
	CommandLine()->CreateCmdLine( argc, argv );
	return pAppSystemGroup->Startup();
}
void AppShutdown( CAppSystemGroup* pAppSystemGroup ) {
	pAppSystemGroup->Shutdown();
}



// --- CSteamApplication ---
CSteamApplication::CSteamApplication( CSteamAppSystemGroup* pAppSystemGroup )
	: m_pChildAppSystemGroup{ pAppSystemGroup }, CAppSystemGroup() {
}

int CSteamApplication::Startup() {
	return this->m_pChildAppSystemGroup->Startup();
}
void CSteamApplication::Shutdown() {
	this->m_pChildAppSystemGroup->Shutdown();
}

// CSteamApplication - IAppSystem
bool CSteamApplication::Create() {
	char fsDllName[1024];
	bool steam;
	FileSystem_SetErrorMode( FSErrorMode_t::FS_ERRORMODE_AUTO );
	const auto res{ FileSystem_GetFileSystemDLLName( fsDllName, 1024, steam ) };
	if ( res != FSReturnCode_t::FS_OK ) {
		const char* error{ "N/D" };
		switch ( res ) {
			case FS_MISSING_GAMEINFO_FILE: error = "MISSING_GAMEINFO_FILE"; break;
			case FS_INVALID_GAMEINFO_FILE: error = "INVALID_GAMEINFO_FILE"; break;
			case FS_INVALID_PARAMETERS: error = "INVALID_PARAMETERS"; break;
			case FS_UNABLE_TO_INIT: error = "UNABLE_TO_INIT"; break;
			case FS_MISSING_STEAM_DLL: error = "MISSING_STEAM_DLL"; break;
		}
		Warning( "Failed to find filesystem module. (%s)", error );
		return false;
	}

	return this->m_pChildAppSystemGroup->Create();
}
bool CSteamApplication::PreInit() {
	return this->m_pChildAppSystemGroup->PreInit();
}
int CSteamApplication::Main() {
	return this->m_pChildAppSystemGroup->Main();
}
void CSteamApplication::PostShutdown() {
	this->m_pChildAppSystemGroup->PostShutdown();
}
void CSteamApplication::Destroy() {
	this->m_pChildAppSystemGroup->Destroy();
}
