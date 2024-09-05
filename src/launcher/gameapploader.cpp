//
// Created by ENDERZOMBI102 on 03/09/2024.
//
#include "gameapploader.hpp"
#include "appframework/AppFramework.h"
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
#include "tier3/tier3.h"
#include "vphysics_interface.h"
#include "vstdlib/cvar.h"
#include "vstdlib/iprocessutils.h"
// This must be the final include in a .cpp file!!!
#include "memdbgon.h"


IFileSystem* g_pFileSystem{nullptr};
namespace {
	char s_BaseDir[ MAX_PATH ];
	char s_GameInfoDir[ MAX_PATH ];
	CDedicatedExports s_DedicatedExports{};
	IDedicatedServerAPI* s_DedicatedApi{nullptr};
	ILauncherAPI* s_LauncherApi{nullptr};
	IStudioDataCache* s_pStudioDataCache{nullptr};
}

// ------------------
// CSteamAppLoader
// ------------------
bool CGameAppLoader::Create() {
	// are we running a dedicated server?
	m_Dedicated = CommandLine()->FindParm( "-dedicated" ) != 0;

	// base dependencies
	AppSystemInfo_t appSystems[] {
		{ "materialsystem", MATERIAL_SYSTEM_INTERFACE_VERSION },
		{ "studiorender"  , STUDIO_RENDER_INTERFACE_VERSION   },
		{ "vphysics"      , VPHYSICS_INTERFACE_VERSION        },
		{ "datacache"     , DATACACHE_INTERFACE_VERSION       },
		{ "datacache"     , MDLCACHE_INTERFACE_VERSION        },
		{ nullptr, nullptr }
	};
	if (! AddSystems( appSystems ) ) {
		return false;
	}

	Assert( AddSystem( LoadModule( "filesystem_stdio" ), QUEUEDLOADER_INTERFACE_VERSION ) );
	if ( m_Dedicated ) {
		// the following are only needed for the dedicated (for now, gotta research more)
		Assert( AddSystem( LoadModule( "datacache" ), STUDIO_DATA_CACHE_INTERFACE_VERSION ) );
		Assert( AddSystem( LoadModule( "inputsystem" ), INPUTSYSTEM_INTERFACE_VERSION ) );
		Assert( AddSystem( LoadModule( "engine" ), VENGINE_HLDS_API_VERSION ) );
	} else {
		Assert( AddSystem( LoadModule( "engine" ), VENGINE_LAUNCHER_API_VERSION ) );
	}

	if ( auto* matSys = FindSystem<IMaterialSystem>( MATERIAL_SYSTEM_INTERFACE_VERSION ) ) {
		auto loadEmpty{ m_Dedicated || CommandLine()->FindParm( "-noshaderapi" ) != 0 };
		matSys->SetShaderAPI( loadEmpty ? "shaderapiempty" : "shaderapidx9" );
	}

	return true;
}
bool CGameAppLoader::PreInit() {
	// set side-aware globals
	if ( m_Dedicated ) {
		QUEUEDLOADER_INTERFACE_VERSION;
		s_DedicatedApi = FindSystem<IDedicatedServerAPI>( VENGINE_HLDS_API_VERSION );
	} else {
		s_LauncherApi = FindSystem<ILauncherAPI>( VENGINE_LAUNCHER_API_VERSION );
	}

	auto factory{ GetFactory() };
	ConnectTier1Libraries( &factory, 1 );
	ConnectTier2Libraries( &factory, 1 );
	g_pMaterialSystem->Connect( factory );

	// Must be done after material system is connected up!
	g_pMaterialSystemHardwareConfig = FindSystem<IMaterialSystemHardwareConfig>( MATERIALSYSTEM_HARDWARECONFIG_INTERFACE_VERSION );

	if (! g_pMaterialSystemHardwareConfig ) {
		return false;
	}

	// load globals
	// generally-useful globals
	g_pFileSystem = FindSystem<IFileSystem>( FILESYSTEM_INTERFACE_VERSION );
	g_pMaterialSystem = FindSystem<IMaterialSystem>( MATERIAL_SYSTEM_INTERFACE_VERSION );
	g_pStudioRender = FindSystem<IStudioRender>( STUDIO_RENDER_INTERFACE_VERSION );
	g_pDataCache = FindSystem<IDataCache>( DATACACHE_INTERFACE_VERSION );
	g_pMDLCache = FindSystem<IMDLCache>( MDLCACHE_INTERFACE_VERSION );

	if (! (g_pFileSystem && g_pMaterialSystem && g_pStudioRender && g_pMDLCache && (s_DedicatedApi || s_LauncherApi)) ) {
		Error( "Unable to load required library interface(s)!\n" );
		return false;
	}

	MathLib_Init();
	g_pMaterialSystem->SetAdapter(0, 0);

	g_pMaterialSystem->ModInit();
	return true;
}
int CGameAppLoader::Main() {

	if ( m_Dedicated ) [[unlikely]] {
		FileSystem_GetExecutableDir( s_BaseDir, MAX_PATH );
		V_StripLastDir( s_BaseDir, MAX_PATH );
		V_StripTrailingSlash( s_BaseDir );

		ModInfo_t modInfo{
			.m_pInstance = nullptr,
			.m_pBaseDirectory = s_BaseDir,
			.m_pInitialMod = s_GameInfoDir,
			.m_pInitialGame = "hl2",
			.m_pParentAppSystemGroup = this,
			.m_bTextMode = true
		};

		s_DedicatedApi->ModInit( modInfo );
	} else {
		s_LauncherApi->Run();
	}

	return 0;
}
void CGameAppLoader::PostShutdown() {
	g_pMaterialSystem->ModShutdown();
}
void CGameAppLoader::Destroy() {
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
