//========= Copyright Valve Corporation, All rights reserved. ============//
//
//=======================================================================================//

#ifndef ISERVERENGINE_H
#define ISERVERENGINE_H
#if IsWindows()
#pragma once
#endif

//----------------------------------------------------------------------------------------

#include "interface.h"

//----------------------------------------------------------------------------------------

class IReplayServerEngine : public IBaseInterface
{
public:
	virtual void EndReplayRecordingSession() = 0;
	virtual bool IsReplayRecording() = 0;
	virtual bool IsReplay() = 0;
};

//----------------------------------------------------------------------------------------

#endif // ISERVERENGINE_H