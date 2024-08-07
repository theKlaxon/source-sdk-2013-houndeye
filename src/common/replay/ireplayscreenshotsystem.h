//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

//----------------------------------------------------------------------------------------

struct WriteReplayScreenshotParams_t;

//----------------------------------------------------------------------------------------

//
// Implementation lives in the client - allows replay to tell the client to grab a
// screenshot or update the cache.
//
class IReplayScreenshotSystem : public IBaseInterface {
public:
	virtual void WriteReplayScreenshot( WriteReplayScreenshotParams_t& params ) = 0;
	virtual void UpdateReplayScreenshotCache() = 0;
};
