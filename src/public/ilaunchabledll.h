//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#pragma once

#define LAUNCHABLE_DLL_INTERFACE_VERSION "launchable_dll_1"


// vmpi_service can use this to debug worker apps in-process,
// and some of the launchers (like texturecompile) use this.
class ILaunchableDLL {
public:
	// All tools with a launcher do is load the tool's module (.dll, .so, etc.) and run this.
	virtual int main( int argc, char **argv ) = 0;
};
