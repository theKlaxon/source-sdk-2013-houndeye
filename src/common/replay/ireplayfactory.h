//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

class CReplay;

abstract_class IReplayFactory : public IBaseInterface {
public:
	virtual CReplay* Create() = 0;
};

#define INTERFACE_VERSION_REPLAY_FACTORY "IReplayFactory001"
