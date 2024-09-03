// Based on code by OzxyBox: https://github.com/Source-SDK-Resources/source-sdk-example-qt
#include "sourceinit.hpp"
#include <span>

#include "tier1/tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "vstdlib/cvar.h"

static const SourceApp::systemReq_t* s_request;
static size_t s_requestCount = 0;

struct loadInfo_t {
	CSysModule* sysModule;
	CreateInterfaceFn factory;
};
struct systemInfo_t {
	uint16 originModule;
	IAppSystem* app;
};

static CUtlDict<systemInfo_t, uint16> s_systemDict;
static CUtlDict<loadInfo_t, uint16> s_moduleDict;

void* Factory( const char* interfaceName, int* retCode ) {
	// Return the interface if we know it
	uint16 id = s_systemDict.Find( interfaceName );
	if ( id != CUtlDict<systemInfo_t, uint16>::InvalidIndex() ) {
		if ( retCode ) {
			*retCode = IFACE_OK;
		}
		return s_systemDict[ id ].app;
	}

	// Do any of our apps know it?
	for ( unsigned int i = 0; i < s_systemDict.Count(); i++ ) {
		if ( void* q = s_systemDict.Element( i ).app->QueryInterface( interfaceName ) ) {
			if ( retCode ) {
				*retCode = IFACE_OK;
			}
			return q;
		}
	}

	// Should we check the modules' factories?

	// Dunno what you're looking for
	if ( retCode ) {
		*retCode = IFACE_FAILED;
	}
	return nullptr;
}


void SourceApp::SetSystemRequest( std::span<SourceApp::systemReq_t> requests ) {
	s_request = requests.data();
	s_requestCount = requests.size();
}

int SourceApp::Load() {
	// Load all modules for each request, once
	for ( unsigned int i = 0; i < s_requestCount; i++ ) {
		uint16 moduleId = 0;

		uint16 idx = s_moduleDict.Find( s_request[ i ].moduleName );
		if ( idx == CUtlDict<loadInfo_t, uint16>::InvalidIndex() ) {
			// We don't have this one yet. Load it
			CSysModule* module = Sys_LoadModule( s_request[ i ].moduleName );
			if ( !module ) {
				Warning( "Failed to load module %s!\n", s_request[ i ].moduleName );
				return static_cast<int>( i + 1 );
			}

			// We need every module's factory
			CreateInterfaceFn factory = Sys_GetFactory( module );
			if ( !factory ) {
				Warning( "Failed to load module factory %s!\n", s_request[ i ].moduleName );
				return static_cast<int>( i + 1 );
			}

			// Store it
			moduleId = s_moduleDict.Insert( s_request[ i ].moduleName, { module, factory } );
		} else {
			// We have it, just assign it
			moduleId = idx;
		}


		if ( s_systemDict.HasElement( s_request[ i ].interfaceName ) ) {
			Warning( "Interface %s listed twice!", s_request[ i ].interfaceName );
			return static_cast<int>( i + 1 );
		}
		s_systemDict.Insert( s_request[ i ].interfaceName, { moduleId, nullptr } );
	}

	// Add in cvars first
	SourceApp::AddSystem( VStdLib_GetICVarFactory()( CVAR_INTERFACE_VERSION, nullptr ), CVAR_INTERFACE_VERSION );

	// Load interfaces
	for ( uint32 i = 0; i < s_requestCount; i++ ) {
		systemInfo_t& sys = s_systemDict.Element( i );
		// Ordered, so we can just access without lookup.
		int ret = 0;
		void* app = s_moduleDict[ sys.originModule ].factory( s_request[ i ].interfaceName, &ret );
		if ( !app || ret ) {
			Warning( "Failed to load system `%s` from module `%s`\n", s_request[ i ].interfaceName, s_request[ i ].moduleName );
			return static_cast<int>( i + 1 );
		}
		sys.app = reinterpret_cast<IAppSystem*>( app );
	}

	return 0;
}

void SourceApp::AddSystem( void* system, const char* interfaceName ) {
	s_systemDict.Insert( interfaceName, { CUtlDict<systemInfo_t, uint16>::InvalidIndex(), reinterpret_cast<IAppSystem*>( system ) } );
}

int SourceApp::Connect() {
	CreateInterfaceFn factory = Factory;

	ConnectTier1Libraries( &factory, 1 );
	ConnectTier2Libraries( &factory, 1 );
//	ConnectTier3Libraries( &factory, 1 );

	for ( unsigned int i = 0; i < s_systemDict.Count(); i++ ) {
		if ( !s_systemDict.Element( i ).app->Connect( factory ) ) {
			Warning( "Failed to connect %s!\n", s_systemDict.GetElementName( i ) );
			return static_cast<int>( i + 1 );
		}
	}

	return false;
}

int SourceApp::Init() {
	for ( unsigned int i = 0; i < s_systemDict.Count(); i++ ) {
		if ( s_systemDict.Element( i ).app->Init() != INIT_OK ) {
			Warning( "Failed to init %s!\n", s_systemDict.GetElementName( i ) );
			return static_cast<int>( i + 1 );
		}
	}
	return 0;
}

void SourceApp::Shutdown() {
	// We do this in reverse in hopes of not unloading something while still in use

	// Shutdown all systems
	int fff = s_systemDict.Count();
	for ( int i = fff - 1; i >= 0; i-- ) {
		s_systemDict.Element( i ).app->Shutdown();
	}

	// Disconnect all systems
	for ( int i = s_systemDict.Count() - 1; i >= 0; i-- ) {
		s_systemDict.Element( i ).app->Disconnect();
	}

	// Bye bye, tiers!
	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
//	DisconnectTier1Libraries();

	// Unload all modules
	for ( int i = static_cast<int>( s_moduleDict.Count() - 1 ); i >= 0; i-- ) {
		Sys_UnloadModule( s_moduleDict.Element( i ).sysModule );
	}
}


CreateInterfaceFn SourceApp::GetFactory() {
	return Factory;
}