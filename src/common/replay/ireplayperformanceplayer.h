//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"


class CReplay;
class CReplayPerformance;


class IReplayPerformancePlayer : public IBaseInterface {
public:
	virtual void BeginPerformancePlay( CReplayPerformance* pPerformance ) = 0;
	virtual void EndPerformancePlay() = 0;

	virtual bool IsPlaying() const = 0;
};
