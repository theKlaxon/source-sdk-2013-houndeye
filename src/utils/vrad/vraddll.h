//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#pragma once
#include "ilaunchabledll.h"
#include "ivraddll.h"


class CVRadDLL : public IVRadDLL, public ILaunchableDLL {
public: // IVRadDLL overrides.
	virtual int main( int argc, char** argv );
	virtual bool Init( char const* pFilename );
	virtual void Release();
	virtual void GetBSPInfo( CBSPInfo* pInfo );
	virtual bool DoIncrementalLight( char const* pVMFFile );
	virtual bool Serialize();
	virtual float GetPercentComplete();
	virtual void Interrupt();
};
