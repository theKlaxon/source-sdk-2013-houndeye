//
// Created by ENDERZOMBI102 on 02/05/2024.
//   Based on code by OzxyBox: https://github.com/Source-SDK-Resources/source-sdk-example-qt
//
#include "appframework/IAppSystemGroup.h"

static CAppSystemGroup* g_pRootAppSystem{ nullptr };

void* AppSystemCreateInterfaceFn( const char* pInterfaceName, int* pRetCode ) {
	AssertMsg( g_pRootAppSystem != nullptr, "RootFactory was asked for an interface when no AppSystemGroup was ran!" );
	auto index = g_pRootAppSystem->m_SystemDict.Find( pInterfaceName );
	if ( index != CUtlDict<int, unsigned short>::InvalidIndex() )
		return g_pRootAppSystem->m_Systems[index];

	for ( const auto& system : g_pRootAppSystem->m_Systems ) {
		auto iface = system->QueryInterface( pInterfaceName );
		if ( iface )
			return iface;
	}

	return nullptr;
}

// public
CAppSystemGroup::CAppSystemGroup( CAppSystemGroup* pParentAppSystem )
	: m_Modules(), m_Systems(), m_SystemDict(), m_pParentAppSystem(pParentAppSystem) {
}

int CAppSystemGroup::Run() {
	g_pRootAppSystem = this;
	InitReturnVal_t res{};

	// Load
	// Connect
	if (! this->ConnectSystems() );
	// Initialize
	res = this->InitSystems();
	// Run main
	res = this->Main();
	// Shutdown
	res = this->ShutdownSystems();
	// Disconnect
	res = this->DisconnectSystems();
	// Unload
	res = this->UnloadAllModules();
}

int CAppSystemGroup::Startup() { }
void CAppSystemGroup::Shutdown() { }

CAppSystemGroup::AppSystemGroupStage_t CAppSystemGroup::GetErrorStage() const {
	return this->m_nErrorStage;
}

// protected
AppModule_t CAppSystemGroup::LoadModule( const char* pDLLName ) {
	for (  auto i{0}; i < this->m_Modules.Count(); i += 1 ) {
		if ( V_strcmp( this->m_Modules[i].m_pModuleName, pDLLName ) == 0 )
			return i;
	}

	CSysModule* sysModule{ Sys_LoadModule( pDLLName ) };
	if (! sysModule ) {
		Warning( "Failed to load module `%s`", pDLLName );
		return CUtlVector<Module_t>::InvalidIndex();
	}

	CreateInterfaceFn factory{ Sys_GetFactory( sysModule ) };
	if (! factory ) {
		Warning( "Failed to load module factory of `%s`", pDLLName );
		return CUtlVector<Module_t>::InvalidIndex();
	}

	Module_t module{ .m_pModule = sysModule, .m_Factory = factory, .m_pModuleName = pDLLName };
	return this->m_Modules.AddToTail( module );
}
AppModule_t CAppSystemGroup::LoadModule( CreateInterfaceFn factory ) {
	for ( int i{ 0 }; i < this->m_Modules.Count(); i += 1 ) {
		if ( this->m_Modules[i].m_Factory == factory )
			return i;
	}

	AssertMsg( false, "Not implemented, if you need this, ask a programmer to add it." );
	return -1;
}

IAppSystem* CAppSystemGroup::AddSystem( AppModule_t module, const char* pInterfaceName ) {
	auto index{ this->m_SystemDict.Find( pInterfaceName ) };
	if ( index == CUtlDict<int, unsigned short>::InvalidIndex() ) {
		index = this->m_Systems.AddToTail();
	} else {
		Warning( "System `%s` has been added multiple times! This will overrides the last value!", pInterfaceName );
	}

	int retCode;
	const auto mod{ this->m_Modules[ module ] };
	auto system{ reinterpret_cast<IAppSystem*>( mod.m_Factory( pInterfaceName, &retCode ) ) };
	if (! retCode ) {
		Warning( "Failed to load system for interface `%s` from module `%s`.", pInterfaceName, mod.m_pModuleName );
		return nullptr;
	}

	this->m_SystemDict.Insert( pInterfaceName, index );
	return system;
}
void CAppSystemGroup::AddSystem( IAppSystem* pAppSystem, const char* pInterfaceName ) {
	auto index{ this->m_Systems.AddToTail( pAppSystem ) };
	this->m_SystemDict.Insert( pInterfaceName, index );
}

bool CAppSystemGroup::AddSystems( AppSystemInfo_t* pSystems ) {
	int i{ 0 };
	while ( pSystems[i].m_pModuleName ) {

		i += 1;
	}
}

void* CAppSystemGroup::FindSystem( const char* pInterfaceName ) {

}

CreateInterfaceFn CAppSystemGroup::GetFactory() {
	return AppSystemCreateInterfaceFn;
}

// private
int CAppSystemGroup::OnStartup() { }
void CAppSystemGroup::OnShutdown() { }

void CAppSystemGroup::UnloadAllModules() { }
void CAppSystemGroup::RemoveAllSystems() { }

bool CAppSystemGroup::ConnectSystems() { }
void CAppSystemGroup::DisconnectSystems() { }

InitReturnVal_t CAppSystemGroup::InitSystems() { }
void CAppSystemGroup::ShutdownSystems() { }

CAppSystemGroup* CAppSystemGroup::GetParent() {
	return this->m_pParentAppSystem;
}

CSysModule* CAppSystemGroup::LoadModuleDLL( const char* pDLLName ) { }

void CAppSystemGroup::ReportStartupFailure( int nErrorStage, int nSysIndex ) { }




