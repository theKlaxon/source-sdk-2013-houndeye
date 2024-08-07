//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

//----------------------------------------------------------------------------------------

class CReplayPerformance;
class CReplay;

//----------------------------------------------------------------------------------------

class IReplayPerformanceManager : public IBaseInterface {
public:
	virtual CReplayPerformance* CreatePerformance( CReplay* pReplay ) = 0;
	virtual void DeletePerformance( CReplayPerformance* pPerformance ) = 0;

	virtual const char* GetRelativePath() const = 0;
	virtual const char* GetFullPath() const = 0;

	virtual const char* GeneratePerformanceFilename( CReplay* pReplay ) = 0;
};
