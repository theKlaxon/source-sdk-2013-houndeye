// Based on code by OzxyBox: https://github.com/Source-SDK-Resources/source-sdk-example-qt
#pragma once

#include "utldict.h"
#include <span>


namespace SourceApp {
	struct systemReq_t {
		const char* moduleName;
		const char* interfaceName;
	};

	// Array of system infos
	void SetSystemRequest( std::span<SourceApp::systemReq_t> requests );

	// Loads all specified modules and interfaces. Returns 0 on success or the index of the request it failed to load plus one
	int Load();

	// Returns the specified module
	template<class T>
	T* FindSystem( const char* interfaceName ) {
		extern void* Factory( const char* interfaceName, int* retCode );
		return reinterpret_cast<T*>( Factory( interfaceName, 0 ) );
	}

	// Returns the specified module
	void AddSystem( void* system, const char* interfaceName );

	// Connects all apps. Returns 0 on success or the index of the request it failed to connect plus one
	int Connect();

	// Inits all apps. Returns errors as the rest do
	int Init();

	void Shutdown();

	CreateInterfaceFn GetFactory();
}