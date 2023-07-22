//========= Resonance Team ============//
//
// Author: OzxyBox
// Dedicated Server Launcher
//   Note: Currently does not take input!
//         Use launch args and cfgs!
//
//=====================================//

#include <appframework/IAppSystemGroup.h>
#include <engine_hlds_api.h>
#include <filesystem_init.h>
#include <tier0/icommandline.h>
#include <appframework/AppFramework.h>
#include <inputsystem/iinputsystem.h>
#include <materialsystem/imaterialsystem.h>
#include <istudiorender.h>
#include <vphysics_interface.h>
#include <datacache/idatacache.h>
#include <filesystem/IQueuedLoader.h>
#include <idedicatedexports.h>
#include <vstdlib/cvar.h>
#include <tier1/tier1.h>
#include <tier2/tier2.h>

IFileSystem* g_pFileSystem;
IDedicatedServerAPI* g_pDedicatedServerApi;
IStudioDataCache* g_pStudioDataCache;
IDataCache* g_pDataCache;

ModInfo_t s_modInfo{ 0,0,0,0,0,0 };
char g_szBaseDir[MAX_PATH];
char g_szGameInfoDir[MAX_PATH];

// Server Runner
class CDedicatedExports : public CBaseAppSystem<IDedicatedExports>
{
public:
	
	virtual void Sys_Printf(char* text)
	{
		// Maybe add logging?
	}
	
	virtual void RunServer()
	{
		
		// Main Server loop
		for(;;)
			g_pDedicatedServerApi->RunFrame();
	
	}
};

CDedicatedExports g_DedicatedExports;


SpewRetval_t LauncherDSSpewFunc( SpewType_t spewType, char const *pMsg )
{
	printf(pMsg);
	return SpewRetval_t::SPEW_CONTINUE;
}

class CLauncherDS : public CSteamAppSystemGroup
{
public:
	// Methods of IApplication
	virtual bool Create();
	virtual bool PreInit();
	virtual int Main();
	virtual void PostShutdown();
	virtual void Destroy();

private:
	// Sets up the search paths
	bool SetupSearchPaths();
};

bool CLauncherDS::Create()
{
	SpewOutputFunc(LauncherDSSpewFunc);

	AppSystemInfo_t appSystems[] = 
	{
		{ "materialsystem.dll",		MATERIAL_SYSTEM_INTERFACE_VERSION },
		{ "studiorender.dll",		STUDIO_RENDER_INTERFACE_VERSION },
		{ "vphysics.dll",			VPHYSICS_INTERFACE_VERSION },
		{ "datacache.dll",			DATACACHE_INTERFACE_VERSION },
		{ "datacache.dll",			MDLCACHE_INTERFACE_VERSION },
		{ "datacache.dll",			STUDIO_DATA_CACHE_INTERFACE_VERSION },
		{ "engine.dll",				VENGINE_HLDS_API_VERSION },
		{ "FileSystem_Stdio.dll",	QUEUEDLOADER_INTERFACE_VERSION },
		{ "inputsystem.dll",		INPUTSYSTEM_INTERFACE_VERSION },
		{ "", "" }	// Required to terminate the list

	};

	if ( !AddSystems( appSystems ) ) 
		return false;

	AddSystem(&g_DedicatedExports, VENGINE_DEDICATEDEXPORTS_API_VERSION);

	static auto g_Factory = GetFactory();

	ConnectTier1Libraries(&g_Factory, 1);
	ConnectTier2Libraries(&g_Factory, 1);


	// Add in the cvar factory
	AppModule_t cvarModule = LoadModule(VStdLib_GetICVarFactory());
	AddSystem(cvarModule, CVAR_INTERFACE_VERSION);


	g_pFileSystem = (IFileSystem*)FindSystem( FILESYSTEM_INTERFACE_VERSION );
	g_pDedicatedServerApi = (IDedicatedServerAPI*)FindSystem( VENGINE_HLDS_API_VERSION );
	
	g_pDataCache = (IDataCache*)FindSystem(DATACACHE_INTERFACE_VERSION);
	g_pStudioDataCache = (IStudioDataCache*)FindSystem(STUDIO_DATA_CACHE_INTERFACE_VERSION);
	
	g_pMaterialSystem = (IMaterialSystem*)FindSystem(MATERIAL_SYSTEM_INTERFACE_VERSION);

	IInputSystem* inputsystem = (IInputSystem*)FindSystem(INPUTSYSTEM_INTERFACE_VERSION);
	


	if (!g_pFileSystem || !g_pDedicatedServerApi || !g_pDataCache || !g_pStudioDataCache || !g_pMaterialSystem || !inputsystem)
	{
		Error("Unable to load required library interface!\n");
		return false;
	}

	g_pMaterialSystem->SetShaderAPI("shaderapiempty.dll");
	g_pMaterialSystem->Connect(GetFactory());
	inputsystem->SetConsoleTextMode(true);

	return true;
}


void CLauncherDS::Destroy()
{
	DisconnectTier1Libraries();
	DisconnectTier2Libraries();

	g_pFileSystem = NULL;
	g_pMaterialSystem = NULL;
	g_pDataCache = NULL;
	g_pStudioDataCache = NULL;
	g_pFileSystem = nullptr;
	g_pDedicatedServerApi = nullptr;
}


//-----------------------------------------------------------------------------
// Sets up the game path
//-----------------------------------------------------------------------------
bool CLauncherDS::SetupSearchPaths()
{
	CFSSteamSetupInfo steamInfo;
	steamInfo.m_pDirectoryName = NULL;
	steamInfo.m_bOnlyUseDirectoryName = false;
	steamInfo.m_bToolsMode = true;
	steamInfo.m_bSetSteamDLLPath = true;
	steamInfo.m_bSteam = g_pFileSystem->IsSteam();
	if ( FileSystem_SetupSteamEnvironment( steamInfo ) != FS_OK )
		return false;

	CFSMountContentInfo fsInfo;
	fsInfo.m_pFileSystem = g_pFileSystem;
	fsInfo.m_bToolsMode = true;
	fsInfo.m_pDirectoryName = steamInfo.m_GameInfoPath;

	if ( FileSystem_MountContent( fsInfo ) != FS_OK )
		return false;

	// Finally, load the search paths for the "GAME" path.
	CFSSearchPathsInit searchPathsInit;
	searchPathsInit.m_pDirectoryName = steamInfo.m_GameInfoPath;
	searchPathsInit.m_pFileSystem = fsInfo.m_pFileSystem;
	if ( FileSystem_LoadSearchPaths( searchPathsInit ) != FS_OK )
		return false;

	char platform[MAX_PATH];
	Q_strncpy( platform, steamInfo.m_GameInfoPath, MAX_PATH );
	Q_StripTrailingSlash( platform );
	Q_strncat( platform, "/../platform", MAX_PATH, MAX_PATH );

	fsInfo.m_pFileSystem->AddSearchPath( platform, "PLATFORM" );

	strncpy(g_szGameInfoDir, steamInfo.m_GameInfoPath, MAX_PATH);
	

	return true;
}


//-----------------------------------------------------------------------------
// Init, shutdown
//-----------------------------------------------------------------------------
bool CLauncherDS::PreInit( )
{
	// Add paths...
	if ( !SetupSearchPaths() )
		return false;


	return true; 
}

void CLauncherDS::PostShutdown()
{
}

//-----------------------------------------------------------------------------
// main application
//-----------------------------------------------------------------------------
int CLauncherDS::Main()
{
	FileSystem_GetExecutableDir(g_szBaseDir, MAX_PATH);
	V_StripLastDir(g_szBaseDir, MAX_PATH);
	V_StripTrailingSlash(g_szBaseDir);

	s_modInfo.m_bTextMode = true;
	s_modInfo.m_pBaseDirectory = g_szBaseDir;
	s_modInfo.m_pInstance = GetAppInstance();
	s_modInfo.m_pParentAppSystemGroup = this;

	s_modInfo.m_pInitialGame = "hl2";
	s_modInfo.m_pInitialMod = g_szGameInfoDir;

	g_pDedicatedServerApi->ModInit(s_modInfo);

	return 0;
}


//-----------------------------------------------------------------------------
//   Main entry point
//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	CommandLine()->CreateCmdLine( argc, argv );

	CLauncherDS launcherds;
	CSteamApplication steamApplication( &launcherds );
	return steamApplication.Run();
}
