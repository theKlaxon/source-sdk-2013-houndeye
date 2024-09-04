//
// Created by ENDERZOMBI102 on 03/09/2024.
//
#include "steamapploader.hpp"
#include "datacache/idatacache.h"
#include "engine/launcher_api.hpp"
#include "filesystem_init.h"
#include "inputsystem/iinputsystem.h"
#include "materialsystem/imaterialsystem.h"
#include "tier3/tier3.h"
#include "icommandline.h"
#include "engine/hlds_api.h"
#include "filesystem.h"
#include "istudiorender.h"
#include "vphysics_interface.h"
#include "appframework/AppFramework.h"
// This must be the final include in a .cpp file!!!
#include "filesystem/IQueuedLoader.h"
#include "memdbgon.h"


IFileSystem* g_pFileSystem{nullptr};
namespace {
	char g_BaseDir[ MAX_PATH ];
	char g_GameInfoDir[ MAX_PATH ];
	IDedicatedServerAPI* s_DedicatedApi;
	ILauncherAPI* s_LauncherApi;
	IStudioDataCache* s_pStudioDataCache{nullptr};
	CDedicatedExports s_DedicatedExports{};
}

// ------------------
// CSteamAppLoader
// ------------------
bool CSteamAppLoader::PreInit() {
	if ( Plat_IsInDebugSession() ) {
		Log( "running with debugger attached!\n" );
	}

	m_Dedicated = CommandLine()->FindParm( "-dedicated" ) != 0;

	if (! CommandLine()->FindParm( "-game" ) ) {
		Warning( "was not given a game to load!! defaulting to `Half-Life 2`" );
		CommandLine()->AppendParm( "-game", "../hl2" );
	}

	// find the `gameinfo.txt`
	CFSSteamSetupInfo steamInfo;
	steamInfo.m_pDirectoryName = nullptr;
	steamInfo.m_bOnlyUseDirectoryName = false;
	steamInfo.m_bToolsMode = false; // CommandLine()->FindParm( "-tools" );
	steamInfo.m_bSetSteamDLLPath = true;
	steamInfo.m_bSteam = g_pFileSystem->IsSteam();
	if ( FileSystem_SetupSteamEnvironment( steamInfo ) != FS_OK ) {
		Error( "Failed to find `gameinfo.txt`! aborting." );
		return false;
	}

	// mount the game
	CFSMountContentInfo fsInfo;
	fsInfo.m_pFileSystem = g_pFileSystem;
	fsInfo.m_bToolsMode = false;
	fsInfo.m_pDirectoryName = steamInfo.m_GameInfoPath;
	if ( FileSystem_MountContent( fsInfo ) != FS_OK ) {
		Error( "Failed to mount gameinfo! aborting." );
		return false;
	}

	// add `GAME` search paths
	CFSSearchPathsInit searchPathsInit;
	searchPathsInit.m_pDirectoryName = steamInfo.m_GameInfoPath;
	searchPathsInit.m_pFileSystem = fsInfo.m_pFileSystem;
	if ( FileSystem_LoadSearchPaths( searchPathsInit ) != FS_OK ) {
		Error( "Failed to load game search paths! aborting." );
		return false;
	}

	// add platform folder to search path
	char platform[ MAX_PATH ];
	V_strncpy( platform, steamInfo.m_GameInfoPath, MAX_PATH );
	V_StripTrailingSlash( platform );
	V_strncat( platform, "/../platform", MAX_PATH, MAX_PATH );
	fsInfo.m_pFileSystem->AddSearchPath( platform, "PLATFORM" );

	MathLib_Init();
	g_pMaterialSystem->SetAdapter(0, 0);

	return true;
}
bool CSteamAppLoader::Create() {
	// module loading
	AppSystemInfo_t appSystems[16] {
		{ "materialsystem", MATERIAL_SYSTEM_INTERFACE_VERSION },  // 0
		{ "studiorender", STUDIO_RENDER_INTERFACE_VERSION },      // 1
		{ "vphysics", VPHYSICS_INTERFACE_VERSION },               // 2
		{ "datacache", DATACACHE_INTERFACE_VERSION },             // 3
		{ "datacache", MDLCACHE_INTERFACE_VERSION },              // 4
	};
	if ( m_Dedicated ) [[unlikely]] {
		// the following are only needed for the dedicated (for now, gotta research more)
		appSystems[5] = { "datacache",        STUDIO_DATA_CACHE_INTERFACE_VERSION };
		appSystems[6] = { "filesystem_stdio", QUEUEDLOADER_INTERFACE_VERSION };
		appSystems[7] = { "inputsystem",      INPUTSYSTEM_INTERFACE_VERSION };
		appSystems[8] = { "engine", VENGINE_HLDS_API_VERSION };
		appSystems[9] = { nullptr, nullptr };
	} else {
		appSystems[5] = { "engine", VENGINE_LAUNCHER_API_VERSION };
		appSystems[6] = { nullptr, nullptr };
	}

	if (! this->AddSystems( appSystems ) ) {
		return false;
	}

	auto factory{ GetFactory() };
	ConnectTier1Libraries( &factory, 1 );
	ConnectTier2Libraries( &factory, 1 );

	g_pFileSystem = static_cast<IFileSystem*>( FindSystem( FILESYSTEM_INTERFACE_VERSION ) );
	g_pMaterialSystem = static_cast<IMaterialSystem*>( FindSystem( MATERIAL_SYSTEM_INTERFACE_VERSION ) );
	g_pStudioRender = static_cast<IStudioRender*>( FindSystem( STUDIO_RENDER_INTERFACE_VERSION ) );
	g_pMDLCache = static_cast<IMDLCache*>( FindSystem( MDLCACHE_INTERFACE_VERSION ) );
	g_pDataCache = static_cast<IDataCache*>( FindSystem( DATACACHE_INTERFACE_VERSION ) );
	auto pEngineApi = FindSystem( m_Dedicated ? VENGINE_HLDS_API_VERSION : VENGINE_LAUNCHER_API_VERSION );

	if (! (g_pFileSystem && g_pMaterialSystem && g_pStudioRender && g_pMDLCache && pEngineApi) ) {
		Error( "Unable to load required library interface(s)!\n" );
		return false;
	}

	g_pMaterialSystem->SetShaderAPI( m_Dedicated ? "shaderapiempty" : "shaderapidx9" );
	g_pMaterialSystem->Connect( factory );

	// Must be done after material system is connected up!
	g_pMaterialSystemHardwareConfig = static_cast<IMaterialSystemHardwareConfig*>( FindSystem( MATERIALSYSTEM_HARDWARECONFIG_INTERFACE_VERSION ) );

	// set side-aware globals
	if ( m_Dedicated ) {
		s_DedicatedApi = static_cast<IDedicatedServerAPI*>( pEngineApi );
	} else {
		s_LauncherApi = static_cast<ILauncherAPI*>( pEngineApi );
	}

	return true;
}
int CSteamAppLoader::Main() {
	g_pMaterialSystem->ModInit();

	// dedicated start
	if ( m_Dedicated ) {
		// init the engine
		FileSystem_GetExecutableDir( g_BaseDir, MAX_PATH );
		V_StripLastDir( g_BaseDir, MAX_PATH );
		V_StripTrailingSlash( g_BaseDir );

		ModInfo_t modInfo{
			.m_pInstance = nullptr,
			.m_pBaseDirectory = g_BaseDir,
			.m_pInitialMod = g_GameInfoDir,
			.m_pInitialGame = "hl2",
			.m_pParentAppSystemGroup = this,
			.m_bTextMode = true
		};

		s_DedicatedApi->ModInit( modInfo );
	}

	return 0;
}
void CSteamAppLoader::PostShutdown() {

}
void CSteamAppLoader::Destroy() {
	DisconnectTier1Libraries();
	DisconnectTier2Libraries();

	g_pFileSystem = nullptr;
	g_pMaterialSystem = nullptr;
}


// -------------------
// CDedicatedExports
// -------------------
void CDedicatedExports::Sys_Printf( char* text ) {
	// Maybe add logging?
	puts( text );
}

void CDedicatedExports::RunServer() {
	// Main Server loop
	for ( ;; ) {
		s_DedicatedApi->RunFrame();
	}
}
