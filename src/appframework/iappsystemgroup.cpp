//
// Created by ENDERZOMBI102 on 02/05/2024.
//   Based on code by OzxyBox: https://github.com/Source-SDK-Resources/source-sdk-example-qt
//
#include "appframework/IAppSystemGroup.h"

static CAppSystemGroup* g_pRootAppSystem{ nullptr };

void* AppSystemCreateInterfaceFn( const char* pInterfaceName, int* pRetCode ) {
	AssertMsg( g_pRootAppSystem != nullptr, "Asked for an interface with no AppSystemGroup initialized!" );
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
	: m_Modules(), m_Systems(), m_SystemDict(), m_pParentAppSystem(pParentAppSystem) { }

int CAppSystemGroup::Run() { } // actually runs

int CAppSystemGroup::Startup() { }
void CAppSystemGroup::Shutdown() { }

CAppSystemGroup::AppSystemGroupStage_t CAppSystemGroup::GetErrorStage() const {
	return this->m_nErrorStage;
}

// protected
AppModule_t CAppSystemGroup::LoadModule( const char* pDLLName ) { }
AppModule_t CAppSystemGroup::LoadModule( CreateInterfaceFn factory ) { }

IAppSystem* CAppSystemGroup::AddSystem( AppModule_t module, const char* pInterfaceName ) {
	auto system = reinterpret_cast<IAppSystem*>( module );
	auto index{ this->m_Systems.AddToTail( system ) };
	this->m_SystemDict.Insert( pInterfaceName, index );
}
void CAppSystemGroup::AddSystem( IAppSystem* pAppSystem, const char* pInterfaceName ) {
	auto index{ this->m_Systems.AddToTail( pAppSystem ) };
	this->m_SystemDict.Insert( pInterfaceName, index );
}

bool CAppSystemGroup::AddSystems( AppSystemInfo_t* pSystems ) { }

void* CAppSystemGroup::FindSystem( const char* pInterfaceName ) { }

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




