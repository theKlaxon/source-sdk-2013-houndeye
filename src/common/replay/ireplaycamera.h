//========= Copyright Valve Corporation, All rights reserved. ============//
//
//----------------------------------------------------------------------------------------
#pragma once
#include "interface.h"

abstract_class IReplayCamera : public IBaseInterface {
public:
	virtual void ClearOverrideView() = 0;
};
