//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//
#pragma once
#include "interface.h"

//----------------------------------------------------------------------------------------

class IReplayServerEngine : public IBaseInterface {
public:
	virtual void EndReplayRecordingSession() = 0;
	virtual bool IsReplayRecording() = 0;
	virtual bool IsReplay() = 0;
};