//
// Created by ENDERZOMBI102 on 02/05/2024.
//   Based on code by OzxyBox: https://github.com/Source-SDK-Resources/source-sdk-example-qt
//
#include "appframework/IAppSystemGroup.h"

namespace {
	CAppSystemGroup* s_pRootAppSystem{ nullptr };
	int s_FailedSystemIndex{ -1 };
}

// public
CAppSystemGroup::CAppSystemGroup( CAppSystemGroup* pParentAppSystem )
	: m_Modules(), m_Systems(), m_SystemDict(), m_pParentAppSystem(pParentAppSystem) {
}

int CAppSystemGroup::Run() {
	s_pRootAppSystem = this;

	// Load
	((void)0);
	// Connect
	this->ConnectSystems();
	// Pre Init
	this->PreInit();
	// Initialize
	this->InitSystems();
	// Run main
	this->Main();
	// Shutdown
	this->ShutdownSystems();
	// Post Shutdown
	this->PostShutdown();
	// Disconnect
	this->DisconnectSystems();
	// Unload
	this->UnloadAllModules();

    return 0;
}

int CAppSystemGroup::Startup() { return 0; }
void CAppSystemGroup::Shutdown() { }

CAppSystemGroup::AppSystemGroupStage_t CAppSystemGroup::GetErrorStage() const {
	return this->m_nErrorStage;
}

// protected
AppModule_t CAppSystemGroup::LoadModule( const char* pDLLName ) {
	for (  auto i{0}; i < this->m_Modules.Count(); i += 1 ) {
		if ( V_strcmp( this->m_Modules[i].m_pModuleName, pDLLName ) == 0 ) {
			return i;
		}
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
		if ( this->m_Modules[i].m_Factory == factory ) {
			return i;
		}
	}

	AssertMsg( false, "Not implemented, if you need this, ask a programmer to add it." );
	return -1;
}

IAppSystem* CAppSystemGroup::AddSystem( AppModule_t module, const char* pInterfaceName ) {
	auto index{ this->m_SystemDict.Find( pInterfaceName ) };
	if ( index == CUtlDict<int, uint16>::InvalidIndex() ) {
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
	for ( int i{0}; pSystems[i].m_pModuleName; i += 1 ) {
		auto info{ pSystems[i] };
		auto module{ LoadModule( info.m_pModuleName ) };
		if ( module == CUtlVector<Module_t>::InvalidIndex() ) {
			s_FailedSystemIndex = i;
			m_nErrorStage = AppSystemGroupStage_t::CREATION;
			return false;
		}

		if (! AddSystem( module,  info.m_pInterfaceName ) ) {
			s_FailedSystemIndex = i;
			m_nErrorStage = AppSystemGroupStage_t::CREATION;
			return false;
		}
	}

    return true;
}

void* CAppSystemGroup::FindSystem( const char* pInterfaceName ) {
	auto index = s_pRootAppSystem->m_SystemDict.Find( pInterfaceName );
	if ( index != CUtlDict<int, uint16>::InvalidIndex() ) {
		return s_pRootAppSystem->m_Systems[index];
	}
	return {};
}

CreateInterfaceFn CAppSystemGroup::GetFactory() {
	return []( const char* pInterfaceName, int* pRetCode ) -> void* {
		AssertMsg( s_pRootAppSystem != nullptr, "RootFactory was asked for an interface when no AppSystemGroup was ran!" );
		auto index = s_pRootAppSystem->m_SystemDict.Find( pInterfaceName );
		if ( index != CUtlDict<int, uint16>::InvalidIndex() ) {
			if ( pRetCode ) {
				*pRetCode = IFACE_OK;
			}
			return s_pRootAppSystem->m_Systems[index];
		}

		for ( const auto& system : s_pRootAppSystem->m_Systems ) {
			if ( auto* iface = system->QueryInterface( pInterfaceName ) ) {
				if ( pRetCode ) {
					*pRetCode = IFACE_OK;
				}
				return iface;
			}
		}

		if ( pRetCode ) {
			*pRetCode = IFACE_FAILED;
		}
		return nullptr;
	};
}

// private
int CAppSystemGroup::OnStartup() { return {}; }
void CAppSystemGroup::OnShutdown() { }

void CAppSystemGroup::UnloadAllModules() { }
void CAppSystemGroup::RemoveAllSystems() { }

bool CAppSystemGroup::ConnectSystems() {
	const auto factory{ GetFactory() };

	for ( int i{0}; i < m_Systems.Size(); i += 1 ) {
		if (! m_Systems[i]->Connect( factory ) ) {
			s_FailedSystemIndex = i;
			m_nErrorStage = AppSystemGroupStage_t::CONNECTION;
			return false;
		}
	}

	return true;
}
void CAppSystemGroup::DisconnectSystems() { }

InitReturnVal_t CAppSystemGroup::InitSystems() { return {}; }
void CAppSystemGroup::ShutdownSystems() { }

CAppSystemGroup* CAppSystemGroup::GetParent() {
	return this->m_pParentAppSystem;
}

CSysModule* CAppSystemGroup::LoadModuleDLL( const char* pDLLName ) {
	return Sys_LoadModule( pDLLName );
}

void CAppSystemGroup::ReportStartupFailure( int nErrorStage, int nSysIndex ) { }




