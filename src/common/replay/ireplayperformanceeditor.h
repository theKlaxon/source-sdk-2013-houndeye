//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"


class CReplay;

//
// Interface to allow the replay DLL to talk to the actual UI.
//
class IReplayPerformanceEditor : public IBaseInterface {
public:
	virtual CReplay* GetReplay() = 0;
	virtual void OnRewindComplete() = 0;
};
