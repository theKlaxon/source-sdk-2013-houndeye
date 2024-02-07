//
// Created by ENDERZOMBI102 on 22/07/2023.
//
#include "launcher.hpp"
#include <array>

#include "appframework/IAppSystemGroup.h"
#include "datacache/idatacache.h"
#include "engine/hlds_api.h"
#include "engine/launcher_api.hpp"
#include "filesystem.h"
#include "filesystem/IQueuedLoader.h"
#include "filesystem_init.h"
#include "icommandline.h"
#include "inputsystem/iinputsystem.h"
#include "istudiorender.h"
#include "materialsystem/imaterialsystem.h"
#include "sourceinit.hpp"
#include "tier3/tier3.h"
#include "vphysics_interface.h"


IFileSystem* g_pFileSystem{nullptr};
IDedicatedServerAPI* g_pDedicatedServerApi{nullptr};
IStudioDataCache* g_pStudioDataCache{nullptr};
CDedicatedExports g_DedicatedExports{};

ModInfo_t s_modInfo{ nullptr, nullptr, nullptr, nullptr, nullptr, false };
char g_szBaseDir[ MAX_PATH ];
char g_szGameInfoDir[ MAX_PATH ];
bool g_bDedicatedServer{ false };

//-----------------------------------------------------------------------------
//   Main entry point
//-----------------------------------------------------------------------------
DLL_EXPORT int LauncherMain( int argc, char* argv[] ) {
	// preinit
	CommandLine()->CreateCmdLine( argc, argv );

	SpewOutputFunc( LauncherSpewFunc );

	if ( Plat_IsInDebugSession() )
		ConLog( "[I] running with debugger attached!\n" );

	g_bDedicatedServer = CommandLine()->FindParm( "-dedicated" ) != 0;

	// init
	Init();

	// main
	auto err{ Main() };

	// shutdown
	Shutdown();

	return err;
}

void Init() {
	// module loading
	std::array<SourceApp::systemReq_t, 9> appSystems {{
		{ "materialsystem", MATERIAL_SYSTEM_INTERFACE_VERSION },
		{ "studiorender", STUDIO_RENDER_INTERFACE_VERSION },
		{ "vphysics", VPHYSICS_INTERFACE_VERSION },
		{ "datacache", DATACACHE_INTERFACE_VERSION },
		{ "datacache", MDLCACHE_INTERFACE_VERSION },
		{ "datacache", STUDIO_DATA_CACHE_INTERFACE_VERSION },
		{ "filesystem_stdio", QUEUEDLOADER_INTERFACE_VERSION },
		{ "inputsystem", INPUTSYSTEM_INTERFACE_VERSION },
		{ "engine", g_bDedicatedServer ? VENGINE_HLDS_API_VERSION : VENGINE_LAUNCHER_API_VERSION }
	}};

	SourceApp::SetSystemRequest( appSystems );

	int err{ SourceApp::Load() };
	if ( err ) {
		Error( "Failed to load %d modules!\n", err );
	}

	SourceApp::AddSystem( &g_DedicatedExports, VENGINE_DEDICATEDEXPORTS_API_VERSION );

	// interconnection between modules
	err = SourceApp::Connect();
	if ( err ) {
		Error( "Failed to connect %d systems!\n", err );
	}

	// globals setup
	g_pFileSystem = SourceApp::FindSystem<IFileSystem>( FILESYSTEM_INTERFACE_VERSION );
	g_pDedicatedServerApi = SourceApp::FindSystem<IDedicatedServerAPI>( VENGINE_HLDS_API_VERSION );

	g_pDataCache = SourceApp::FindSystem<IDataCache>( DATACACHE_INTERFACE_VERSION );
	g_pStudioDataCache = SourceApp::FindSystem<IStudioDataCache>( STUDIO_DATA_CACHE_INTERFACE_VERSION );

	g_pMaterialSystem = SourceApp::FindSystem<IMaterialSystem>( MATERIAL_SYSTEM_INTERFACE_VERSION );

	auto* inputsystem = SourceApp::FindSystem<IInputSystem>( INPUTSYSTEM_INTERFACE_VERSION );

	if ( !g_pFileSystem || !g_pDedicatedServerApi || !g_pDataCache || !g_pStudioDataCache || !g_pMaterialSystem || !inputsystem ) {
		Error( "Unable to load required library interface(s)!\n" );
	}

	g_pMaterialSystem->SetShaderAPI( g_bDedicatedServer ? "shaderapiempty" : "shaderapidx9" );
	g_pMaterialSystem->Connect( SourceApp::GetFactory() );
	inputsystem->SetConsoleTextMode( g_bDedicatedServer );

	err = SetupSearchPaths();
	if ( err ) {
		Error( "Failed to setup search paths! (stage: %d)", err );
	}
}

int SetupSearchPaths() {
	// environment info
	CFSSteamSetupInfo steamInfo;
	steamInfo.m_pDirectoryName = nullptr;
	steamInfo.m_bOnlyUseDirectoryName = false;
	steamInfo.m_bToolsMode = false;
	steamInfo.m_bSetSteamDLLPath = true;
	steamInfo.m_bSteam = g_pFileSystem->IsSteam();
	if ( FileSystem_SetupSteamEnvironment( steamInfo ) != FS_OK )
		return 1;

	// gameinfo mounts
	CFSMountContentInfo fsInfo;
	fsInfo.m_pFileSystem = g_pFileSystem;
	fsInfo.m_bToolsMode = false;
	fsInfo.m_pDirectoryName = steamInfo.m_GameInfoPath;
	if ( FileSystem_MountContent( fsInfo ) != FS_OK )
		return 2;

	// search paths
	CFSSearchPathsInit searchPathsInit;
	searchPathsInit.m_pDirectoryName = steamInfo.m_GameInfoPath;
	searchPathsInit.m_pFileSystem = fsInfo.m_pFileSystem;
	if ( FileSystem_LoadSearchPaths( searchPathsInit ) != FS_OK )
		return 3;

	// add platform folder to search path
	char platform[ MAX_PATH ];
	Q_strncpy( platform, steamInfo.m_GameInfoPath, MAX_PATH );
	Q_StripTrailingSlash( platform );
	Q_strncat( platform, "/../platform", MAX_PATH, MAX_PATH );

	fsInfo.m_pFileSystem->AddSearchPath( platform, "PLATFORM" );

	strncpy( g_szGameInfoDir, steamInfo.m_GameInfoPath, MAX_PATH );

	return 0;
}

int Main() {
	FileSystem_GetExecutableDir( g_szBaseDir, MAX_PATH );
	V_StripLastDir( g_szBaseDir, MAX_PATH );
	V_StripTrailingSlash( g_szBaseDir );

	s_modInfo.m_bTextMode = g_bDedicatedServer;
	s_modInfo.m_pBaseDirectory = g_szBaseDir;
	s_modInfo.m_pInstance = nullptr;
	s_modInfo.m_pParentAppSystemGroup = nullptr;

	s_modInfo.m_pInitialGame = "hl2";
	s_modInfo.m_pInitialMod = g_szGameInfoDir;

	if ( g_bDedicatedServer ) {
		g_pDedicatedServerApi->ModInit( s_modInfo );
	} else {
		// TODO: What about the client?
	}
	return 0;
}

void Shutdown() {
	SourceApp::Shutdown();

	g_pFileSystem = nullptr;
	g_pMaterialSystem = nullptr;
	g_pDataCache = nullptr;
	g_pStudioDataCache = nullptr;
	g_pFileSystem = nullptr;
	g_pDedicatedServerApi = nullptr;
}

SpewRetval_t LauncherSpewFunc( SpewType_t spewType, char const* pMsg ) {
	switch ( spewType ) {
		case SPEW_MESSAGE:
			printf( "[I] %s", pMsg );
			break;
		case SPEW_WARNING:
			printf( "[W] %s", pMsg );
			break;
		case SPEW_ASSERT:
			printf( "[assert] %s", pMsg );
			break;
		case SPEW_ERROR:
			printf( "[E] %s", pMsg );
			break;
		case SPEW_LOG:
			printf( "[D] %s", pMsg );
			break;
		case SPEW_TYPE_COUNT:
			break;
	}
	return SpewRetval_t::SPEW_CONTINUE;
}

void CDedicatedExports::Sys_Printf( char* text ) {
	// Maybe add logging?
	puts( text );
}

void CDedicatedExports::RunServer() {
	// Main Server loop
	for ( ;; )
		g_pDedicatedServerApi->RunFrame();
}

